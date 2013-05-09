//
//  rampsync.cpp
//
//  Created by Karl Yerkes on Tue Apr 23 16:52:23 PDT 2013
//  Copyright (c) 2013. All rights reserved.
//

#include "ctl_bcm.h"
#include "ctl_wait.h"
#include "ctl_timer.h"
#include "ctl_egl.h"
#include "ctl_gl.h"

using namespace std;
using namespace ctl::EGL;
using namespace ctl;

struct App : BCM /* 1 */, Wait /* 2 */, Timer /* 3 */, Window /* 4 */ { // order matters

  float color;

  App() {
    color = 0;
    initGL();
  }

  ~App() {}

  void initGL(){
    glClearColor(1, 1, 1, 1);
    srand(time(NULL));
  }

  virtual void onDraw() {
  }

  virtual void onFrame() {
    color += 1/60.;
    if (color > 1)
      color = 0;
    glClearColor(color, color, color, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    onDraw();
    swapBuffers();
  }

  virtual void onTimer() {
    onFrame();
  }
};

int main() {
  App app;
  app.start(1 / 60.);
  app.wait();
  app.stop();
  return 0;
}
