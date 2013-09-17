#ifndef __CF_SOUND__
#define __CF_SOUND__
namespace cf {

struct Sound {
  struct SoundData {
  };
  void init();
  virtual void onSound(SoundData& io);
};

}
#endif
