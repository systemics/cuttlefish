/*
 * =====================================================================================
 *
 *       Filename:  xEGLWindow.cpp
 *
 *    Description:  Same as xGlutBuffer.cpp but on a Raspberry pi
 *
 *        Version:  1.0
 *        Created:  05/21/2014 18:30:09
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
 * Call Renderer::initGL() after initializing window context 
 *-----------------------------------------------------------------------------*/
struct MyApp : GFXApp<RPIContext> {

  MBO mbo;
  float time = 0;

  MyApp(int w, int h, string name) : GFXApp<RPIContext>(w,h,name) {
      // process = new gfx::MotionTrace(_w,_h,this);  
  }

  void setup(){
    mbo = Mesh::Sphere();  
    mRenderer.immediate(false);
  }
  
   void update(){
//    mbo.mesh.moveTo( sin(time) * scene.camera.lens.width()/2.0,0,0);
   // mbo.update();
  }
  
   void onDraw(){
    time+=.015; 
  
    draw(mbo,1,0,0);
  }

};


int main(int argc, char ** argv){
  MyApp app(1600,1200, "test");
  app.start();
  return 0;
}

