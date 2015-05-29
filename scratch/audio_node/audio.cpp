#include "ctl_audio.h"
#include "Cuttlebone/Cuttlebone.hpp"
#include "Gamma/SamplePlayer.h"
//#include "Gamma/Sync.h"

#include "../../example/swarm/state.hpp"

struct MyApp : Audio {

  cuttlebone::Taker<State> taker;
  State* state;

  gam::SamplePlayer<float, gam::ipl::Linear, gam::tap::Wrap> play[MAX_TOUCHES];
  float gain[MAX_TOUCHES];

  MyApp() : Audio(1024) {
    state = new State();
    memset(state, 0, sizeof(State));

    for (int i = 0; i < MAX_TOUCHES; i++) {
      char fileName[] = "/root/sound .wav";
      fileName[11] = '0' + i;

      if (!play[i].load(fileName)) {
        LOG("ERROR: failed to load %s", fileName);
      }
    }
    for (auto& e : gain) e = 0.1;
    gam::Sync::master().spu(sampleRate);
  }

  virtual void process(float* output) {
    static cuttlebone::Stats fps;
    fps((float)blockSize / sampleRate);

    taker.get(*state);
    for (int i = 0; i < MAX_TOUCHES; i++) {
      gain[i] = state->touch[i].y;
      play[i].rate(state->touch[i].y);
    }

    //   Vec3f& v = renderState->position[renderState->index[i]];
    // for (int i = 0; i < MAX_TOUCHES; i++) {
    //   static float xLast;
    //   Vec3f& v = renderState->position[renderState->index[i]];
    //   float rate = v.x - xLast;
    //   rate /= 30;
    //   gain[i] = fabs(v.z) / 5;
    //   gain[i] = sqrt(gain[i]);
    //   play[i].rate(rate);
    //   //LOG("%f %f", v.z, v.x - xLast);
    //   xLast = v.x;
    // }

    for (unsigned i = 0; i < blockSize; ++i) {
      float s = 0;
      for (int k = 0; k < MAX_TOUCHES; k++) s += play[k]() * gain[k];
      *output++ = s;
      *output++ = s;
    }
  }
};

int main() {
  MyApp app;
  app.taker.start();
  app.start();
}
