#include <stdint.h>
#include <linux/input.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <map>

using namespace std;

#define BITS_PER_LONG (sizeof(long) * 8)
#define NBITS(x) ((((x)-1)/BITS_PER_LONG)+1)
#define OFF(x)  ((x)%BITS_PER_LONG)
#define BIT(x)  (1UL<<OFF(x))
#define LONG(x) ((x)/BITS_PER_LONG)
#define test_bit(bit, array)  ((array[LONG(bit)] >> OFF(bit)) & 1)

struct Touch {
  int id;
  float x, y;
  float major, minor, angle;
  inline void make() {
    id = -1;
    x = y = major = minor = angle = 0;
  }
};

#define MAP(v, low, high) ((v - low) / (high - low))

int main (int argc, char **argv) {
  int fd, rd, i, j, k;
  struct input_event ev[64];
  int version;
  unsigned short id[4];
  unsigned long bit[EV_MAX][NBITS(KEY_MAX)];
  char name[256] = "Unknown";
  int abs[5];

  if (argc < 2) {
    printf("Usage: evtest /dev/input/eventX\n");
    printf("Where X = input device number\n");
    return 1;
  }

  if ((fd = open(argv[argc - 1], O_RDONLY)) < 0) {
    perror("evtest");
    return 1;
  }

  if (ioctl(fd, EVIOCGVERSION, &version)) {
    perror("evtest: can't get version");
    return 1;
  }

  printf("Input driver version is %d.%d.%d\n",
    version >> 16, (version >> 8) & 0xff, version & 0xff);

  ioctl(fd, EVIOCGID, id);
  printf("Input device ID: bus 0x%x vendor 0x%x product 0x%x version 0x%x\n", id[ID_BUS], id[ID_VENDOR], id[ID_PRODUCT], id[ID_VERSION]);

  ioctl(fd, EVIOCGNAME(sizeof(name)), name);
  printf("Input device name: \"%s\"\n", name);

  memset(bit, 0, sizeof(bit));
  ioctl(fd, EVIOCGBIT(0, EV_MAX), bit[0]);

  map<int, int> indexOf;
  Touch touch[10];
  for (int i = 0; i < 10; ++i)
    touch[i].make();
  int active = -1, count = 0;

  while (1) {
    rd = read(fd, ev, sizeof(struct input_event) * 64);

    if (rd < (int) sizeof(struct input_event)) {
      printf("yyy\n");
      perror("\nevtest: error reading");
      return 1;
    }

    for (i = 0; i < rd / sizeof(struct input_event); i++) {
      if (ev[i].type != 3)
        continue;
      //printf("%ld.%06ld (%d, %d)\n", ev[i].time.tv_sec, ev[i].time.tv_usec, ev[i].code, ev[i].value);

      switch (ev[i].code) {

        case 53: // _ABS_MT_POSITION_X:
        case 0: // _ABS_X:
          if (active == -1)
            return -1;
          touch[active].x = MAP((float)ev[i].value, -2909, 3167);
          break;

        case 54: // _ABS_MT_POSITION_Y:
        case 1: // _ABS_Y:
          if (active == -1)
            return -1;
          touch[active].y = MAP((float)ev[i].value, -2456, 2565);
          break;

        case 48: // _ABS_MT_TOUCH_MAJOR:
          if (active == -1)
            return -1;
          touch[active].major = MAP((float)ev[i].value, 0, 1020);
          break;

        case 49: // _ABS_MT_TOUCH_MINOR:
          if (active == -1)
            return -1;
          touch[active].minor = MAP((float)ev[i].value, 0, 1020);
          break;

        case 52: // _ABS_MT_ORIENTATION:
          if (active == -1)
            return -1;
          touch[active].angle = MAP((float)ev[i].value, -31, 32);
          break;

        //case 47: // _ABS_MT_SLOT:
        case 57: // _ABS_MT_TRACKING_ID:
          if (ev[i].value == -1) {
            count--;
            //indexOf.erase(active);
          }
          else {
            count++;
            active = ev[i].value;
            //indexOf.erase(active);
          }
          break;
      }

      if (active != -1) {
        printf("%d: (%f, %f) [%d]\n", active, touch[active].x, touch[active].y, count);
      }
    }
  }
}
