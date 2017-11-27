#pragma once

#include <QToolBar>
#include <QWidget>

#include <vlc/vlc.h>

#include <memory>
#include <mutex>
#include <thread>
#include <string_view>

class MusicPlayer : public QWidget
{
public:
  MusicPlayer(QToolBar *aParent);
  ~MusicPlayer();

  void Play();
  void Pause();
  void Stop();
  void AddToPlaylist(std::string_view aSong);

  // Clears current playlist.
  void SwitchSong(std::string_view aSong);

  class Action
  {
  public:
    virtual void Invoke() = 0;
  };

private:

  static void MediaEndReached(const struct libvlc_event_t *aEvent, void *aSelf);

  static void MusicThread(MusicPlayer *aPlayer);

  std::unique_ptr<std::thread> mThread;

  libvlc_instance_t *mInstance;
  libvlc_media_player_t *mPlayer;
  std::mutex mActionsMutex;
  std::vector<std::unique_ptr<Action>> mActions;

  std::vector<std::unique_ptr<Action>> mPlaylistActions;

  bool mClosing;
};