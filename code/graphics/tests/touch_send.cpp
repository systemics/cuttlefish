#include "ctl_mainloop.h"
#include "ctl_touch.h"
#include "ctl_throttle.h"
#include <lo/lo.h>

using namespace std;
using namespace ctl;

struct MyApp : MainLoop, Touch {
  lo_address t, m, b, c;

  MyApp() {
    t = lo_address_new("pi-t.mat.ucsb.edu", "8082");
    m = lo_address_new("pi-m.mat.ucsb.edu", "8082");
    b = lo_address_new("pi-b.mat.ucsb.edu", "8082");
    c = lo_address_new("pi-c.mat.ucsb.edu", "8082");
  }

  virtual void onLoop(float dt) {
    static unsigned sent = 0;
    if (pollTouches()) {
//      bool dirty = false;
      for (int k = 0; k < N_SLOTS; ++k)
        if ((touchPoint[k].id != 0) && (touchPoint[k].dirty == 1)) {
//          dirty = true;
//          printf("%u:(%i, %i) ", touchPoint[k].id, touchPoint[k].x, touchPoint[k].y);
          lo_send(t, "/touch", "iiii", touchPoint[k].n, touchPoint[k].x, touchPoint[k].y, touchPoint[k].orientation);
          lo_send(m, "/touch", "iiii", touchPoint[k].n, touchPoint[k].x, touchPoint[k].y, touchPoint[k].orientation);
          lo_send(b, "/touch", "iiii", touchPoint[k].n, touchPoint[k].x, touchPoint[k].y, touchPoint[k].orientation);
          lo_send(c, "/touch", "iiii", touchPoint[k].n, touchPoint[k].x, touchPoint[k].y, touchPoint[k].orientation);

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
