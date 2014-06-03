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

#include "ctl_app.h"

using namespace ctl;
using namespace gfx;

struct MyApp : CuttleboneApp<Foo> {

  MBO* circle;
  MBO* mboR;
  MBO* mboC;

  MyApp() : CuttleboneApp<Foo>(Layout(4, 4), 30.0) {}

  virtual void setup() { circle = new MBO(Mesh::Circle(6)); }

  virtual void update(float dt, Foo& state, int popCount) {

    // change position of vertices
    circle->mesh.moveTo(sin(state.time) * layout.totalWidth() / 2.0,
                        cos(state.time) * layout.totalHeight() / 2.0, 0);
    circle->mesh.color(1, fabs(sin(state.time)), 1);

    // send changes to buffer
    circle->update();

    mboR = new MBO(Mesh::Num(identifier.row).translate(-1, 0, 0).moveTo(
        layout.speakerL + Vec3f(3, 0, 0)));
    mboC = new MBO(Mesh::Num(identifier.col).translate(1, 0, 0).moveTo(
        layout.speakerL + Vec3f(layout.screenWidth - 3, 0, 0)));

    // background color
    background.set(fabs(sin(state.time)) * .2, fabs(cos(state.time)) * 1, 1, 1);
  }

  virtual void onDraw() {
    pipe.line(*circle);
    pipe.line(*mboR);
    pipe.line(*mboC);
  }

  virtual void onSound(SoundData& io) {}
};

#endif

int main() { MyApp().start(); }
