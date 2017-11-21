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

#include <QListView>


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

int main(int argc, char *argv[])
{
  QCoreApplication::addLibraryPath("./");
  QCoreApplication::addLibraryPath("C:/Qt/5.8/msvc2015_64/bin");

  // Unsure if required.
  QApplication app(argc, argv);

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
  
  // You can move the tabs around.
  centralTabs->setMovable(true);
  // Tabs get close buttons.
  centralTabs->setTabsClosable(true);
  // When there are two many tabs, this will make buttons to scroll 
  // through them.
  centralTabs->setUsesScrollButtons(true);

  // Add tab options for the menu bar layer.
  QMenu* fileMenu = new QMenu("File", menuTabs);
  fileMenu->addMenu(new QMenu("Open"));

  menuTabs->addMenu(fileMenu);

  // Some text editors for testing.
  auto listView = new QListView(centralTabs);

  centralTabs->addTab(listView, "Library");

  // Throwing some data into a tree view.
  QStandardItemModel *treeModel = new QStandardItemModel(&window);
  QStandardItem *parentItem = treeModel->invisibleRootItem();
  for (int i = 0; i < 4; ++i)
  {
    QStandardItem *item = new QStandardItem(QString("item %0").arg(i));
    parentItem->appendRow(item);
    parentItem = item;
  }

  MakeTreeDockables(&window, Qt::RightDockWidgetArea, treeModel);

  // Shows some files off in a tree view.
  //QFileSystemModel *fileModel = new QFileSystemModel;
  //fileModel->setRootPath(QDir::currentPath());
  //
  //MakeTreeDockables(window, Qt::BottomDockWidgetArea, fileModel);

  window.show();

  return app.exec();
}