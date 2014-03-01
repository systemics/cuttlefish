//
//  blinksync.cpp
//
//  Created by Karl Yerkes on Tue Apr 23 16:52:23 PDT 2013
//  Copyright (c) 2013. All rights reserved.
//

#include "ctl_egl.h"
#include "ctl_gl.h"
#include <iostream>

#include "ShaderManager.h"

using namespace std;
using namespace ctl::EGL;

GLfloat rectangle[12] = {
  -1, -1,  0,
  -1,  1,  0,
   1,  0,  0,
   1,  0,  0,
//   1,  1,  0,
//   1, -1,  0,
};

using namespace ctl;

struct MyWindow : public Window {

  ShaderManager shaderManager;

  MyWindow() : Window () {
    initGL();
  }

  ~MyWindow(){}

  void initGL(){
    glClearColor(1, 1, 1, 1);
  }

  virtual void onDraw() {
    glVertexAttribPointer(shaderManager.locationOf("position"), 3, GL_FLOAT, 0, 0, rectangle);
    glEnableVertexAttribArray(shaderManager.locationOf("position"));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }

  virtual void onFrame() {
    static bool firstTime = true;
    if (firstTime) {
      firstTime = false;
      shaderManager.make("basic",
        STRINGIFY(
          attribute vec4 position;
          void main(void) {
            gl_Position = position;
          }
        ),
        STRINGIFY(
          void main(void) {
            gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0); 
          }
        )
      );
    }

    glClearColor(0, 0, 0, 1);
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

    cout << "GOTHERE" << endl;
  MyWindow win;

  while (running) {
    win.onFrame();
    usleep(16666);
  }

  bcm_host_deinit();
  return 0;
}
