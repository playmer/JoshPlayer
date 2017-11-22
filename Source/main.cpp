#include <qapplication.h>
#include <qmainwindow.h>
#include <qdockwidget.h>
#include <qtextedit.h>
#include <qmenubar.h>
#include <qtoolbar.h>
#include <qaction.h>
#include <qtreewidget.h>
#include <qstylefactory.h>

#include <QStandardItem>
#include <QFileSystemModel>
#include <QFileDialog>
//#include <QtSql/QSqlTableModel>
//#include <QtSql/QSqlRecord>
//#include <QtSql/QSqlField>
#include <QTableView>

#include <QListView>

#include <string>

#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>


#include <vlc/vlc.h>

#include "Library.hpp"
#include "MusicPlayer.hpp"


namespace fs = std::experimental::filesystem;

// Our boy Nick gave us this.
void SetDarkTheme(QApplication& app)
{
  app.setStyle(QStyleFactory::create("Fusion"));

  QPalette darkPalette;
  darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
  darkPalette.setColor(QPalette::WindowText, Qt::white);
  darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
  darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
  darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
  darkPalette.setColor(QPalette::ToolTipText, Qt::white);
  darkPalette.setColor(QPalette::Text, Qt::white);
  darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
  darkPalette.setColor(QPalette::ButtonText, Qt::white);
  darkPalette.setColor(QPalette::BrightText, Qt::red);
  darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

  darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
  darkPalette.setColor(QPalette::HighlightedText, Qt::black);

  app.setPalette(darkPalette);
  app.setStyleSheet("QToolTip { color: #101010; background-color: #2a82da; border: 1px solid white; }");
}

namespace Environment
{
  constexpr bool Debug()
  {
    #ifdef NDEBUG
        return false;
    #else
        return true;
    #endif
  }

  constexpr bool Release()
  {
    return !Debug();
  }

  constexpr bool x64()
  {  // Check windows
    #if _WIN32 || _WIN64
      #if _WIN64
        return true;
      #else
        return false;
      #endif
    #endif

        // Check GCC
    #if __GNUC__
      #if __x86_64__ || __ppc64__
        return true;
      #else
        return false;
      #endif
    #endif
  }

  constexpr bool x86()
  {
    return !x64();
  }
}



void MakeTreeDockables(QMainWindow *window, Qt::DockWidgetArea area, QAbstractItemModel *model)
{
  // Self explanatory
  QTreeView *tree = new QTreeView();
  // Not as much, a model is a set of data that the tree and various 
  // other objects can act on. In this case, this is the data that the 
  // tree view will allow us to traverse.
  tree->setModel(model);
  tree->show();

  // DockWidgets are like TabWidgets, but for the dock area. These 
  // Widgets can be pulled from the dock, put into their own windows, 
  // and moved back into the docking area.
  QDockWidget *dock = new QDockWidget("FileTree", window);
  dock->setAllowedAreas(Qt::BottomDockWidgetArea | 
                        Qt::TopDockWidgetArea |
                        Qt::LeftDockWidgetArea | 
                        Qt::RightDockWidgetArea);

  dock->setWidget(tree);

  window->addDockWidget(area, dock);
}

class FileMenu : public QMenu
{
public:

  FileMenu(QWidget *aParent, Library *aLibrary)
    : QMenu("Files", aParent)
    , mLibrary(aLibrary)
  {
    auto scanAction = new QAction("Scan", this);
    this->connect(scanAction, &QAction::triggered, this, &FileMenu::ScanLibrary);
    this->addAction(scanAction);
  }

  void ScanLibrary()
  {
    fs::path dir = QFileDialog::getExistingDirectory().toStdString();
    mLibrary->ScanLibrary(dir);
  }

private:

  Library *mLibrary;
};

int main()
{
  QCoreApplication::addLibraryPath("./");
  QCoreApplication::addLibraryPath("C:/Qt/5.8/msvc2015_64/bin");

  int argc = 0;

  // Unsure if required.
  QApplication app{ argc, nullptr };

  SetDarkTheme(app);

  // Make a window. This type has support for docking and gives a 
  // central window in the middle of the docking panels that doesn't move.
  QMainWindow window{};

  // Enables "infinite docking".
  window.setDockNestingEnabled(true);

  // Sets the default window size.
  window.resize(850, 700);

  // Used to store widgets in the central widget.
  QTabWidget *centralTabs = new QTabWidget(&window);
  window.setCentralWidget(centralTabs);



  // Adding a menuTab layer for menu options.
  QMenuBar *menuTabs = new QMenuBar(centralTabs);
  window.setMenuBar(menuTabs);


  QToolBar *toolTabs = new QToolBar(centralTabs);
  window.addToolBar(toolTabs);

  MusicPlayer player{ toolTabs };
      
  // You can move the tabs around.
  centralTabs->setMovable(true);
  // Tabs get close buttons.
  centralTabs->setTabsClosable(true);
  // When there are two many tabs, this will make buttons to scroll 
  // through them.
  centralTabs->setUsesScrollButtons(true);

  auto library = std::make_unique<Library>(&window);

  if (fs::exists("Library.txt"))
  {
    library->LoadLibrary("Library.txt");
  }

  // Add tab options for the menu bar layer.
  menuTabs->addMenu(new FileMenu(menuTabs, library.get()));

  // Some text editors for testing.
  QTableView *libraryTable = new QTableView(centralTabs);
  libraryTable->setModel(library.get());
  libraryTable->setAlternatingRowColors(true);
  libraryTable->setCornerButtonEnabled(false);
  libraryTable->setSortingEnabled(true);
  libraryTable->setWordWrap(false);
  libraryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
  libraryTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  libraryTable->show();

  centralTabs->addTab(libraryTable, "Library");

  // Throwing some data into a tree view.
  QStandardItemModel *treeModel = new QStandardItemModel(&window);
  QStandardItem *parentItem = treeModel->invisibleRootItem();
  
  QStandardItem *libraryItem = new QStandardItem(QString("Library"));
  parentItem->appendRow(libraryItem);

  libraryItem->appendRow(new QStandardItem(QString("Artists")));
  libraryItem->appendRow(new QStandardItem(QString("Albums")));
  libraryItem->appendRow(new QStandardItem(QString("Songs")));

  QStandardItem *playlists = new QStandardItem(QString("Playlists"));
  parentItem->appendRow(playlists);

  MakeTreeDockables(&window, Qt::LeftDockWidgetArea, treeModel);

  window.show();

  app.exec();

  return 0;
}