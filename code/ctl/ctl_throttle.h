/*
 *  ctl_throttle.h
 *  Cuttlefish
 *  Created by Karl Yerkes on Wed May 22 08:59:40 PDT 2013
 *  Copyright 2013 Karl Yerkes. All rights reserved.
 *
*/
#ifndef CTL_THROTTLE_H_INCLUDED
#define CTL_THROTTLE_H_INCLUDED
namespace ctl {
struct Throttle {
  float t, d;
  Throttle(float d) : t(0), d(d) {}
  bool operator()(float dt) {
    bool returnValue = (t > d);
    if (returnValue)
      t -= d;
    t += dt;
    return returnValue;
  }
};
}
#endif
