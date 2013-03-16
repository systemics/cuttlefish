#include <iostream>
#include <vector>
#include <signal.h>
#include "bcm_host.h"
#include "EGLWindow.hpp"

using namespace std;

struct MyGLWindow : public EGLWindow {
  struct {
    float r, g, b;
  } color;

  MyGLWindow(EGLconfig* config) : EGLWindow(config) {
  }

  virtual ~MyGLWindow() {}

  virtual void paintGL() {
    glClearColor(color.r, color.g, color.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    glFinish();
    swapBuffers();
  }

  virtual void initializeGL() {}
};

int main(int argc, char* argv[]) {
  bcm_host_init();
  atexit(bcm_host_deinit);
  signal(SIGINT, exit);

  vector<MyGLWindow*> window;
  EGLconfig* config = new EGLconfig();
  config->setRGBA(0, 0, 0, 0);
  config->setDepth(24);
  MyGLWindow* win = new MyGLWindow(config);
  win->setUpscale(false);
  win->setScreen(0, 0, 64, 64);
  win->color.r = 1.0;
  window.push_back(win);

  while (true) {
    for (unsigned i = 0; i < window.size(); ++i)
      window[i]->paintGL();
    usleep(100000);
  }

  for (unsigned i = 0; i < window.size(); ++i)
    delete window[i];

  bcm_host_deinit();
  return 0;
}
