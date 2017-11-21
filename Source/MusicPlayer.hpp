#pragma once

#include <vlc/vlc.h>

#include <memory>
#include <mutex>
#include <thread>
#include <string_view>

class MusicPlayer
{
public:
  MusicPlayer();
  ~MusicPlayer();

  void Play();
  void Pause();
  void Stop();
  void SwitchSong(std::string_view aSong);

  class Action
  {
  public:
    virtual void Invoke() = 0;
  };


private:
  static void MusicThread(MusicPlayer *aPlayer);

  std::unique_ptr<std::thread> mThread;

  libvlc_instance_t *mInstance;
  libvlc_media_player_t *mPlayer;
  std::mutex mActionsMutex;
  std::vector<std::unique_ptr<Action>> mActions;

  bool mClosing;
};