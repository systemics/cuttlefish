/*
 * =====================================================================================
 *
 *       Filename:  xBasicApp.h
 *
 *    Description:  raw app
 *
 *        Version:  1.0
 *        Created:  02/26/2015 14:28:02
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */

#include "ctl_bcm.h"
#include "ctl_timer.h"
#include "ctl_host.h"
#include "ctl_sound.h"
#include "ctl_osc.h"

#include "Cuttlebone/cuttlebone.hpp"

#include "gfx/util/egl_window.h"  
#include "gfx/gfx_render.h"

using namespace ctl;
using namespace gfx;


/*-----------------------------------------------------------------------------
 *  STATE (TO BE PASSED AROUND TO ALL SCREENS)
 *-----------------------------------------------------------------------------*/
static const int NUM = 1000;
struct State {
  float speed[NUM];
};

struct App : Host {
  
  /*-----------------------------------------------------------------------------
   *  State
   *-----------------------------------------------------------------------------*/
   cuttlebone::Taker<State> taker;
   State * state;

  /*-----------------------------------------------------------------------------
   *  Context and Graphics
   *-----------------------------------------------------------------------------*/
    SceneGraph mSceneGraph;         //<-- OpenGL Pipeline Graph
    RPIContext mContext;            //<-- EGL Window Context 

   /*-----------------------------------------------------------------------------
    *  1. Initialize Context, Graphics Objects, and Monitor Layout
    *-----------------------------------------------------------------------------*/    
    void initContext(){
      RPIContext::System -> Initialize();
      mContext.create(1920,1080,"hullo");
      mSceneGraph.init(1920,1080);
      mSceneGraph.setView(50, true, identifier.row, identifier.col);
      setup();
    }

    virtual void setup()  = 0;                          //<-- subclasses MUST define this
    virtual void onDraw() { mSceneGraph.onRender(); }   //<-- subclasses CAN redefine this
    virtual void onAnimate(){}                          //<-- subclasses CAN redefine this

    void start(){
      initContext();     
      state = new State;
      taker.start();   
      while(true){       
         onFrame();
         usleep(166);
      }    
    }

    void onFrame(){
      onAnimate();
      onDraw(); 
      RPIContext::SwapBuffers();
    }

   ~App(){
     RPIContext::System -> Terminate();
   } 

};


struct MyApp : App {
  MBO mbo;
  void setup(){
    mbo =  Mesh::Circle(10) ;
    mSceneGraph.mMeshNode.add(&mbo);
  }
  
};

int main(){

  MyApp app;
  app.start();

  return 0;
}
