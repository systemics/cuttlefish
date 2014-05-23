#include <algorithm>
#include <fcntl.h>
#include <linux/input.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <thread>
#include <unistd.h>

using namespace std;

#define BITS_PER_LONG (sizeof(long) * 8)
#define NBITS(x) ((((x)-1)/BITS_PER_LONG)+1)
#define OFF(x) ((x)%BITS_PER_LONG)
#define BIT(x) (1UL<<OFF(x))
#define LONG(x) ((x)/BITS_PER_LONG)
#define test_bit(bit, array) ((array[LONG(bit)] >> OFF(bit)) & 1)
#define N_SLOTS (16)

struct Touch {
  int id, n, x, y, major, minor, orientation;
  inline void make() {
    id = -1;
    x = y = major = minor = orientation = 0;
  }
};

bool running = true;
void quit(int) {
  running = false;
}

int main_other();
int main() {
  signal(SIGINT, quit);
  thread other(main_other);
  other.join();
}

int main_other() {
  int fd, rd;
  struct input_event ev[64];
  unsigned long bit[EV_MAX][NBITS(KEY_MAX)];

  if ((fd = open("/dev/input/event0", O_RDONLY | O_NONBLOCK)) < 0) {
    printf("FAIL!\n");
    return 1;
  }

  memset(bit, 0, sizeof(bit));
  ioctl(fd, EVIOCGBIT(0, EV_MAX), bit[0]);

  Touch touch[N_SLOTS];
  Touch sorted[N_SLOTS];
  for (int i = 0; i < N_SLOTS; ++i)
    touch[i].make();
  int slot = -1, count = 0;

  while (running) {
    usleep(1000);

    rd = read(fd, ev, sizeof(struct input_event) * 64);

    if (rd < (int) sizeof(struct input_event))
      continue;

    for (unsigned i = 0; i < rd / sizeof(struct input_event); i++) {
      if (ev[i].type != 3)
        continue;

      switch (ev[i].code) {

        // x position
        //
        case 53:
        //case 0:
          if (slot == -1)
            continue;
          touch[slot].x = ev[i].value;
          break;

        // y position
        //
        case 54:
        //case 1:
          if (slot == -1)
            continue;
          touch[slot].y = ev[i].value;
          break;

        // touch ellipse size on the major axis
        //
        case 48:
          if (slot == -1)
            continue;
          touch[slot].major = ev[i].value;
          break;

        // touch ellipse size on the major axis
        //
        case 49:
          if (slot == -1)
            continue;
          touch[slot].minor = ev[i].value;
          break;

        // touch ellipse orientation
        //
        case 52:
          if (slot == -1)
            continue;
          touch[slot].orientation = ev[i].value;
          break;

        // slot
        //
        case 47:
          slot = ev[i].value;
          break;

        // unique tracking id
        //
        case 57:
          touch[slot].n = count;
          if (ev[i].value == -1)
            count--;
          else
            count++;
          touch[slot].id = ev[i].value;
          break;
      }
    }

    memcpy(sorted, touch, sizeof(touch));
    sort(sorted, sorted + (sizeof(sorted) / sizeof(sorted[0])),
      [](const Touch& a, const Touch& b) {
        return a.n < b.n;
      }
    );

    //printf("%u ", count);
    //for (int k = 0; k < N_SLOTS; ++k)
    //  if (touch[k].id != -1)
    //    printf("%u:(%i, %i) ", touch[k].id, touch[k].x, touch[k].y);
    //printf("\n");

    printf("%u ", count);
    for (int k = 0; k < N_SLOTS; ++k)
      if (sorted[k].id != -1)
        printf("%u:(%i, %i) ", sorted[k].n, sorted[k].x, sorted[k].y);
    printf("\n");
  }

  return 0;
}
