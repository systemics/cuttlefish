#include "ctl_mainloop.h"
#include "ctl_stopwatch.h"
#include "ctl_throttle.h"
#include "ctl_sound.h"
#include "ctl_host.h"
#include "ctl_touch.h"
#include <lo/lo.h>
#include "ctl_osc.h"

#include <Gamma/Oscillator.h>

using namespace std;
using namespace ctl;

struct MyApp : MainLoop, Touch, Sound, Host, OSCPacketHandler {
  lo_address t, m, b, c;

  StopWatch<> stopWatch;

  gam::SineD<> sineD[5];

  MyApp() {
    if (identifier == 1) {
      t = lo_address_new("pi-t.mat.ucsb.edu", "8082");
      m = lo_address_new("pi-m.mat.ucsb.edu", "8082");
      b = lo_address_new("pi-b.mat.ucsb.edu", "8082");
      c = lo_address_new("pi-c.mat.ucsb.edu", "8082");
    }
    else {
      addListener(gotTouch, "/touch", "iii", this);
    }

    for (int i = 0; i < 5; ++i)
      sineD[i] = SineD<>(440 * (i + 1), 0.9, 0.3);
  }

  static int gotTouch(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {
    MyApp& app = *(MyApp*)user_data;  
    if (argv[0]->i < 5)
      app.sineD[argv[0]->i].reset();
    return 0;
  }  

  virtual void onLoop(float dt) {

    if (identifier == 1)
      if (pollTouches()) {
        for (int k = 0; k < N_SLOTS; ++k)
          if (touchPoint[k].id != -1) {
            printf("%u:(%i, %i) ", touchPoint[k].n, touchPoint[k].x, touchPoint[k].y);
            lo_send(t, "/touch", "iii", touchPoint[k].n, touchPoint[k].x, touchPoint[k].y);
            lo_send(m, "/touch", "iii", touchPoint[k].n, touchPoint[k].x, touchPoint[k].y);
            lo_send(b, "/touch", "iii", touchPoint[k].n, touchPoint[k].x, touchPoint[k].y);
            lo_send(c, "/touch", "iii", touchPoint[k].n, touchPoint[k].x, touchPoint[k].y);
          }
        printf("\n");
      }
  }

  virtual void onSound(AudioIOData& io) {
    if (identifier != 1)
      while (io()) {
        float f = 0;
        for (int i = 0; i < 5; ++i)
          f += sineD[i]();
        f /= 5;
        io.out(0) = io.out(1) = f;
      }
  }
};

int main() {
  MyApp().start();
  return 0;
}
