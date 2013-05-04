#include "Timer.hpp"
#include <unistd.h>

int main() {
  Timer timer;
  timer.start(1 / 60.);
  sleep(1);
  timer.stop();
  sleep(1);
}
