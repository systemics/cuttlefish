#include "ctl_sim.h"
#include "ctl_touch.h"
#include "ctl_grid.h"
#include "gfx/gfx_matrix.h"
#include "Cuttlebone/Cuttlebone.hpp"
#include "state.hpp"

using namespace gfx; // Vec2f

struct MyApp : SimApp<State>, ctl::Touch {

  Grid* grid;

  virtual void onSetup() {
    Touch::setup("/dev/input/event2");

    grid = new Grid(16, 9);
  }

  virtual void onSimulate(double dt) {
    static cuttlebone::Stats stats;
    stats(dt);
    static double t;
    t += dt;


    int N = 5;
    Vec2f touch[N];
    pollTouches();
    int k = 0;
    for (auto& e : touchPoint)
      if (e.id != 0) {
        touch[k] = Vec2f(CLAMP(e.x / 3200.0f, -1.0, 1.0), CLAMP(e.y / -2600.0f, -1.0, 1.0));
        k++;
        if (k >= N) break;
      }
    if (k) {
      float f = grid->read(touch[0]);
      LOG("%f", f);
      grid->add(touch[0], 0.1);
    }

    if (t > 1.0) {
      t = t - 1.0;
      grid->print();
    }

    state->phase++;
    //LOG("phase:%d time:%lf", state->phase, t);
    //LOG("phase:%d", state->phase);
    usleep(16667);
  }
};

int main() { MyApp().start(true); } // true starts a main loop
