#include "ctl_bone.h"

struct Foo {
  float time;
};

#if SIM

struct MyApp : ctl::Simulator<Foo> {
  MyApp() : Simulator<Foo>("192.168.7.255", 1 / 30.f) {}
  virtual void setup(Foo& state) { state.time = 0; }
  virtual void update(float dt, Foo& state) { state.time += 0.01f; }
};

#else

#include "ctl_bcm.h"
#include "ctl_host.h"
#include "ctl_sound.h"
#include "ctl_egl.h"
#include "gfx/gfx_renderer.h"  //<-- the encapsulated rendering engine

struct MyApp : ctl::BCM, ctl::Host, ctl::Subscriber<Foo>, gfx::Renderer {

  ctl::EGL::Window* w;

  gfx::MBO* circle;
  gfx::MBO* mboR;
  gfx::MBO* mboC;

  MyApp() : gfx::Renderer(gfx::Layout(4, 4), 30.0) {
    setView(30.0, true, identifier.row, identifier.col);
  }

  virtual void firstRun() {
    w = new ctl::EGL::Window;
    initGL(gfx::Renderer::GLES, gfx::Renderer::BUFFERED, w->surface.width,
           w->surface.height);

    circle = new gfx::MBO(gfx::Mesh::Circle(6));
  }

  virtual void gotState(float dt, Foo& state, int popCount) {

    // change position of vertices
    circle->mesh.moveTo(sin(state.time) * layout.totalWidth() / 2.0,
                        cos(state.time) * layout.totalHeight() / 2.0, 0);
    circle->mesh.color(1, fabs(sin(state.time)), 1);

    // send changes to buffer
    circle->update();

    mboR =
        new gfx::MBO(gfx::Mesh::Num(identifier.row).translate(-1, 0, 0).moveTo(
            layout.speakerL + gfx::Vec3f(3, 0, 0)));
    mboC =
        new gfx::MBO(gfx::Mesh::Num(identifier.col).translate(1, 0, 0).moveTo(
            layout.speakerL + gfx::Vec3f(layout.screenWidth - 3, 0, 0)));

    // background color
    background.set(fabs(sin(state.time)) * .2, fabs(cos(state.time)) * 1, 1, 1);

    this->onFrame();
  }

  virtual void onDraw() {
    pipe.line(*circle);
    pipe.line(*mboR);
    pipe.line(*mboC);
  }

  virtual void onFrame() {
    gfx::Renderer::clear();
    gfx::Renderer::render();
    w->swapBuffers();
  }
};

#endif

int main() { MyApp().start(); }
