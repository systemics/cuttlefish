#include "ctl_app.h"
#include "ctl_mainloop.h"
#include "ctl_touch.h"
#include "ctl_gl_mbo.h"

#include <chrono>
#include <thread>

using namespace std;
using namespace ctl;

struct Timer {
  float t, d;
  Timer(float d) : t(0), d(d) {}
  bool operator()(float dt) {
    bool returnValue = (t > d);
    if (returnValue)
      t -= d;
    t += dt;
    return returnValue;
  }
};

struct MyApp : MainLoop, App, Touch {
  MBO* circles;
  Vec3f* touches;

  int num;

  MyApp() : App(14. + 3./8., 10. + 11./16.) {
    init();
  }

  virtual void onLoop(float dt) {
    static Timer frameTimer(1.0f / 60);
    if (frameTimer(dt))
      onFrame();
    poll();
  }

  virtual void init() {
    num = 10;
    touches = new Vec3f[num];
    circles = new MBO[num];
    for (int i = 0; i < num; ++i){
      double t = TWOPI * i / num;
      touches[i] = Vec3f(sin(t), cos(t), 0);
      circles[i] = MBO(Mesh::Circle());
    }
  }

  void updateMeshes() {
    for (int i = 0; i < num; ++i) {
      touches[i].x = touchPoint[i].x / 1000.0f;
      touches[i].y = touchPoint[i].y / -1000.0f;
      circles[i].mesh.moveTo(touches[i]);
      circles[i].update();
    }
  }

  virtual void onDraw() {
    updateMeshes();
    pipe.bind(scene.xf);
    for (int i = 0; i < num; ++i)
      pipe.line(circles[i]);
    pipe.unbind();
  }
};

int main() {
  MyApp().start();
  return 0;
}
