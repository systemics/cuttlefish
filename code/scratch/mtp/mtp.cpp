#include <stdint.h>
#include <linux/input.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <algorithm>

using namespace std;

#define BITS_PER_LONG (sizeof(long) * 8)
#define NBITS(x) ((((x)-1)/BITS_PER_LONG)+1)
#define OFF(x) ((x)%BITS_PER_LONG)
#define BIT(x) (1UL<<OFF(x))
#define LONG(x) ((x)/BITS_PER_LONG)
#define test_bit(bit, array) ((array[LONG(bit)] >> OFF(bit)) & 1)
#define N_TOUCHES (10)

struct Touch {
  int id, n, x, y, major, minor, orientation;
  inline void make() {
    id = -1;
    x = y = major = minor = orientation = 0;
  }
};

int main () {
  int fd, rd;
  struct input_event ev[64];
  //unsigned short id[4];
  unsigned long bit[EV_MAX][NBITS(KEY_MAX)];

  if ((fd = open("/dev/input/event0", O_RDONLY)) < 0) {
    perror("evtest");
    return 1;
  }

  memset(bit, 0, sizeof(bit));
  ioctl(fd, EVIOCGBIT(0, EV_MAX), bit[0]);

  Touch touch[N_TOUCHES];
  Touch sorted[N_TOUCHES];
  for (int i = 0; i < N_TOUCHES; ++i)
    touch[i].make();
  int active = 0, count = 0;

  while (true) {
    rd = read(fd, ev, sizeof(struct input_event) * 64);

    if (rd < (int) sizeof(struct input_event)) {
      printf("yyy\n");
      perror("\nevtest: error reading");
      return 1;
    }

    for (unsigned i = 0; i < rd / sizeof(struct input_event); i++) {
      if (ev[i].type != 3)
        continue;

      switch (ev[i].code) {

        // x position
        //
        case 53:
        case 0:
          touch[active].x = ev[i].value;
          break;

        // y position
        //
        case 54:
        case 1:
          touch[active].y = ev[i].value;
          break;

        // touch ellipse size on the major axis
        //
        case 48:
          touch[active].major = ev[i].value;
          break;

        // touch ellipse size on the major axis
        //
        case 49:
          touch[active].minor = ev[i].value;
          break;

        // touch ellipse orientation
        //
        case 52:
          touch[active].orientation = ev[i].value;
          break;

        // slot
        //
        case 47:
          active = ev[i].value;
          break;

        // unique tracking id
        //
        case 57:
          touch[active].n = count;
          if (ev[i].value == -1)
            count--;
          else
            count++;
          touch[active].id = ev[i].value;
          break;
      }
    }

    memcpy(sorted, touch, sizeof(touch));
    sort(sorted, sorted + (sizeof(sorted) / sizeof(sorted[0])),
      [](const Touch& a, const Touch& b) {
        return a.n < b.n;
      }
    );

    for (int k = 0; k < N_TOUCHES; ++k)
      if (sorted[k].id != -1)
        printf("%u:(%i, %i) ", sorted[k].n, sorted[k].x, sorted[k].y);
    printf("\n");
  }
}
