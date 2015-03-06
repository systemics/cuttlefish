#include "ctl_mainloop.h"
#include "ctl_touch.h"
#include "ctl_throttle.h"
#include <lo/lo.h>

using namespace std;
using namespace ctl;

struct MyApp : MainLoop, Touch {
  lo_address broadcast;

  MyApp() {
    broadcast = lo_address_new("255.255.255.255", "8082");
  }

  virtual void onLoop(float dt) {
    static unsigned sent = 0;
    if (pollTouches()) {
//      bool dirty = false;
      for (int k = 0; k < N_SLOTS; ++k)
        if ((touchPoint[k].id != 0) && (touchPoint[k].dirty == 1)) {
//          dirty = true;
//          printf("%u:(%i, %i) ", touchPoint[k].id, touchPoint[k].x, touchPoint[k].y);
          lo_send(broadcast, "/touch", "iiii", touchPoint[k].n, touchPoint[k].x, touchPoint[k].y, touchPoint[k].orientation);
          sent++;
        }
//      if (dirty)
//        printf("\n");
    }

    static Throttle reportThrottle(1.0f);
    if (reportThrottle(dt))
      cout << sent << endl;
  }

};

int main() {
  MyApp().start();
  return 0;
}
