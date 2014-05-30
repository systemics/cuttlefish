#include "ctl_bone.h"

struct Foo {
  float value;
};

#if SIM

struct MyApp : ctl::Simulator<Foo> {
  MyApp() : Simulator<Foo>("192.168.7.255", 1 / 30.f) {
    //
    // shouldLog = true;
  }
  virtual void setup(Foo& state) { state.value = 0; }
  virtual void update(float dt, Foo& state) {
    state.value += 0.01f;
    //    LOG("update(%f) -> %f", dt, state.value);
  }
};

#else

#include "ctl_bcm.h"
#include "ctl_host.h"
#include "ctl_sound.h"
#include "ctl_egl.h"
#include "gfx/gfx_renderer.h"  //<-- the encapsulated rendering engine

struct MyApp : ctl::BCM, ctl::Host, ctl::Renderer<Foo>, gfx::Renderer {
  ctl::EGL::Window* w;
  float v;

  MyApp() : gfx::Renderer(30, 20) {
    //
    // shouldLog = true;
  }

  virtual void firstRun() {
    w = new ctl::EGL::Window;
    initGL(gfx::Renderer::GLES, gfx::Renderer::BUFFERED, w->surface.width,
           w->surface.height);
  }

  virtual void gotState(float dt, Foo& state, int popCount) {
    v = state.value - int(state.value);
    this->onFrame();
  }

  virtual void onDraw() { background.set(v, 1 - v, v * v * v, 1.0f); }

  virtual void onFrame() {
    gfx::Renderer::clear();
    gfx::Renderer::render();
    w->swapBuffers();
  }
};

#endif

int main() { MyApp().start(); }
