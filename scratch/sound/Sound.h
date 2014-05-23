#ifndef __CF_SOUND__
#define __CF_SOUND__

namespace cf {

struct SoundImpl;
struct Sound {
  SoundImpl* impl;
  struct SoundData {
    short* outputBuffer;
    short* inputBuffer;
    short n;
  } soundData;
  void init();
  virtual void onSound(SoundData& io);
};

}
#endif
