#ifndef __TIMER__
#define __TIMER__
#include <time.h>
#include <signal.h>
class Timer {
  timer_t timerid;
  struct itimerspec timer_settings;
  struct sigevent sevp;
public:
  Timer();
  void start(float period);
  void stop();
  void onTimer();
};
#endif
