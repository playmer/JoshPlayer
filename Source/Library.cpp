#include <vlc/vlc.h>

#include <qprogressdialog.h>
#include <QTableView>
#include <QMessageBox>

#include <chrono>
#include <thread>
#include <iostream>

#include "Library.hpp"

using namespace std::literals::chrono_literals;

Library::Library(QMainWindow *aMainWindow, QTableView *aView, MusicPlayer *aMusicPlayer)
  : mMainWindow (aMainWindow)
  , mView(aView)
  , mMusicPlayer(aMusicPlayer)
{
  this->connect(mView, &QTableView::doubleClicked, this, &Library::PlayTrack);
}

// Get* will create an Artist if artist doesn't exist.
Artist* Library::GetArtist(std::string_view aArtist)
{
  auto result = FindArtist(aArtist);

  if (nullptr == result)
  {
    std::string value{ aArtist.data(), aArtist.size() };

    auto emplacedValue = std::make_unique<Artist>(aArtist);
    result = emplacedValue.get();

    mArtists.emplace(value, std::move(emplacedValue));
  }

  return result;
}

Album* Library::GetAlbum(std::string_view aAlbum)
{
  auto result = FindAlbum(aAlbum);

  if (nullptr == result)
  {
    std::string value{ aAlbum.data(), aAlbum.size() };

    auto emplacedValue = std::make_unique<Album>(aAlbum);
    result = emplacedValue.get();

    mAlbums.emplace(value, std::move(emplacedValue));
  }

  return result;
}
  
Playlist* Library::GetPlaylist(std::string_view aPlaylist)
{
  auto result = FindPlaylist(aPlaylist);

  if (nullptr == result)
  {
    std::string value{ aPlaylist.data(), aPlaylist.size() };

    auto emplacedValue = std::make_unique<Playlist>(aPlaylist);
    result = emplacedValue.get();

    mPlaylists.emplace(value, std::move(emplacedValue));
  }

  return result;
}

// Find* will return nullptr if not found.
Artist* Library::FindArtist(std::string_view aArtist)
{
  std::string artist{ aArtist };

  auto it = mArtists.find(artist);

  if (it == mArtists.end())
  {
    return nullptr;
  }

  return it->second.get();
}

Album* Library::FindAlbum(std::string_view aAlbum)
{
  std::string album{ aAlbum };

  auto it = mAlbums.find(album);

  if (it == mAlbums.end())
  {
    return nullptr;
  }

  return it->second.get();
}

Playlist* Library::FindPlaylist(std::string_view aPlaylist)
{
  std::string playlist{ aPlaylist };

  auto it = mPlaylists.find(playlist);

  if (it == mPlaylists.end())
  {
    return nullptr;
  }

  return it->second.get();
}

void Library::LoadLibrary(std::string aPath)
{
  std::vector<fs::path> files;

  std::ifstream libraryFile{ aPath };
  std::string line;

  while (std::getline(libraryFile, line))
  {
    fs::path path{ line };

    files.emplace_back(path);
  }
}

void Library::PlayTrack(const QModelIndex &aIndex)
{
  if (!aIndex.isValid())
  {
    return;
  }

  if (aIndex.row() >= mTracks.size() || aIndex.row() < 0)
  {
    return;
  }

  if (aIndex.column() >= 4 || 0 > aIndex.column())
  {
    return;
  }

  auto &track = mTracks[aIndex.row()];

  mMusicPlayer->SwitchSong(track->mLocation);

  auto ptrTitle = libvlc_media_get_meta(track->mMedia, libvlc_meta_t::libvlc_meta_Title);
  printf("ptrTitle: %s\n", ptrTitle);
  auto ptrArtist = libvlc_media_get_meta(track->mMedia, libvlc_meta_t::libvlc_meta_Artist);
  printf("ptrArtist: %s\n", ptrArtist);
  auto ptrGenre = libvlc_media_get_meta(track->mMedia, libvlc_meta_t::libvlc_meta_Genre);
  printf("ptrGenre: %s\n", ptrGenre);
  auto ptrCopyright = libvlc_media_get_meta(track->mMedia, libvlc_meta_t::libvlc_meta_Copyright);
  printf("ptrCopyright: %s\n", ptrCopyright);
  auto ptrAlbum = libvlc_media_get_meta(track->mMedia, libvlc_meta_t::libvlc_meta_Album);
  printf("ptrAlbum: %s\n", ptrAlbum);
  auto ptrTrackNumber = libvlc_media_get_meta(track->mMedia, libvlc_meta_t::libvlc_meta_TrackNumber);
  printf("ptrTrackNumber: %s\n", ptrTrackNumber);
  auto ptrDescription = libvlc_media_get_meta(track->mMedia, libvlc_meta_t::libvlc_meta_Description);
  printf("ptrDescription: %s\n", ptrDescription);
  auto ptrRating = libvlc_media_get_meta(track->mMedia, libvlc_meta_t::libvlc_meta_Rating);
  printf("ptrRating: %s\n", ptrRating);
  auto ptrDate = libvlc_media_get_meta(track->mMedia, libvlc_meta_t::libvlc_meta_Date);
  printf("ptrDate: %s\n", ptrDate);
  auto ptrSetting = libvlc_media_get_meta(track->mMedia, libvlc_meta_t::libvlc_meta_Setting);
  printf("ptrSetting: %s\n", ptrSetting);
  auto ptrURL = libvlc_media_get_meta(track->mMedia, libvlc_meta_t::libvlc_meta_URL);
  printf("ptrURL: %s\n", ptrURL);
  auto ptrLanguage = libvlc_media_get_meta(track->mMedia, libvlc_meta_t::libvlc_meta_Language);
  printf("ptrLanguage: %s\n", ptrLanguage);
  auto ptrNowPlaying = libvlc_media_get_meta(track->mMedia, libvlc_meta_t::libvlc_meta_NowPlaying);
  printf("ptrNowPlaying: %s\n", ptrNowPlaying);
  auto ptrPublisher = libvlc_media_get_meta(track->mMedia, libvlc_meta_t::libvlc_meta_Publisher);
  printf("ptrPublisher: %s\n", ptrPublisher);
  auto ptrEncodedBy = libvlc_media_get_meta(track->mMedia, libvlc_meta_t::libvlc_meta_EncodedBy);
  printf("ptrEncodedBy: %s\n", ptrEncodedBy);
  auto ptrArtworkURL = libvlc_media_get_meta(track->mMedia, libvlc_meta_t::libvlc_meta_ArtworkURL);
  printf("ptrArtworkURL: %s\n", ptrArtworkURL);
  auto ptrTrackID = libvlc_media_get_meta(track->mMedia, libvlc_meta_t::libvlc_meta_TrackID);
  printf("ptrTrackID: %s\n", ptrTrackID);
  auto ptrTrackTotal = libvlc_media_get_meta(track->mMedia, libvlc_meta_t::libvlc_meta_TrackTotal);
  printf("ptrTrackTotal: %s\n", ptrTrackTotal);
  auto ptrDirector = libvlc_media_get_meta(track->mMedia, libvlc_meta_t::libvlc_meta_Director);
  printf("ptrDirector: %s\n", ptrDirector);
  auto ptrSeason = libvlc_media_get_meta(track->mMedia, libvlc_meta_t::libvlc_meta_Season);
  printf("ptrSeason: %s\n", ptrSeason);
  auto ptrEpisode = libvlc_media_get_meta(track->mMedia, libvlc_meta_t::libvlc_meta_Episode);
  printf("ptrEpisode: %s\n", ptrEpisode);
  auto ptrShowName = libvlc_media_get_meta(track->mMedia, libvlc_meta_t::libvlc_meta_ShowName);
  printf("ptrShowName: %s\n", ptrShowName);
  auto ptrActors = libvlc_media_get_meta(track->mMedia, libvlc_meta_t::libvlc_meta_Actors);
  printf("ptrActors: %s\n", ptrActors);
}

void Library::ScanLibrary(fs::path aPath)
{
  std::vector<fs::path> files;

  {
    std::set<fs::path> acceptableExtensions{ ".mp3~2", ".MP3", ".Mp3", ".mp3", ".flac", ".m4a", ".wav", ".mov", ".m4v" };

    std::set<fs::path> changeToMp3{ ".mp3~2", ".MP3", ".Mp3" };
    std::set<fs::path> move{ ".mp3", ".flac" };
    fs::path m4a{ ".m4a" };
    fs::path mp3{ ".mp3" };


    fs::path path;
    fs::path toPath;
    fs::path extension;

    std::set<fs::path> unexpectedExtensions;

    auto messageBox = new QMessageBox(QMessageBox::Information, 
                                      "Scanning Folder",
                                      "This may take some time...",
                                      QMessageBox::NoButton, 
                                      mMainWindow);

    for (auto& it : fs::recursive_directory_iterator(aPath))
    {
      path = it;
      extension = path.extension();

      if (fs::is_directory(path) ||
          extension.empty())
      {
        continue;
      }

      if (acceptableExtensions.find(extension) == acceptableExtensions.end())
      {
        unexpectedExtensions.emplace(extension);
        continue;
      }
    
      // TODO (Josh): We should ask if they want to do normalization
      //toPath = path;
      //
      //
      //extension = path.extension();
      //
      //if (extension == m4a)
      //{
      //
      //}
      //
      //if (changeToMp3.find(extension) != changeToMp3.end())
      //{
      //  toPath.replace_extension(mp3);
      //}

      files.emplace_back(path);
    }

    std::cout << "Found some unexpected extension types in the folder scanned, "
                 "they were not added to the library: \n";
    for (auto &unexpectedExtension : unexpectedExtensions)
    {
      std::cout << unexpectedExtension << '\n';
    }
  }







  fs::path logFile{ "Library.txt" };

  std::ofstream libraryFile;
  libraryFile.open(logFile, std::ios::out | std::ios::trunc | std::ios::binary);





  {
    struct Thread
    {
      Thread(std::vector<fs::path> &&aFiles)
        : mFiles (std::move(aFiles))
        , mThread(ParseFiles, this)
      {

      }

      static void ParseFiles(Thread *aSelf)
      {
        auto instance = libvlc_new(0, nullptr);
        aSelf->mTracks.reserve(aSelf->mFiles.size());

        for (auto &file : aSelf->mFiles)
        {
          std::string path{ "file:///" };
          path += file.u8string();
          auto extension{ file.extension().u8string() };
          auto track = std::make_unique<Track>(extension, path);
          track->mMedia = libvlc_media_new_location(instance, path.c_str());
          libvlc_media_parse_async(track->mMedia);

          aSelf->mTracks.emplace_back(std::move(track));
        }

        for (auto &track : aSelf->mTracks)
        {
          auto media = track->mMedia;
          track->mMedia = nullptr;

          while (false == libvlc_media_is_parsed(media));

          auto safeStrView = [](const char *aString)
          {
            if (nullptr == aString)
            {
              return std::string_view{ "", 0 };
            }

            return std::string_view{ aString };
          };

          std::string_view title = safeStrView(libvlc_media_get_meta(media, libvlc_meta_t::libvlc_meta_Title));
          track->mTitle = title;
          std::string_view artist = safeStrView(libvlc_media_get_meta(media, libvlc_meta_t::libvlc_meta_Artist));
          track->mArtistName = artist;
          std::string_view album = safeStrView(libvlc_media_get_meta(media, libvlc_meta_t::libvlc_meta_Album));
          track->mAlbumName = album;

          std::string trackId = safeStrView(libvlc_media_get_meta(media, libvlc_meta_t::libvlc_meta_TrackNumber)).data();

          long long id = 0;

          if (0 != trackId.size())
          {
            id = std::stoll(trackId);
          }

          libvlc_media_release(media);

          track->mTrackNumber = id;

          track->mParsed = true;
        }

        libvlc_release(instance);
      }

      std::vector<std::unique_ptr<Track>> mTracks;
      std::vector<fs::path> mFiles;
      std::thread mThread;
    };

    auto numThreads = std::thread::hardware_concurrency();

    std::vector<std::unique_ptr<Thread>> threads;

    auto totalFiles = files.size();

    if (numThreads > totalFiles)
    {
      numThreads = totalFiles;
    }

    for (size_t i = 0; i < numThreads; ++i)
    {
      std::vector<fs::path> filesForThread;

      auto filesPerThread = totalFiles / numThreads;

      // Unsure if this check is actually needed, but I'm too tired to think of the
      // integer math around the filesPerThread number and the edge cases therein.
      if (filesPerThread > files.size())
      {
        filesPerThread = files.size();
      }

      if (i < numThreads - 1)
      {
        auto begin = files.end() - filesPerThread;
        auto end = files.end();


        filesForThread.insert(filesForThread.begin(), 
                              std::make_move_iterator(begin),
                              std::make_move_iterator(end));

        files.erase(begin, end);
      }
      else
      {
        filesForThread = std::move(files);
      }

      threads.emplace_back(std::make_unique<Thread>(std::move(filesForThread)));
    }


    // Join the threads.
    for (auto &thread : threads)
    {
      thread->mThread.join();
      
      for (auto &track : thread->mTracks)
      {
        libraryFile << track->mLocation << ';' 
                    << track->mTitle << ';'
                    << track->mArtistName << ';'
                    << track->mAlbumName << ';'
                    << track->mTrackNumber << '\n';
      }
    }
  }



















  libraryFile.close();
}


//typedef void (*libvlc_log_cb)(void *data, int level, const libvlc_log_t *ctx,
//                              const char *fmt, va_list args);

//LIBVLC_API void libvlc_log_set( libvlc_instance_t *,
//                                libvlc_log_cb cb, void *data );

//void Library::ParseFiles(std::vector<fs::path> &aFiles)
//{
//  //auto args = "-vvv";
//  //libvlc_instance_t *instance = libvlc_new(1, &args);
//  mInstance = libvlc_new(0, NULL);
//
//  std::string u8Path;
//  std::string u8Extension;
//
//  QProgressDialog progress("Parsing folder", "cancel", 0, 100, mMainWindow);
//  progress.setWindowModality(Qt::WindowModal);
//
//  float percentageComplete = 0.0;
//
//  size_t i{ 0 };
//  float size{ static_cast<float>(aFiles.size()) };
//
//  for (auto &file : aFiles)
//  {
//    if (progress.wasCanceled())
//    {
//      return;
//    }
//
//    u8Extension = file.extension().u8string();
//    u8Path = file.generic_u8string();
//
//    std::string path{ "file:///" };
//    path += u8Path;
//
//    auto &track = mTracks.emplace_back(std::make_unique<Track>(u8Extension,
//                                                               path));
//
//    track->mMedia = libvlc_media_new_location(mInstance, path.c_str());
//
//    libvlc_media_parse_async(track->mMedia);
//
//    this->dataChanged(this->index(mTracks.size() - 1, 0), this->index(mTracks.size() - 1, 4));
//
//    auto err = libvlc_errmsg();
//
//    if (nullptr != err)
//    {
//      printf("Media path error: %s, %s\n", u8Path.c_str(), libvlc_errmsg());
//    }
//
//    ++i;
//    percentageComplete = i / size;
//
//    if (progress.value() < percentageComplete && percentageComplete != 100)
//    {
//      progress.setValue(static_cast<int>(percentageComplete));
//    }
//  }
//
//  progress.setValue(100);
//}


void Library::ParseAlbum(std::string_view aAlbum, std::string_view aArtist, Track *aTrack)
{
  if (0 == aAlbum.size())
  {
    return;
  }

  auto album = GetAlbum(aAlbum);
  album->mTracks.emplace_back(aTrack);

  if (0 == aArtist.size())
  {
    auto artist = GetArtist(aArtist);

    artist->mAlbums.emplace_back(album);
  }
}

void Library::ParseArtist(std::string_view aArtist)
{
  if (0 == aArtist.size())
  {
    return;
  }

  GetArtist(aArtist);
}


QVariant Library::data(const QModelIndex &index, int role) const
{
  //printf("Data %d\n", mThrash++);

  if (role != Qt::DisplayRole)
  {
    return QVariant();
  }

  if (!index.isValid())
  {
    return QVariant();
  }

  if (index.row() >= mTracks.size() || index.row() < 0)
  {
    return QVariant();
  }

  if (index.column() >= 4 || 0 > index.column())
  {
    return QVariant();
  }

  auto &track = mTracks[index.row()];

  auto media = track->mMedia;

  if (false == track->mParsed)
  {
    while (false == libvlc_media_is_parsed(media));

    auto safeStrView = [](const char *aString)
    {
      if (nullptr == aString)
      {
        return std::string_view{ "", 0 };
      }                                                                      

      return std::string_view{ aString };
    };

    std::string_view title = safeStrView(libvlc_media_get_meta(media, libvlc_meta_t::libvlc_meta_Title));
    track->mTitle = title;
    std::string_view artist = safeStrView(libvlc_media_get_meta(media, libvlc_meta_t::libvlc_meta_Artist));
    track->mArtistName = artist;
    std::string_view album = safeStrView(libvlc_media_get_meta(media, libvlc_meta_t::libvlc_meta_Album));
    track->mAlbumName = album;

    //libvlc_media_track_t **elementaryTracks;
    //auto number = libvlc_media_tracks_get(media, &elementaryTracks);
    //
    //for (size_t i = 0; i < number; ++i)
    //{
    //  auto track = elementaryTracks[i];
    //}
    //
    //libvlc_media_tracks_release(elementaryTracks, number);

    std::string trackId = safeStrView(libvlc_media_get_meta(media, libvlc_meta_t::libvlc_meta_TrackNumber)).data();

    long long id = 0;

    if (0 != trackId.size())
    {
      id = std::stoll(trackId);
    }

    track->mTrackNumber = id;
  }

  switch (index.column())
  {
    case 0:
    {
      //QString::fromUtf8()
      return track->mTitle.c_str();
    }
    case 1:
    {
      return track->mArtistName.c_str();
    }
    case 2:
    {
      return track->mAlbumName.c_str();
    }
    case 3:
    {
      return track->mTrackNumber;
    }
  }

  return QVariant();
}

QVariant Library::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (Qt::Orientation::Vertical == orientation)
  {
    return QVariant();
  }

  if (role != Qt::DisplayRole)
  {
    return QVariant();
  }

  if (5 <= section || 0 > section)
  {
    return QVariant();
  }

  switch (section)
  {
    case 0:
    {
      return "Name";
    }
    case 1:
    {
      return "Artist";
    }
    case 2:
    {
      return "Album";
    }
    case 3:
    {
      return "Track Number";
    }
  }

  return QVariant();
}

Qt::ItemFlags Library::flags(const QModelIndex &index) const
{
  if (!index.isValid())
  {
    return Qt::ItemIsEnabled;
  }

  return QAbstractTableModel::flags(index);
}

bool Library::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (role != Qt::EditRole)
  {
    return false;
  }
  
  if (!index.isValid())
  {
    return false;
  }

  if (index.row() >= mTracks.size() || index.row() < 0)
  {
    return false;
  }

  if (index.column() >= 5 || 0 > index.column())
  {
    return false;
  }

  auto &track = mTracks[index.row()];


  switch (index.column())
  {
    case 0:
    {
      track->mTitle = value.toString().toStdString();
    }
    case 1:
    {
      track->mArtistName.c_str();
    }
    case 2:
    {
      track->mAlbumName.c_str();
    }
    case 3:
    {
      track->mTrackNumber;
    }
  }


  emit(dataChanged(index, index));

  return true;
}

bool Library::insertRows(int position, int rows, const QModelIndex &index)
{
  Q_UNUSED(index);
  beginInsertRows(QModelIndex(), position, position + rows - 1);

  for (int row = 0; row < rows; ++row)
  {
    mTracks.emplace(mTracks.begin() + position, std::make_unique<Track>());
  }

  endInsertRows();
  return true;
}

bool Library::removeRows(int position, int rows, const QModelIndex &index)
{
  Q_UNUSED(index);
  beginRemoveRows(QModelIndex(), position, position + rows - 1);

  mTracks.erase(mTracks.begin() + position, mTracks.begin() + position + rows);

  endRemoveRows();
  return true;
}
