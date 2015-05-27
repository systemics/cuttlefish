#include "Cuttlebone/Cuttlebone.hpp"
#include "ctl_mainloop.h"
#include "gfx/gfx_matrix.h"
#include "state.hpp"

struct MyApp : ctl::MainLoop {
  cuttlebone::Maker<State> maker;
  State* state;
  MyApp() : maker("192.168.7.255") {}

  virtual void setup() {
    state = new State;
    memset(state, 0, sizeof(State));
  }

  virtual void onLoop(float dt) {
    state->phase++;
    maker.set(*state);
    usleep(10000);
  }
};

int main() {
  MyApp app;
  app.maker.start();
  app.start();
  return 0;
}

