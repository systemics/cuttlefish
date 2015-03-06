#include "ctl_bone.h"

struct Foo {
  float time;
  int touchCount;
  struct {
    float x, y;
  } touch[10];
};

#if SIM

#include "ctl_touch.h"

using namespace ctl;

struct MyApp : Simulator<Foo>, Touch {
  MyApp() : Simulator<Foo>("192.168.7.255", 1 / 30.f) {}
  virtual void setup(Foo& state) { state.time = 0; }
  virtual void update(float dt, Foo& state) {
    state.time += 0.01f;

    pollTouches();
    state.touchCount = 0;
    for (auto& e : touchPoint)
      if (e.id != 0) {
        state.touch[state.touchCount].x = e.x / 3200.0f;
        state.touch[state.touchCount].y = e.y / -2600.0f;
        state.touchCount++;
      }
    if (state.touchCount) LOG("x:%f y:%f", state.touch[0].x, state.touch[0].y);
  }
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

  virtual void setup() {
    circle = new MBO(Mesh::Circle(6));
    mboR = new MBO(Mesh::Num(identifier.row).translate(-1, 0, 0).moveTo(
        layout.speakerL + Vec3f(3, 0, 0)));
    mboC = new MBO(Mesh::Num(identifier.col).translate(1, 0, 0).moveTo(
        layout.speakerL + Vec3f(layout.screenWidth - 3, 0, 0)));
  }

  virtual void update(float dt, Foo& state, int popCount) {

    static float x, y;

    if (state.touchCount) {
      x = state.touch[0].x;
      y = state.touch[0].y;
    }

    circle->mesh.moveTo(x * layout.totalWidth() / 2.0,
                        y * layout.totalHeight() / 2.0, 0);
    circle->mesh.color(1, fabs(sin(state.time)), 1);

    // send changes to buffer
    circle->update();

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
