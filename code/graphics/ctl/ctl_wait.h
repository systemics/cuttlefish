#ifndef CTL_WAIT_H_INCLUDED
#define CTL_WAIT_H_INCLUDED

#include <iostream>
using namespace std;

namespace ctl {

/*

we might want to use a singleton at some point

class S {
  S() {};
  S(S const&);
  void operator=(S const&);
public:
  static S& getInstance() {
    static S instance;
    return instance;
  }
};
*/

bool running = true;
void quit(int) {
  running = false;
}

struct Wait {
  Wait() {
    signal(SIGABRT, quit);
    signal(SIGTERM, quit);
    signal(SIGINT, quit);
  }

  void wait() {
    cout << "\nwaiting...\n";
    while (running)
      sleep(1);
    cout << "\n...done\n";
  }

  virtual ~Wait() {
    usleep(100000);
  }
};

} //ctl::
#endif
