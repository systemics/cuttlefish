#include "cf_app.h"
#include "gfx/gfx_mbo.h"

using namespace std;
using namespace ctl;

struct MyApp : public App {

    //THIS IS THE APP SPECIFIC STUFF,
  MBO * mbo;

  Quat model;
  //Frame model;

  float time;
  float x, y;

  //INSTANTIATE THE APPLICATION WITH WIDTH AND HEIGHT
  MyApp() : App(30, 20), time(0) {
    init();
    x = 0;
    y = 0;
  }

  //INIITIALIZE ELEMENTS OF THE SCENE
  virtual void init(){

    //Make a Mesh and add vertices
    Mesh mesh;
    mesh.add ( -1, -1, 0).add();
    mesh.add ( -1, 1, 0).add();
    mesh.add ( 1, 1,  0).add();
    mesh.add ( 1, -1, 0).add();


    mbo = new MBO( mesh );
  }

  void onTouch(int finger, int xPosition, int yPosition, int orientation){
    x = xPosition / 3000.0;
    y = yPosition / 2500.0;
  }

  void update() {
    time +=.01;

    //scene.model.rot()   = Quat( sin( time) * PI, Vec3f(1,0,0) );
    //scene.camera.rot()  = Quat( sin( time) * PI, Vec3f(0,1,0) );

    mbo -> mesh.rotateA( Quat( sin((x - 0.5) * 2), Vec3f(0,1,0) ) );
    mbo -> mesh.translate( (y - 0.5) * 3, 0, 0 );
    mbo -> update();
  }

  //THIS DRAWS THE CIRCLE TO THE SCREEN
  virtual void onDraw() {
    pipe.line( *mbo );
  }
};

STARTANDRUN()

