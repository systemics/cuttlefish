// Karl Yerkes
// Thu Apr 25 18:35:07 PDT 2013
//
// initial attempt at a Raspberry Pi + Apple Magic Trackpad class
//

#include <stdint.h>
#include <linux/input.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include <map>
using namespace std;

#ifndef EV_SYN
#define EV_SYN 0
#endif

#define BITS_PER_LONG (sizeof(long) * 8)
#define NBITS(x) ((((x)-1)/BITS_PER_LONG)+1)
#define OFF(x)  ((x)%BITS_PER_LONG)
#define BIT(x)  (1UL<<OFF(x))
#define LONG(x) ((x)/BITS_PER_LONG)
#define test_bit(bit, array)  ((array[LONG(bit)] >> OFF(bit)) & 1)

int main (int argc, char **argv) {
  int fd, rd, i, j, k;
  struct input_event ev[64];
  int version;
  unsigned short id[4];
  unsigned long bit[EV_MAX][NBITS(KEY_MAX)];
  char name[256] = "Unknown";
  int abs[5];

  map<int, const char*> code;
  code[0 ] = "ABS_X";
  code[1 ] = "ABS_Y";
  code[47] = "ABS_MT_SLOT";
  code[48] = "ABS_MT_TOUCH_MAJOR";
  code[49] = "ABS_MT_TOUCH_MINOR";
  code[52] = "ABS_MT_ORIENTATION";
  code[53] = "ABS_MT_POSITION_X";
  code[54] = "ABS_MT_POSITION_Y";
  code[57] = "ABS_MT_TRACKING_ID";


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

  printf("Testing ... (interrupt to exit)\n");

  while (1) {
    rd = read(fd, ev, sizeof(struct input_event) * 64);

    if (rd < (int) sizeof(struct input_event)) {
      printf("yyy\n");
      perror("\nevtest: error reading");
      return 1;
    }

    for (i = 0; i < rd / sizeof(struct input_event); i++)
      if (ev[i].type == 3) {
        ev[i].code,
        printf("Event: time %ld.%06ld, code %d (%s), value %d\n", ev[i].time.tv_sec, ev[i].time.tv_usec, ev[i].code, code[ev[i].code], ev[i].value);
      }
  }
}


