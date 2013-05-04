#ifndef CTL_TIMER_H_INCLUDED
#define CTL_TIMER_H_INCLUDED
#include <time.h>
#include <signal.h>
#include <assert.h>
#include <pthread.h>

namespace ctl {

void timer_function(sigval_t s);

class Timer {
  timer_t timerid;
  pthread_attr_t attr;
  struct itimerspec timer_settings;
  struct sigevent sevp;
public:
  Timer() {

    //assert(thread_attr_init(attr) == 0);
    pthread_getattr_np(pthread_self(), &attr);

    // create a POSIX high-precision timer
    //
    sevp.sigev_notify = SIGEV_THREAD;
    sevp.sigev_value.sival_ptr = (void*)this;
    sevp.sigev_notify_function = timer_function;
    sevp.sigev_notify_attributes = &attr;
    //sevp.sigev_notify_attributes = 0; // important
    assert(timer_create(CLOCK_REALTIME, &sevp, &timerid) == 0);
  }

  virtual ~Timer() {
    //assert(thread_attr_destroy(attr) == 0);
  }

  void start(float period);
  void stop();
  virtual void onTimer() = 0;
};

void timer_function(sigval_t s) {
  ((Timer*)(s.sival_ptr))->onTimer();
}


inline void Timer::start(float period) {

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

inline void Timer::stop() {
  // all zeros disarms the timer
  //
  timer_settings.it_interval.tv_sec = 0;
  timer_settings.it_interval.tv_nsec = 0;
  timer_settings.it_value.tv_sec = 0;
  timer_settings.it_value.tv_nsec = 0;
  assert(timer_settime(timerid, 0, &timer_settings, 0) == 0);
}

} //ctl::
#endif
