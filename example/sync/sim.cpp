#include "ctl_sim.h"
#include "Cuttlebone/Cuttlebone.hpp"
#include "state.hpp"

struct MyApp : SimApp<State> {
  virtual void setup() { }

  virtual void onSimulate(double dt) {
    static cuttlebone::Stats stats;
    stats(dt);
    static double t;
    t += dt;
    state->phase++;
    //LOG("phase:%d time:%lf", state->phase, t);
    //LOG("phase:%d", state->phase);
    usleep(16667);
  }
};

int main() { MyApp().start(true); } // true starts a main loop
