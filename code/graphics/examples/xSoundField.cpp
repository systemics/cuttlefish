#include "ctl_app.h"
#include "gfx/gfx_mbo.h"

using namespace std;
using namespace ctl;

struct MyApp : public App {
  MBO* mbo;

  Quat model;

  float time;

  MyApp() : App(21.5, 14.5), time(0) {
    init();
  }

  virtual void onSound( SoundData& io ){
    for (int i = 0; i < io.n; ++i) {
      float s = i / (float)io.n;
      for (int j = 0; j < 2; j++) {
        *io.outputBuffer++ = (short)(s * 32767.0);
      }
    }
  }

  virtual void init(){
    Mesh mesh;
    mesh.add( -1, -1, 0 ).add();
    mesh.add( -1,  1, 0 ).add();
    mesh.add(  1,  1, 0 ).add();
    mesh.add(  1, -1, 0 ).add();
    mbo = new MBO( mesh );
  }

  void update() {
    time +=.01;
    mbo -> mesh.rotate( Quat( .05, Vec3f(0,1,0) ) );
    mbo -> update();

  }

  virtual void onDraw() {
    pipe.line( *mbo );
  }
};

bool running = true;
void quit(int) {
  running = false;
}

int main() {

  MyApp app;

  while(running){
    app.onFrame();
    usleep(1);
  }

  return 0;
}
