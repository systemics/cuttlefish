//
//  linesync.cpp
//
//  Created by Karl Yerkes on Tue Apr 23 16:52:23 PDT 2013
//  Copyright (c) 2013. All rights reserved.
//

#include "ctl_egl.h"
#include "ctl_gl.h"
#include "ctl_timer.h"
#include <iostream>

#include <lo/lo.h>
#include "ShaderManager.h"

using namespace std;
using namespace ctl::EGL;
using namespace ctl;

GLfloat rectangle[8] = { 0, 0, 0, 1, 1, 1, 1, 0 };
//GLfloat rectangle[8] = { -1, -1, -1,  1, 1,  1, 1, -1, };
GLubyte rectangle_index[] = { 0, 1, 2, 0, 2, 3, };

struct MyWindow : public Window, public Timer {

  ShaderManager shaderManager;

  MyWindow() : Window () {
    initGL();
  }

  ~MyWindow(){}

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
          gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0); 
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

bool running = true;
void quit(int) {
  running = false;
}

int update_rectangle(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {
  for (int i = 0; i < 8; ++i)
    rectangle[i] = argv[i]->f;
  printf("update_rectangle...\n");
  return 0;
}

int main() {
  bcm_host_init();
  signal(SIGABRT, quit);
  signal(SIGTERM, quit);
  signal(SIGINT, quit);

  lo_server_thread st = lo_server_thread_new("7770", 0);
  lo_server_thread_add_method(st, "/rectangle", "ffffffff", update_rectangle, 0);
  lo_server_thread_start(st);

  MyWindow win;

  win.start(1 / 60.);

  printf("\nrunning...\n");
  while (running)
    sleep(1);
  printf("...shutting down\n");

  win.stop();
  bcm_host_deinit();
  return 0;
}
