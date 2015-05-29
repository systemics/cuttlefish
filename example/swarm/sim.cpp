#include "ctl_sim.h"

#include "form/vsr_knot.h"
#include "form/vsr_twist.h"
#include "util/vsr_stat.h"
#include "form/vsr_group.h"

#include "swarm.h"
#include "state.hpp"
#include "ctl_sim.h"
#include "ctl_touch.h"

// using namespace vsr;
// using namespace vsr::cga;

struct MyApp : SimApp<State>, ctl::Touch {
  Population population;

  virtual void onSetup() {
    Touch::setup("/dev/input/event2");

    population.init<Jelly>();
  }

  double t;
  cuttlebone::Stats stats;
  virtual void onSimulate(double dt) {
    stats(dt);  // XXX comment this out if shit crashes (BUG in LOGGER)
    t += dt;

    int N = 5;
    gfx::Vec2f touch[N];
    // while (pollTouches()) {} // consider greed!?
    pollTouches();
    int k = 0;
    for (auto& e : touchPoint)
      if (e.id != 0) {
        touch[k] =
            gfx::Vec2f(CLAMP(0.5f + 0.5f * (e.x / 3200.0f), 0.01, 0.99),
                       CLAMP(0.5f + 0.5f * (e.y / -2600.0f), 0.01, 0.99));
        population.substrate.grid.add(touch[0], 0.4 * dt);
        k++;
        if (k >= N) break;
      }

    // memcopy food data
    memcpy(
        state->food, population.substrate.grid.data,
        sizeof(float) * NUM_CELLS_WIDTH_SUBSTRATE * NUM_CELLS_HEIGHT_SUBSTRATE);

    population.step(.01);

    for (int i = 0; i < NUM_AGENTS; ++i) {
      for (int j = 0; j < NUM_VERTEX_PER_AGENT; ++j) {
        int idx = i * NUM_VERTEX_PER_AGENT + j;
        state->vec[idx] = population.member[i]->pnt[j];
      }
    }
    
    for (int i=0;i<NUM_VERTICES_SUBSTRATE;++i){
       state->vec2[i] = Vec2f(population.substrate.pnt[i][0],population.substrate.pnt[i][1]);
    }
    
  }
};

int main() {
  MyApp app;
  app.start(true);  // use internal thread
  // app.start();
}
