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
#include <chrono>
#include <unistd.h>
#include <assert.h>

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
  int id, n, x, y, major, minor, orientation, dirty;
  inline void make() {
    id = n = x = y = major = minor = orientation = dirty = 0;
  }
};

struct Touch {
  int ID, fd, slot, touchCount;
  struct input_event ev[64];
  unsigned long bit[EV_MAX][NBITS(KEY_MAX)];

  TouchPoint rawTouch[N_SLOTS];
  TouchPoint touchPoint[N_SLOTS];

  Touch() {
    assert((fd = open("/dev/input/event2", O_RDONLY | O_NONBLOCK)) >= 0);

    memset(bit, 0, sizeof(bit));
    ioctl(fd, EVIOCGBIT(0, EV_MAX), bit[0]);

    for (int i = 0; i < N_SLOTS; ++i) {
      rawTouch[i].make();
    }

    ID = 1;
    slot = -1;
    touchCount = 0;
  }

  bool pollTouches() {
    //usleep(1000);

    for (int i = 0; i < N_SLOTS; ++i)
      rawTouch[i].dirty = 0;

    int readResult = read(fd, ev, sizeof(struct input_event) * 64);
    if (readResult < (int)sizeof(struct input_event))
      return false;

    for (unsigned i = 0; i < readResult / sizeof(struct input_event); i++) {
      if (ev[i].type != 3)
        continue;

      switch (ev[i].code) {
        case 53:
          if (slot == -1)
            continue;
          rawTouch[slot].x = ev[i].value;
          rawTouch[slot].dirty = 1;
          break;

        case 54:
          if (slot == -1)
            continue;
          rawTouch[slot].y = ev[i].value;
          rawTouch[slot].dirty = 1;
          break;

        case 48:
          if (slot == -1)
            continue;
          rawTouch[slot].major = ev[i].value;
          rawTouch[slot].dirty = 1;
          break;

        case 49:
          if (slot == -1)
            continue;
          rawTouch[slot].minor = ev[i].value;
          rawTouch[slot].dirty = 1;
          break;

        case 52:
          if (slot == -1)
            continue;
          rawTouch[slot].orientation = ev[i].value;
          rawTouch[slot].dirty = 1;
          break;

        case 47:
          slot = ev[i].value;
          break;

        case 57:
          if (ev[i].value == -1) {
            touchCount--;
            rawTouch[slot].id = 0;
          }
          else {
            touchCount++;
            rawTouch[slot].id = ID++;
          }
          break;
      }
    }

    memcpy(touchPoint, rawTouch, sizeof(rawTouch));
    sort(touchPoint, touchPoint + (sizeof(touchPoint) / sizeof(touchPoint[0])),
      [](const TouchPoint& a, const TouchPoint& b) {
        return a.id < b.id;
      }
    );

    int finger = 0;
    for (int k = 0; k < N_SLOTS; ++k)
      if (touchPoint[k].id != 0)
        touchPoint[k].n = finger++;

    return true;
  }
};

} //ctl::
#endif
