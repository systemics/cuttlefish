#include <iostream>
#include <chrono>
#include <unistd.h>

#include "../../graphics/ctl/ctl_stopwatch.h"

int main () {
  ctl::Stopwatch<> s;
  for (int i = 0; i < 20; ++i) {
    s.tic();
    usleep(5000);
    s.toc();
  }
  s.report();
  return 0;
}
