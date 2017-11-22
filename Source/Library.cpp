#include <vlc/vlc.h>

#include <qprogressdialog.h>
#include <QTableView>


#include "Library.hpp"

#include <chrono>
#include <thread>

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

  ParseFiles(files);
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

  if (aIndex.column() >= 5 || 0 > aIndex.column())
  {
    return;
  }

  auto &track = mTracks[aIndex.row()];

  mMusicPlayer->SwitchSong(track->mLocation);
  mMusicPlayer->Play();
}

void Library::ScanLibrary(fs::path aPath)
{
  std::set<fs::path> changeToMp3{ ".mp3~2", ".MP3", ".Mp3" };
  std::set<fs::path> move{ ".mp3", ".flac" };
  fs::path m4a{ ".m4a" };
  fs::path mp3{ ".mp3" };

  std::vector<fs::path> files;

  fs::path logFile{ "Library.txt" };
    
  std::ofstream libraryFile;
  libraryFile.open(logFile, std::ios::out | std::ios::trunc | std::ios::binary);

  fs::path path;
  fs::path toPath;
  fs::path extension;

  for (auto& it : fs::recursive_directory_iterator(aPath))
  {
    path = it;

    if (fs::is_directory(path))
    {
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


    libraryFile << path.u8string() << '\n';
    files.emplace_back(path);
  }

  ParseFiles(files);

  libraryFile.close();
}


//typedef void (*libvlc_log_cb)(void *data, int level, const libvlc_log_t *ctx,
//                              const char *fmt, va_list args);

//LIBVLC_API void libvlc_log_set( libvlc_instance_t *,
//                                libvlc_log_cb cb, void *data );

void Library::ParseFiles(std::vector<fs::path> &aFiles)
{
  //auto args = "-vvv";
  //libvlc_instance_t *instance = libvlc_new(1, &args);
  mInstance = libvlc_new(0, NULL);

  std::string u8Path;
  std::string u8Extension;

  QProgressDialog progress("Parsing folder", "cancel", 0, 100, mMainWindow);
  progress.setWindowModality(Qt::WindowModal);

  float percentageComplete = 0.0;

  size_t i{ 0 };
  float size{ static_cast<float>(aFiles.size()) };

  for (auto &file : aFiles)
  {
    if (progress.wasCanceled())
    {
      return;
    }

    u8Extension = file.extension().u8string();
    u8Path = file.generic_u8string();

    std::string path{ "file:///" };
    path += u8Path;

    mTracks.emplace_back(std::make_unique<Track>("",
                                                 "",          // Should be codec.
                                                 u8Extension,
                                                 u8Path));

    this->dataChanged(this->index(mTracks.size() - 1, 0), this->index(mTracks.size() - 1, 4));

    auto err = libvlc_errmsg();

    if (nullptr != err)
    {
      printf("Media path error: %s, %s\n", u8Path.c_str(), libvlc_errmsg());
    }

    ++i;
    percentageComplete = i / size;

    if (progress.value() < percentageComplete && percentageComplete != 100)
    {
      progress.setValue(static_cast<int>(percentageComplete));
    }
  }

  progress.setValue(100);
  //dataChanged(index, index)
}


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

  if (index.column() >= 5 || 0 > index.column())
  {
    return QVariant();
  }

  auto &track = mTracks[index.row()];

  auto media = track->mMedia;

  if (nullptr == media)
  {
    media = libvlc_media_new_location(mInstance, track->mLocation.c_str());

    track->mMedia = media;

    if (nullptr == media)
    {
      printf("Media path error: %s, %s\n", track->mLocation.c_str(), libvlc_errmsg());
      return QVariant();
    }

    libvlc_clearerr();

    libvlc_media_parse(media);

    auto safeStrView = [](const char *aString)
    {
      if (nullptr == aString)
      {
        return std::string_view{ "", 0 };
      }                                                                      

      return std::string_view{ aString };
    };

    std::string_view title = safeStrView(libvlc_media_get_meta(media, libvlc_meta_t::libvlc_meta_Title));
    track->mName = title;
    //std::string_view artist = safeStrView(libvlc_media_get_meta(media, libvlc_meta_t::libvlc_meta_Artist));
    //std::string_view album = safeStrView(libvlc_media_get_meta(media, libvlc_meta_t::libvlc_meta_Album));


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

    track->mId = id;
  }



  switch (index.column())
  {
    case 0:
    {
      return track->mName.c_str();
    }
    case 1:
    {
      return track->mKind.c_str();
    }
    case 2:
    {
      return track->mFileType.c_str();
    }
    case 3:
    {
      return track->mLocation.c_str();
    }
    case 4:
    {
      return track->mId;
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
      return "Kind";
    }
    case 2:
    {
      return "File Type";
    }
    case 3:
    {
      return "Location";
    }
    case 4:
    {
      return "Id";
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
      track->mName = value.toString().toStdString();
    }
    case 1:
    {
      track->mKind.c_str();
    }
    case 2:
    {
      track->mFileType.c_str();
    }
    case 3:
    {
      track->mLocation.c_str();
    }
    case 4:
    {
      track->mId;
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
