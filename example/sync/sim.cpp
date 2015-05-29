#include "ctl_sim.h"
#include "ctl_touch.h"
#include "ctl_grid.h"
#include "gfx/gfx_matrix.h"
#include "Cuttlebone/Cuttlebone.hpp"
#include "state.hpp"

using namespace gfx; // Vec2f

struct MyApp : SimApp<State>, ctl::Touch {

  ctl::Grid* grid;

  virtual void onSetup() {
    Touch::setup("/dev/input/event2");
    grid = new ctl::Grid(16, 9);
  }

  cuttlebone::Stats stats;
  double t;
  virtual void onSimulate(double dt) {
    //stats(dt);
    t += dt;


    int N = 5;
    Vec2f touch[N];
    //while (pollTouches()) {} // consider greed!?
    pollTouches();
    int k = 0;
    for (auto& e : touchPoint)
      if (e.id != 0) {
        touch[k] = Vec2f(CLAMP(0.5f + 0.5f * (e.x / 3200.0f), 0.01, 0.99), CLAMP(0.5f + 0.5f * (e.y / -2600.0f), 0.01, 0.99));
        k++;
        if (k >= N) break;
      }
    if (k) {
      float f = grid->read(touch[0]);
      //LOG("%f", f);
      grid->add(touch[0], 0.1);
    }

    if (t > 0.1) {
      t = t - 0.1;
      printf("\033[2J\033[H");
      grid->print();
      fflush(stdout);
    }

    //state->phase++;
    //LOG("phase:%d time:%lf", state->phase, t);
    //LOG("phase:%d", state->phase);
    //usleep(16667);
    usleep(1667);
  }
};

int main() {
  MyApp app;
  //cuttlebone::Timestamp<> t;
  //double time, last;
  //app.onSetup();
  //while (true) {
  //  time = t.stamp();
  //  app.onSimulate(time - last);
  //  last = time;
  //}
  app.start(true); // use internal thread
}
