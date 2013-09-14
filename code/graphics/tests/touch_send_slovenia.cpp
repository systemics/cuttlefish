#include "ctl_mainloop.h"
#include "ctl_touch.h"
#include "ctl_throttle.h"
#include <lo/lo.h>

using namespace std;
using namespace ctl;

struct MyApp : MainLoop, Touch {
  lo_address loa[9];

  MyApp() {
    loa[0] = lo_address_new("00.cf", "8082");
    loa[1] = lo_address_new("01.cf", "8082");
    loa[2] = lo_address_new("02.cf", "8082");
    loa[3] = lo_address_new("03.cf", "8082");
    loa[4] = lo_address_new("10.cf", "8082");
    loa[5] = lo_address_new("11.cf", "8082");
    loa[6] = lo_address_new("12.cf", "8082");
    loa[7] = lo_address_new("13.cf", "8082");
    loa[8] = lo_address_new("test.cf", "8082");
  }

  virtual void onLoop(float dt) {
    static unsigned sent = 0;
    if (pollTouches()) {
      for (int k = 0; k < N_SLOTS; ++k)
        if ((touchPoint[k].id != 0) && (touchPoint[k].dirty == 1)) {
          for (int j = 0; j < 9; ++j) {
            lo_send(loa[j], "/touch", "iiii", touchPoint[k].n, touchPoint[k].x, touchPoint[k].y, touchPoint[k].orientation);
          }
          sent++;
        }
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
