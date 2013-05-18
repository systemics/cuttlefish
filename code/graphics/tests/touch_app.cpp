#include "ctl_app.h"
#include "ctl_gl_mbo.h"
#include "ctl_touch.h"

using namespace std;
using namespace ctl;

struct MyApp : App, Touch {
  MBO* circles;       //<-- Circle Objects
  Vec3f* touches;        //<-- Vec3f touch posiitons

  int num;

  MyApp() : App(14. + 3./8., 10. + 11./16.) {
    init();
  }

  virtual void init(){
    cout << "init buffer objects" << endl;

    num = 10;
    touches = new Vec3f[num];
    circles = new MBO[num];
    for (int i = 0; i < num; ++i){
      double t = TWOPI * i/num;
      touches[i] = Vec3f(sin(t), cos(t), 0);     //<-- Default Positions Init
      circles[i] = MBO(Mesh::Circle());        //<-- Circle Buffers Init
    }
  }

  //moves circles to values stored in touches.
  void updateMeshes(){
    for (int i = 0; i < num; ++i) {
      touches[i].x = touchPoint[i].x / 2000.0f;
      touches[i].y = touchPoint[i].y / -2000.0f;
      circles[i].mesh.moveTo(touches[i]);
      circles[i].update();
    }
  }

  virtual void onDraw(){
    updateMeshes();
    pipe.bind(scene.xf);

    for (int i = 0; i < num; ++i)
      pipe.line(circles[i]);

    pipe.unbind();
  }
};

bool running = true;
void quit(int) {
  running = false;
}

int main() {
  MyApp app;

  while (running) {
    app.onFrame();
    app.poll();
    usleep(166);
  }

  return 0;
}
