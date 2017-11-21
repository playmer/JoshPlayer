#include "MusicPlayer.hpp"


template<typename tTo, typename tFrom>
static std::unique_ptr<tTo> static_unique_pointer_cast(std::unique_ptr<tFrom> &&aValue)
{
  return std::unique_ptr<tTo>{static_cast<tTo*>(aValue.release())};
}

MusicPlayer::MusicPlayer()
  : mClosing(false)
{
  mThread = std::make_unique<std::thread>(MusicThread, this);

  /* Load the VLC engine */
  mInstance = libvlc_new(0, nullptr);

  mPlayer = libvlc_media_player_new(mInstance);
}

MusicPlayer::~MusicPlayer()
{
  /* Stop playing */
  libvlc_media_player_stop(mPlayer);

  /* Free the media_player */
  libvlc_media_player_release(mPlayer);

  libvlc_release(mInstance);

  mClosing = true;

  mThread->join();
}

namespace Actions
{
  class PlaySong : public MusicPlayer::Action
  {
  public:
    PlaySong(libvlc_instance_t *aInstance,
             libvlc_media_player_t *aPlayer)
      : mInstance(aInstance)
      , mPlayer(aPlayer)
    {

    }

    void Invoke() override
    {
      auto result = libvlc_media_player_play(mPlayer);

      if (result)
      {
        printf("%s\n", libvlc_errmsg());
        return;
      }
    }

    libvlc_instance_t *mInstance;
    libvlc_media_player_t *mPlayer;
  };

  class PauseSong : public MusicPlayer::Action
  {
  public:
    PauseSong(libvlc_instance_t *aInstance,
              libvlc_media_player_t *aPlayer)
      : mInstance(aInstance)
      , mPlayer(aPlayer)
    {

    }

    void Invoke() override
    {
      libvlc_media_player_pause(mPlayer);
    }

    libvlc_instance_t *mInstance;
    libvlc_media_player_t *mPlayer;
  };


  class StopSong : public MusicPlayer::Action
  {
  public:
    StopSong(libvlc_instance_t *aInstance,
             libvlc_media_player_t *aPlayer)
      : mInstance(aInstance)
      , mPlayer(aPlayer)
    {

    }

    void Invoke() override
    {
      libvlc_media_player_stop(mPlayer);
    }

    libvlc_instance_t *mInstance;
    libvlc_media_player_t *mPlayer;
  };

  class SwitchSong : public MusicPlayer::Action
  {
  public:
    SwitchSong(libvlc_instance_t *aInstance,
               libvlc_media_player_t *aPlayer,
               std::string_view aLocation)
      : mInstance(aInstance)
      , mPlayer(aPlayer)
      , mLocation(aLocation)
    {

    }

    void Invoke() override
    {
      libvlc_media_t *media = libvlc_media_new_location(mInstance, mLocation.c_str());

      if (nullptr == media)
      {
        printf("Switch Error: %s, %s\n", mLocation.c_str(), libvlc_errmsg());
        return;
      }

      libvlc_media_player_set_media(mPlayer, media);
      libvlc_media_release(media);
    }

    libvlc_instance_t *mInstance;
    libvlc_media_player_t *mPlayer;
    std::string mLocation;
  };

}

void MusicPlayer::Play()
{
  auto playSong = std::make_unique<Actions::PlaySong>(mInstance, mPlayer);
  std::lock_guard<std::mutex> guard{ mActionsMutex };
  mActions.emplace_back(std::move(playSong));
}

void MusicPlayer::Pause()
{
  auto pauseSong = std::make_unique<Actions::PauseSong>(mInstance, mPlayer);
  std::lock_guard<std::mutex> guard{ mActionsMutex };
  mActions.emplace_back(static_unique_pointer_cast<MusicPlayer::Action>(std::move(pauseSong)));
}

void MusicPlayer::Stop()
{
  auto stopSong = std::make_unique<Actions::StopSong>(mInstance, mPlayer);
  std::lock_guard<std::mutex> guard{ mActionsMutex };
  mActions.emplace_back(static_unique_pointer_cast<MusicPlayer::Action>(std::move(stopSong)));
}

void MusicPlayer::SwitchSong(std::string_view aSong)
{
  std::string path{ "file:///" };
  path += aSong;

  auto switchSong = std::make_unique<Actions::SwitchSong>(mInstance, mPlayer, aSong);
  std::lock_guard<std::mutex> guard{ mActionsMutex };
  mActions.emplace_back(static_unique_pointer_cast<MusicPlayer::Action>(std::move(switchSong)));
}

void MusicPlayer::MusicThread(MusicPlayer *aPlayer)
{
  using namespace std::literals::chrono_literals;

  while (false == aPlayer->mClosing)
  {
    std::this_thread::sleep_for(10ms);
    std::lock_guard<std::mutex> guard{ aPlayer->mActionsMutex };

    auto &actions = aPlayer->mActions;

    for (auto &action : actions)
    {
      action->Invoke();
    }

    actions.clear();
  }
}