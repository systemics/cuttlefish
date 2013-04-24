//
//  rampsync.cpp
//
//  Created by Karl Yerkes on Tue Apr 23 16:52:23 PDT 2013
//  Copyright (c) 2013. All rights reserved.
//

#include "ctl_egl.h"
#include "ctl_gl.h"
#include <iostream>

using namespace std;
using namespace ctl::EGL;

struct MyWindow : public Window {

  float color;
  MyWindow() : Window () {
    color = 0;
    initGL();
  }

  ~MyWindow(){}

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
};

bool running = true;
void quit(int) {
  running = false;
}

int main() {
  bcm_host_init();
  signal(SIGABRT, quit);
  signal(SIGTERM, quit);
  signal(SIGINT, quit);

  MyWindow win;

  while (running) {
    win.onFrame();
    usleep(16666);
  }

  bcm_host_deinit();
  return 0;
}
