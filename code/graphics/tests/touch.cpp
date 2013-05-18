#include "ctl_mainloop.h"
#include "ctl_touch.h"

#include <iostream>
using namespace std;
using namespace ctl;

struct MyApp : MainLoop, Touch {
  MyApp() {
    cout << "MyApp()\n";
  }

  virtual void onLoop() {
    poll();
    usleep(1666);
  }
};

int main() {
  cout << "main()\n";
  MyApp().start();
  return 0;
}
