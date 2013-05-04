#include <time.h>
#include <signal.h>
#include <assert.h>
#include <stdio.h>
#include "Timer.hpp"

void timer_function(sigval_t s) {
  ((Timer*)(s.sival_ptr))->onTimer();
}

Timer::Timer() {
  sevp.sigev_notify = SIGEV_THREAD;
  sevp.sigev_value.sival_ptr = (void*)this;
  sevp.sigev_notify_function = timer_function;
  sevp.sigev_notify_attributes = 0; // important
  assert(timer_create(CLOCK_REALTIME, &sevp, &timerid) == 0);
}

void Timer::start(float period) {
  int t = (int)period;
  timer_settings.it_interval.tv_sec = t;
  timer_settings.it_interval.tv_nsec = (period - t) * 1000000000;
  timer_settings.it_value.tv_sec = 0;
  timer_settings.it_value.tv_nsec = 1;
  assert(timer_settime(timerid, 0, &timer_settings, 0) == 0);
}

void Timer::stop() {
  timer_settings.it_interval.tv_sec = 0;
  timer_settings.it_interval.tv_nsec = 0;
  timer_settings.it_value.tv_sec = 0;
  timer_settings.it_value.tv_nsec = 0;
  assert(timer_settime(timerid, 0, &timer_settings, 0) == 0);
}

void Timer::onTimer() {
  printf("got here\n");
}
