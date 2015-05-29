#include "ctl_audio.h"
#include "Cuttlebone/Cuttlebone.hpp"
#include "Gamma/SamplePlayer.h"
//#include "Gamma/Sync.h"

#include "../../example/swarm/state.hpp"

struct MyApp : Audio {
  cuttlebone::Taker<State> taker;
  State* state;

  unsigned offset;

  gfx::Vec2f last[MAX_TOUCHES];

  gam::SamplePlayer<float, gam::ipl::Linear, gam::tap::Wrap> play[MAX_TOUCHES];
  float gain[MAX_TOUCHES];

  MyApp() : Audio(1024) {
    state = new State();
    memset(state, 0, sizeof(State));
    memset(&last[0], 0, sizeof(gfx::Vec2f) * MAX_TOUCHES);

    offset = 0;

    for (int i = 0; i < MAX_TOUCHES; i++) {
      char fileName[] = "/root/sound .wav";
      fileName[11] = '0' + i;

      if (!play[i].load(fileName)) {
        LOG("ERROR: failed to load %s", fileName);
      }
      LOG("loaded: %s", fileName);
    }
    for (auto& e : gain) e = 0.1;
    gam::Sync::master().spu(sampleRate);
  }

  virtual void process(float* output) {
    static cuttlebone::Stats fps;
    fps((float)blockSize / sampleRate);

    taker.get(*state);

    static int lastnumtouches = 0;

    if (state->numtouches == 0 && lastnumtouches != 0) {
      // change finger to wave mapping
      offset++; 
LOG("offset:%u", offset);
    }
    lastnumtouches = state->numtouches;

    if (state->numtouches == 0)
      for (int i = 0; i < MAX_TOUCHES; i++) gain[i] *= 0.93;

    for (int i = 0; i < state->numtouches; i++) {
      gain[i] += 0.09;
      if (gain[i] > 0.7) gain[i] = 0.7;
      float v = (state->touch[i] - last[i]).len();
      play[i].rate(0.4 + v * 2);
    }

    memcpy(&last[0], state->touch, sizeof(gfx::Vec2f) * MAX_TOUCHES);

    for (unsigned i = 0; i < blockSize; ++i) {
      float s = 0;
      for (int k = 0; k < MAX_TOUCHES; k++) s += play[(k + offset) % MAX_TOUCHES]() * gain[k] / 3.7;
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
