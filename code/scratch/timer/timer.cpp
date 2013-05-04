#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>

struct Foo {
  void print() {
    printf("got here\n");
  }
};

void function(sigval_t s) {
  ((Foo*)(s.sival_ptr))->print();
}

int main() {
  timer_t timerid;
  struct itimerspec new_value = {{0, 16666666}, {1, 0}};
  struct sigevent sevp;

  Foo foo;

  sevp.sigev_notify = SIGEV_THREAD;
  sevp.sigev_value.sival_ptr = (void*)&foo;
  sevp.sigev_notify_function = function;
  sevp.sigev_notify_attributes = 0; // important

  assert(timer_create(CLOCK_REALTIME, &sevp, &timerid) == 0);
  assert(timer_settime(timerid, 0, &new_value, 0) == 0);

  for (;;) {
    sleep(1);
  }
}
