#include "ctl_audio.h"
#include "Cuttlebone/Cuttlebone.hpp"
#include "Gamma/SamplePlayer.h"
//#include "Gamma/Sync.h"

#include "../../example/swarm/state.hpp"

struct MyApp : Audio {

  cuttlebone::Taker<State> taker;
  State* state;

  gfx::Vec2f last[MAX_TOUCHES];

  gam::SamplePlayer<float, gam::ipl::Linear, gam::tap::Wrap> play[MAX_TOUCHES];
  float gain[MAX_TOUCHES];

  MyApp() : Audio(1024) {
    state = new State();
    memset(state, 0, sizeof(State));
    memset(&last[0], 0, sizeof(gfx::Vec2f) * MAX_TOUCHES);

    for (int i = 0; i < MAX_TOUCHES; i++) {
      char fileName[] = "/root/sound .wav";
      fileName[11] = '0' + i;

      if (!play[i].load(fileName)) {
        LOG("ERROR: failed to load %s", fileName);
      }
      LOG("loaded: ", fileName);
    }
    for (auto& e : gain) e = 0.1;
    gam::Sync::master().spu(sampleRate);
  }

  virtual void process(float* output) {
    static cuttlebone::Stats fps;
    fps((float)blockSize / sampleRate);

    taker.get(*state);

    if (state->numtouches == 0) 
      for (int i = 0; i < MAX_TOUCHES; i++)
        gain[i] *= 0.9;

    for (int i = 0; i < state->numtouches; i++) {
      gain[i] += 0.05;
      if (gain[i] > 0.7) gain[i] = 0.7;
      float v = (state->touch[i] - last[i]).len();
      play[i].rate(0.25 + v);
    }

    memcpy(&last[0], state->touch, sizeof(gfx::Vec2f) * MAX_TOUCHES);

    for (unsigned i = 0; i < blockSize; ++i) {
      float s = 0;
      for (int k = 0; k < MAX_TOUCHES; k++) s += play[k]() * gain[k] / 3.5;
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
