/*
 * =====================================================================================
 *
 *       Filename:  xGFX_App.cpp
 *
 *    Description:  gfx app
 *
 *        Version:  1.0
 *        Created:  02/05/2015 15:27:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include "util/egl_window.h"
#include "gfx_app.h"

using namespace gfx;

/*-----------------------------------------------------------------------------
 * example use of gfx_renderer with mesh buffer objects
 *-----------------------------------------------------------------------------*/
struct MyApp : GFXApp<RPIContext> {

  MBO mbo;
  float time = 0;

  MyApp(int w, int h, string name) : GFXApp<RPIContext>(w,h,name) 
  {
  }

  void setup(){
    mbo = MBO( Mesh::Circle(1,10) );
    mRenderer.immediate(false);  
  }
  
   void onAnimate(){
    mbo.mesh.moveTo( sin(time) * 3,0,0);//scene.camera.lens.width()/2.0,0,0);
    mbo.update();
  }
  
   void onDraw(){
    time+=.015;      
    draw(mbo);
  }

};


int main(int argc, char ** argv){
  MyApp app(1920,1080, "test");
  app.start();
  return 0;
}

