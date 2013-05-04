/*
 *  ctl_bcm.h
 *  Cuttlefish
 *  Created by Karl Yerkes on Sat May  4 13:33:23 PDT 2013
 *  Copyright 2013 Karl Yerkes. All rights reserved.
 *
*/
#ifndef CTL_BCM_H_INCLUDED
#define CTL_BCM_H_INCLUDED

#include "bcm_host.h"
#include <iostream>
using namespace std;

namespace ctl {

struct BCM {
  BCM() {
    bcm_host_init();
    cout << "bcm_host_init()\n";
  }
  virtual ~BCM() {
    bcm_host_deinit();
    cout << "bcm_host_deinit()\n";
  }
};

} //ctl::
#endif
