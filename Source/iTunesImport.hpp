#pragma once

class iTunesHandler
{


  //std::unordered_map<uint64_t, Track> mTracks;
  //std::unordered_map<std::string, Playlist> mPlaylists;
  //std::string mMusicLibrary;

public:
  /*
  iTunesHandler(std::string aFile)
  {
  mTracks = new std::unordered_map<uint64_t, Track>();
  mPlaylists = new std::unordered_map<std::string, Playlist>();

  XmlDocument doc = new XmlDocument();
  doc.Load(aFile);

  XmlElement plist = doc.DocumentElement;
  XmlNodeList nodes = plist.GetElementsByTagName("dict"); // You can also use XPath here

  HandleDictionary(nodes.Item(0));
  }


  void HandleTrack(XmlNode aKey, XmlNode aValue)
  {
  if (aValue.HasChildNodes)
  {
  Track track;

  auto nodes = aValue.ChildNodes;
  auto size = nodes.Count;
  auto halfSize = size / 2;


  for (int i = 0; i < halfSize; ++i)
  {
  auto keyIndex = i * 2;
  auto valueIndex = keyIndex + 1;

  auto key = nodes[i * 2];
  auto value = nodes[i * 2 + 1];

  if (key.InnerText == "Track ID")
  {
  uint64_t.TryParse(value.InnerText, out track.mId);
  break;
  }
  else if (key.InnerText == "Kind")
  {
  track.mKind = value.InnerText;
  break;
  }
  else if (key.InnerText == "Location")
  {
  track.mLocation = QUrl::fromPercentEncoding(value.InnerText).toStdString();
  track.mFileType = Path.GetExtension(track.mLocation);
  break;
  }
  else if (key.InnerText == "Name")
  {
  track.mName = value.InnerText;
  break;
  }
  }

  mTracks.emplace(track.mId, track);
  }

  }


  void HandleTracks(XmlNode aNode)
  {
  if (aNode.HasChildNodes)
  {
  auto nodes = aNode.ChildNodes;
  auto size = nodes.Count;
  auto halfSize = size / 2;


  for (int i = 0; i < halfSize; ++i)
  {
  auto keyIndex = i * 2;
  auto valueIndex = keyIndex + 1;

  auto key = nodes[i * 2];
  auto value = nodes[i * 2 + 1];

  HandleTrack(key, value);
  }
  }
  }


  void HandlePlaylist(XmlNode aNode)
  {
  if (aNode.HasChildNodes)
  {
  Playlist playlist;

  playlist.mName = "";

  auto nodes = aNode.ChildNodes;
  auto size = nodes.Count;
  auto halfSize = size / 2;


  for (int i = 0; i < halfSize; ++i)
  {
  auto keyIndex = i * 2;
  auto valueIndex = keyIndex + 1;

  auto key = nodes[i * 2];
  auto value = nodes[i * 2 + 1];

  if (key.InnerText == "Name")
  {
  playlist.mName = value.InnerText;
  }
  else if (key.InnerText == "Visible")
  {
  if (value.Name == "false")
  {
  return;
  }
  }
  else if (key.InnerText == "Playlist Items")
  {
  auto playlistItems = value.ChildNodes;
  auto playlistSize = value.ChildNodes.Count;
  for (int j = 0; j < playlistSize; ++j)
  {
  auto itemId = playlistItems[j].ChildNodes[1];
  uint64_t trackId;

  if (true == uint64_t.TryParse(itemId.InnerText, out trackId))
  {
  playlist.mTracks.emplace_back(trackId);
  }
  }
  }
  }

  while (mPlaylists.count(playlist.mName))
  {
  playlist.mName += "_DuplicatePlaylistName";
  }

  mPlaylists.Add(playlist.mName, playlist);
  }
  }

  void HandlePlaylists(XmlNode aNode)
  {
  if (aNode.HasChildNodes)
  {
  auto nodes = aNode.ChildNodes;
  auto size = nodes.Count;
  //auto halfSize = size / 2;


  for (int i = 0; i < size; ++i)
  {
  auto value = nodes[i];
  HandlePlaylist(value);
  }
  }
  }

  void HandleTopLevel(XmlNode aKey, XmlNode aValue)
  {
  if (aKey.InnerText == "Tracks")
  {
  HandleTracks(aValue);
  }
  else if (aKey.InnerText == "Playlists")
  {
  HandlePlaylists(aValue);
  }
  else if (aKey.InnerText == "Music Folder")
  {
  mMusicLibrary = aValue.InnerText;
  }
  }

  void HandleDictionary(XmlNode node)
  {
  if (node.HasChildNodes)
  {
  auto nodes = node.ChildNodes;
  auto size = nodes.Count;
  auto halfSize = size / 2;


  for (int i = 0; i < halfSize; ++i)
  {
  auto keyIndex = i * 2;
  auto valueIndex = keyIndex + 1;

  auto key = nodes[i * 2];
  auto value = nodes[i * 2 + 1];

  HandleTopLevel(key, value);
  }
  }
  }


  */
};