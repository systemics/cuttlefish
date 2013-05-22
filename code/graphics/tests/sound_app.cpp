#include "ctl_mainloop.h"
#include "ctl_stopwatch.h"
#include "ctl_throttle.h"
#include "ctl_sound.h"

#include <Gamma/Oscillator.h>

using namespace std;
using namespace ctl;
using namespace gam;

struct MyApp : MainLoop, Sound {
  StopWatch<> stopWatch;

  SineD<> sineD;

  MyApp() {
    sineD = SineD<>(220, 0.6, 0.5);
  }

  virtual void onLoop(float dt) {

    static Throttle frameThrottle(1.0f / 20);
    if (frameThrottle(dt)) {
      stopWatch.start();
      usleep(1000);
      stopWatch.stop();
    }

    static Throttle reportThrottle(1.0f);
    if (reportThrottle(dt)) {
      stopWatch.report();
      sineD.reset();
    }

  }

  virtual void onSound(AudioIOData& io) {
    while (io()) {
      io.out(0) = io.out(1) = sineD();
    }
  }
};

int main() {
  MyApp().start();
  return 0;
}
