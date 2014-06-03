/*
 * =====================================================================================
 *
 *       Filename:  ctl_app.h
 *
 *    Description:  APP CLASS FOR RASPBERRY PI
 *
 *        Version:  1.0
 *        Created:  02/17/2014 18:53:05
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:
 *
 * =====================================================================================
 */

#include "ctl_bcm.h"
#include "ctl_bone.h"
#include "ctl_timer.h"
#include "ctl_host.h"
#include "ctl_sound.h"
#include "ctl_osc.h"
#include "ctl_egl.h"

#include "gfx/gfx_renderer.h"  //<-- the encapsulated rendering engine

namespace ctl {

using namespace EGL;
using namespace gfx;

struct App : public BCM, Host, Window, Renderer, OSCPacketHandler, Sound {

  /// CONSTRUCTOR FOR SINGLE COMPUTER AND SCREEN
  App(float w, float h, float z = 30.0) : Window(), Renderer(w, h, z) {
    cout << "ID: " << identifier.row << " " << identifier.col << endl;
    Renderer::initGL(Renderer::GLES, Renderer::BUFFERED, surface.width,
                     surface.height);
    addListener(GetTouch, "/touch", "iiii", this);
  }

  /// CONSTRUCTOR FOR MULTIPLE COMPUTERS AND SCREENS
  App(const Layout &l, float z = 30.0) : Window(), Renderer(l, z) {
    cout << "ID: " << identifier.row << " " << identifier.col << endl;
    Renderer::initGL(Renderer::GLES, Renderer::BUFFERED, surface.width,
                     surface.height);
    /// Set individual view frustums
    setView(z, true, identifier.row, identifier.col);
    addListener(GetTouch, "/touch", "iiii", this);
  }

  virtual void init() {}
  virtual void update() {}

  // TOUCH CALLBACK (could be encapsulated in a TouchHandler class)
  static int GetTouch(const char *path, const char *types, lo_arg **argv,
                      int argc, void *data, void *user_data) {
    App &app = *(App *)user_data;
    app.onTouch(argv[0]->i, argv[1]->i, argv[2]->i, argv[3]->i);
  }

  virtual void onTouch(int n, int x, int y, int t) { cout << n << endl; }

  virtual void onSound(SoundData &io) {}

  ///  THIS IS THE FUNCTION TO OVERLOAD
  virtual void onDraw() {};

  /// You can also overload this function for adding effects (blur, motion
  /// trace, etc)
  virtual void onFrame() {
    update();
    Renderer::clear();
    Renderer::render();
    Window::swapBuffers();
  }
};

/// ***MACRO***
#define STARTANDRUN()                 \
  bool running = true;                \
  void quit(int) { running = false; } \
                                      \
  int main() {                        \
                                      \
    MyApp app;                        \
                                      \
    while (running) {                 \
      app.onFrame();                  \
      usleep(166);                    \
    }                                 \
                                      \
    return 0;                         \
  }

template <typename STATE, unsigned PACKET_SIZE = 1400>
struct CuttleboneApp : BCM,
                       Host,
                       Subscriber<STATE, PACKET_SIZE>,
                       Renderer,
                       Sound {

  EGL::Window* w;

  CuttleboneApp(float w, float h, float z = 30.0) : Renderer(w, h, z) {
    cout << "ID: " << identifier.row << " " << identifier.col << endl;
  }

  CuttleboneApp(const Layout &l, float z = 30.0) : Renderer(l, z) {
    cout << "ID: " << identifier.row << " " << identifier.col << endl;
    setView(z, true, identifier.row, identifier.col);
  }

  virtual void firstRun() {
    w = new EGL::Window;
    Renderer::initGL(Renderer::GLES, Renderer::BUFFERED, w->surface.width,
                     w->surface.height);
    setup();
  }

  virtual void gotState(float dt, STATE &state, int popCount) {
    update(dt, state, popCount);
    onFrame();
  }

  virtual void setup() = 0;
  virtual void update(float dt, STATE &state, int popCount) = 0;
  virtual void onSound(SoundData &io) = 0;
  virtual void onDraw() = 0;

  virtual void onFrame() {
    Renderer::clear();
    Renderer::render();
    w->swapBuffers();
  }
};

}  // ctl::
