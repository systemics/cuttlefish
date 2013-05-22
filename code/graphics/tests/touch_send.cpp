#include "ctl_mainloop.h"
#include "ctl_touch.h"
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

};

int main() {
  MyApp().start();
  return 0;
}
