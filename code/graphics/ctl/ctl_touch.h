/*
 *  ctl_touch.h
 *  Cuttlefish
 *  Created by Karl Yerkes on Sat May  4 13:33:23 PDT 2013
 *  Copyright 2013 Karl Yerkes. All rights reserved.
 *
*/
#ifndef CTL_TOUCH_H_INCLUDED
#define CTL_TOUCH_H_INCLUDED

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

namespace ctl {

struct TouchPoint {
  int id, n, x, y, major, minor, orientation;
  inline void make() {
    id = -1;
    x = y = major = minor = orientation = 0;
  }
};

struct Touch {
  int fd, rd;
  int slot = -1, touchCount = 0;
  struct input_event ev[64];
  unsigned long bit[EV_MAX][NBITS(KEY_MAX)];
  TouchPoint rawTouch[N_SLOTS];
  TouchPoint touchPoint[N_SLOTS];

  Touch() {
    if ((fd = open("/dev/input/event0", O_RDONLY | O_NONBLOCK)) < 0) {
      printf("FAIL!\n");
    }

    memset(bit, 0, sizeof(bit));
    ioctl(fd, EVIOCGBIT(0, EV_MAX), bit[0]);

    for (int i = 0; i < N_SLOTS; ++i)
      rawTouch[i].make();

    slot = -1;
    touchCount = 0;
  }

  bool pollTouches() {
    usleep(1000);

    rd = read(fd, ev, sizeof(struct input_event) * 64);
    if (rd < (int) sizeof(struct input_event))
      return false;

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
          rawTouch[slot].x = ev[i].value;
          break;

        // y position
        //
        case 54:
        //case 1:
          if (slot == -1)
            continue;
          rawTouch[slot].y = ev[i].value;
          break;

        // rawTouch ellipse size on the major axis
        //
        case 48:
          if (slot == -1)
            continue;
          rawTouch[slot].major = ev[i].value;
          break;

        // rawTouch ellipse size on the major axis
        //
        case 49:
          if (slot == -1)
            continue;
          rawTouch[slot].minor = ev[i].value;
          break;

        // rawTouch ellipse orientation
        //
        case 52:
          if (slot == -1)
            continue;
          rawTouch[slot].orientation = ev[i].value;
          break;

        // slot
        //
        case 47:
          slot = ev[i].value;
          break;

        // unique tracking id
        //
        case 57:
          rawTouch[slot].n = touchCount;
          if (ev[i].value == -1)
            touchCount--;
          else
            touchCount++;
          rawTouch[slot].id = ev[i].value;
          break;
      }
    }

    memcpy(touchPoint, rawTouch, sizeof(rawTouch));
    sort(touchPoint, touchPoint + (sizeof(touchPoint) / sizeof(touchPoint[0])),
      [](const TouchPoint& a, const TouchPoint& b) {
        return a.n < b.n;
      }
    );

    //printf("%u ", touchCount);
    //for (int k = 0; k < N_SLOTS; ++k)
    //  if (touchPoint[k].id != -1)
    //    printf("%u:(%i, %i) ", touchPoint[k].n, touchPoint[k].x, touchPoint[k].y);
    //printf("\n");
    return true;
  }
};

} //ctl::
#endif
