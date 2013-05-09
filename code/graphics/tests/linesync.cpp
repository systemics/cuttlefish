//
//  linesync.cpp
//
//  Created by Karl Yerkes on Tue Apr 23 16:52:23 PDT 2013
//  Copyright (c) 2013. All rights reserved.
//

#include "ctl_bcm.h"
#include "ctl_wait.h"
#include "ctl_timer.h"
#include "ctl_egl.h"
#include "ctl_gl.h"
#include "ctl_shader_manager.h"
#include "ctl_stopwatch.h"
#include <iostream>

#include <lo/lo.h>

using namespace std;
using namespace ctl::EGL;
using namespace ctl;

GLfloat rectangle[8] = { 0, 0, 0, .1, .1, .1, .1, 0 };
GLubyte rectangle_index[] = { 0, 1, 2, 0, 2, 3, };

struct Rpi : BCM, Wait, Timer {};
struct App : Rpi, Window {
  Stopwatch stopwatch;

  ShaderManager shaderManager;

  App() { // : Window() {
    initGL();
  }

  ~App(){}

  void initGL(){
    glClearColor(1, 0, 0, 1);
    shaderManager.make("basic",
      STRINGIFY(
        attribute vec2 position;
        void main(void) {
          gl_Position = vec4(position.x * 2.0 - 1.0, position.y * 2.0 - 1.0, 0., 1.);
        }
      ),
      STRINGIFY(
        void main(void) {
          gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0); 
        }
      )
    );
  }

  virtual void onDraw() {
    glVertexAttribPointer(shaderManager.locationOf("position"), 2, GL_FLOAT, 0, 0, rectangle);
    glEnableVertexAttribArray(shaderManager.locationOf("position"));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, rectangle_index);
  }

  virtual void onFrame() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    onDraw();
    swapBuffers();
  }

  virtual void onTimer() {
    onFrame();
  }
};

int update_rectangle(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {
  for (int i = 0; i < 8; ++i)
    rectangle[i] = argv[i]->f;
  return 0;
}

int main() {

  lo_server_thread st = lo_server_thread_new("7770", 0);
  lo_server_thread_add_method(st, "/rectangle", "ffffffff", update_rectangle, 0);
  lo_server_thread_start(st);

  App app;
  app.start(1 / 60.);
  app.wait();
  app.stop();

  return 0;
}
