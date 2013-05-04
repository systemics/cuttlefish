#include <time.h>
#include <signal.h>
#include <unistd.h>

int main() {
  // int timer_create(clockid_t clockid, struct sigevent *sevp, timer_t *timerid);
  // int timer_settime(timer_t timerid, int flags, const struct itimerspec *new_value, struct itimerspec * old_value);

  int result = 0;

  struct sigevent sevp;
  timer_t timerid;
  result = timer_create(CLOCK_REALTIME, &sevp, &timerid);

  const struct itimerspec new_value = {{0, 16666666}, {1, 0}};
  result = timer_settime(timerid, 0, &new_value, 0);

  for (;;) {
    sleep(1);
  }
}
