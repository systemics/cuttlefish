/*
 *  ctl_touch.h
 *  Cuttlefish
 *  Created by Karl Yerkes on Sat May  4 13:33:23 PDT 2013
 *  Copyright 2013 Karl Yerkes. All rights reserved.
 *
*/
#ifndef CTL_TOUCH_H_INCLUDED
#define CTL_TOUCH_H_INCLUDED

#include <assert.h>
#include <stdint.h>
#include <linux/input.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <map>
#include <iostream>
using namespace std;

#define BITS_PER_LONG (sizeof(long) * 8)
#define NBITS(x) ((((x)-1)/BITS_PER_LONG)+1)
#define OFF(x)  ((x)%BITS_PER_LONG)
#define BIT(x)  (1UL<<OFF(x))
#define LONG(x) ((x)/BITS_PER_LONG)
#define test_bit(bit, array)  ((array[LONG(bit)] >> OFF(bit)) & 1)

struct TouchPoint {
  int id;
  int x, y, major, minor, orientation;

  inline void make() {
    id = -1;
    x = y = major = minor = orientation = 0;
  }
};

namespace ctl {

struct Touch {
  unsigned activeTouchIndex, touchCount;
  TouchPoint touchPoint[16];
  int fd, rd;
  struct input_event ev[64];
  int version;
  unsigned short id[4];
  unsigned long bit[EV_MAX][NBITS(KEY_MAX)];

  Touch(const char* deviceName = "/dev/input/event0") {
    make(deviceName);
  }

  virtual ~Touch() {
  }

  int make(const char* deviceName) {
    assert((fd = open(deviceName, O_RDONLY | O_NONBLOCK)) >= 0);
    memset(bit, 0, sizeof(bit));
    ioctl(fd, EVIOCGBIT(0, EV_MAX), bit[0]);

    for (int i = 0; i < 16; ++i)
      touchPoint[i].make();
    activeTouchIndex = -1, touchCount = 0;
  }

  int poll() {
    rd = read(fd, ev, sizeof(struct input_event) * 64);

    if (rd < (int) sizeof(struct input_event)) {
      //perror("error reading\n");
      return -1;
    }

    for (unsigned i = 0; i < rd / sizeof(struct input_event); i++) {
      if (ev[i].type != 3)
        continue;

      switch (ev[i].code) {
        case 53:
        case 0:
          if (activeTouchIndex == -1)
            continue;
          touchPoint[activeTouchIndex].x = ev[i].value;
          break;

        case 54:
        case 1:
          if (activeTouchIndex == -1)
            continue;
          touchPoint[activeTouchIndex].y = ev[i].value;
          break;

        case 48:
          if (activeTouchIndex == -1)
            continue;
          touchPoint[activeTouchIndex].major = ev[i].value;
          break;

        case 49:
          if (activeTouchIndex == -1)
            continue;
          touchPoint[activeTouchIndex].minor = ev[i].value;
          break;

        case 52:
          if (activeTouchIndex == -1)
            continue;
          touchPoint[activeTouchIndex].orientation = ev[i].value;
          break;

        case 47:
          activeTouchIndex = ev[i].value;
          break;

        case 57:
          if (ev[i].value == -1)
            touchCount--;
          else
            touchCount++;
          touchPoint[activeTouchIndex].id = ev[i].value;
          break;
      }
    }

    /*
    for (int k = 0; k < 16; ++k)
      if (touchPoint[k].id != -1)
        printf("(%i, %i) ", touchPoint[k].x, touchPoint[k].y);
    printf("\n");
    */
  }
};

} //ctl::
#endif
