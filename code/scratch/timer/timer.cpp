#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>

void sig_handler(int signo) {
  printf("GOT HERE\n");
}

int main() {
  assert(signal(SIGUSR1, sig_handler) != SIG_ERR);

  struct sigevent sevp;
  sevp.sigev_notify = SIGEV_SIGNAL;
  sevp.sigev_signo = SIGUSR1;

  timer_t timerid;
  assert(timer_create(CLOCK_REALTIME, &sevp, &timerid) == 0);
  const struct itimerspec new_value = {{0, 16666666}, {1, 0}};
  assert(timer_settime(timerid, 0, &new_value, 0) == 0);

  for (;;) {
    sleep(1);
  }
}
