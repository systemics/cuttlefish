#include "Timer.hpp"
#include <unistd.h>
#include <stdio.h>

struct Foo : Timer {
  void onTimer() {
    printf("got here\n");
  }
};

int main() {
  Foo foo;
  foo.start(1 / 60.);
  sleep(1);
  foo.stop();
  sleep(1);
}
