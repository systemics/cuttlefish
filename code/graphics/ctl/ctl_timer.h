/*
 *  ctl_timer.h
 *  Cuttlefish
 *  Created by Karl Yerkes on Sat May  4 13:33:23 PDT 2013
 *  Copyright 2013 Karl Yerkes. All rights reserved.
 *
*/
#ifndef CTL_TIMER_H_INCLUDED
#define CTL_TIMER_H_INCLUDED
#include <time.h>
#include <signal.h>
#include <assert.h>
#include <pthread.h>

namespace ctl {

class Timer;
Timer* that = 0;
void timer_function(int);

class Timer {
  timer_t timerid;
  struct itimerspec timer_settings;
  struct sigevent sevp;
public:
  Timer() {
    that = this;
    sevp.sigev_notify = SIGEV_SIGNAL;
    sevp.sigev_signo = SIGUSR1;
    sevp.sigev_value.sival_ptr = (void*)this;
    sevp.sigev_notify_attributes = 0; // important
    signal(SIGUSR1, timer_function);
    assert(timer_create(CLOCK_REALTIME, &sevp, &timerid) == 0);
  }

  virtual ~Timer() {
  }

  void start(float period);
  void stop();
  virtual void onTimer() = 0;
};

void timer_function(int) {
  that->onTimer();
}

inline void Timer::start(float period) {
  int t = (int)period;
  timer_settings.it_interval.tv_sec = t;
  timer_settings.it_interval.tv_nsec = (period - t) * 1000000000;
  timer_settings.it_value.tv_sec = 0;
  timer_settings.it_value.tv_nsec = 1;
  assert(timer_settime(timerid, 0, &timer_settings, 0) == 0);
}

inline void Timer::stop() {
  timer_settings.it_interval.tv_sec = 0;
  timer_settings.it_interval.tv_nsec = 0;
  timer_settings.it_value.tv_sec = 0;
  timer_settings.it_value.tv_nsec = 0;
  assert(timer_settime(timerid, 0, &timer_settings, 0) == 0);
}

} //ctl::
#endif
