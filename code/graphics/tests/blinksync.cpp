//
//  blinksync.cpp
//
//  Created by Karl Yerkes on Tue Apr 23 16:52:23 PDT 2013
//  Copyright (c) 2013. All rights reserved.
//

#include "ctl_bcm.h"
#include "ctl_wait.h"
#include "ctl_timer.h"
#include "ctl_egl.h"
#include "ctl_gl.h"
#include <iostream>

using namespace std;
using namespace ctl::EGL;
using namespace ctl;

struct App : BCM /* 1 */, Wait /* 2 */, Timer /* 3 */, Window /* 4 */ { // order matters

  int n;
  App() {
    n = 0;
    initGL();
  }

  ~App(){}

  void initGL(){
    glClearColor(1, 1, 1, 1);
    srand(time(NULL));
  }

  virtual void onDraw() {
  }

  virtual void onTimer() {
    onFrame();
  }

  virtual void onFrame() {
    n++;

    if (n & (1 << 5))
      glClearColor((n & 1), (n & 1), (n & 1), 1);
    else
      glClearColor(0, 0, 0, 1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    onDraw();
    swapBuffers();
  }
};

int main() {
  App app;
  app.start(1 / 60.);
  app.wait();
  app.stop();
  return 0;
}
