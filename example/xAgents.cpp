/*
 * =====================================================================================
 *
 *       Filename:  xAgents.cpp
 *
 *    Description:  raspberry pi rendering of Agents (controlled by control/xAgents_control.cpp)
 *
 *        Version:  1.0
 *        Created:  03/09/2015 20:08:38
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
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
#include "vsr/util/vsr_stat.h"

using namespace ctl;
using namespace gfx;

#define NUMPOSE 60

struct State {
  gfx::Pose pose[NUMPOSE];
  float speed[NUMPOSE];
};

struct App : Host {

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
      
      /*! SET TO TRUE FOR MULTISCREEN GRID */
      mSceneGraph.setView(50, false, identifier.row, identifier.col);
      //mSceneGraph.setView(50, true, identifier.row, identifier.col);
      setup();
    }

    virtual void setup()  = 0;                          //<-- subclasses must define this
    virtual void onDraw() { mSceneGraph.onRender(); }   //<-- subclasses can redefine this

    virtual void onAnimate(){
      int popCount = taker.get(*state);
    }

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

using namespace vsr;

struct MyApp : App {
  MBO mbo;
  void setup(){
    mbo = MBO( Mesh::Points(NUMPOSE*3).mode(GL::T), GL::DYNAMIC);
    mSceneGraph.mMeshNode.add(&mbo);
  }

  virtual void onAnimate(){
    App::onAnimate();
    //update mesh
    for(int i=0;i<NUMPOSE;++i){
      int idx = i*3;
      auto a = state->pose[i].pos()+state->pose[i].x();
      auto b = state->pose[i].pos()+state->pose[i].y();
      auto c = state->pose[i].pos()+state->pose[i].z();
      auto norm = (b-a).cross(c-a).unit();
      mbo.mesh[idx].Pos =  a; //Vec3f(Rand::Num(2), Rand::Num(2), Rand::Num(2) );
      mbo.mesh[idx+1].Pos = b;//state->pose[i].pos()+state->pose[i].y();
      mbo.mesh[idx+2].Pos =  c;//state->pose[i].pos()+state->pose[i].z();
      mbo.mesh[idx].Norm = norm;
      mbo.mesh[idx+1].Norm = norm;
      mbo.mesh[idx+2].Norm = norm;
      mbo.mesh[idx].Col = Vec4f( state->speed[i], 1, 1-state->speed[i],.8 );
      mbo.mesh[idx+1].Col = Vec4f( 1, state->speed[i], 1-state->speed[i],.8 );
      mbo.mesh[idx+2].Col = Vec4f( state->speed[i], 1-state->speed[i],1,.8 );
    }
    mbo.update();
  }
};

int main(){

  MyApp app;
  app.start();

  return 0;
}
