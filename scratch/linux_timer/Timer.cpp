// read these for more information
//
// http://man7.org/linux/man-pages/man2/timer_create.2.html
// http://man7.org/linux/man-pages/man7/sigevent.7.html
// http://man7.org/linux/man-pages/man2/timer_settime.2.html
//

#include <time.h>
#include <signal.h>
#include <assert.h>
#include "Timer.hpp"

void timer_function(sigval_t s) {
  ((Timer*)(s.sival_ptr))->onTimer();
}

Timer::Timer() {
  // create a POSIX high-precision timer
  //
  sevp.sigev_notify = SIGEV_THREAD;
  sevp.sigev_value.sival_ptr = (void*)this;
  sevp.sigev_notify_function = timer_function;
  sevp.sigev_notify_attributes = 0; // important
  assert(timer_create(CLOCK_REALTIME, &sevp, &timerid) == 0);
}

void Timer::start(float period) {

  // set the period of the timer
  //
  int t = (int)period;
  timer_settings.it_interval.tv_sec = t;
  timer_settings.it_interval.tv_nsec = (period - t) * 1000000000;

  // start timer (almost) immediately
  //
  timer_settings.it_value.tv_sec = 0;
  timer_settings.it_value.tv_nsec = 1;

  // "arm" (start) the timer
  //
  assert(timer_settime(timerid, 0, &timer_settings, 0) == 0);
}

void Timer::stop() {
  // all zeros disarms the timer
  //
  timer_settings.it_interval.tv_sec = 0;
  timer_settings.it_interval.tv_nsec = 0;
  timer_settings.it_value.tv_sec = 0;
  timer_settings.it_value.tv_nsec = 0;
  assert(timer_settime(timerid, 0, &timer_settings, 0) == 0);
}

//void Timer::onTimer() {}
