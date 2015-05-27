/*
 * =====================================================================================
 *
 *       Filename:  ctl_app.h
 *
 *    Description:  simplified wrapper
 *
 *        Version:  1.0
 *        Created:  05/19/2015 18:34:52
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

using namespace ctl;
using namespace gfx;

namespace ctl{

/*-----------------------------------------------------------------------------
 *  APPLICATION IS TEMPLATED ON THE STATE PARAMETERS TO BE PASSED WITH CUTTLEBONE
 *  THIS APPLICATION WILL TYPICALLY RUN ON ONE OF THE RASPBERRY PIs
 *-----------------------------------------------------------------------------*/
template<class STATE>
struct RenderApp : Host {
  
  /*-----------------------------------------------------------------------------
   *  State
   *-----------------------------------------------------------------------------*/
   cuttlebone::Taker<STATE> taker;
   STATE * state;

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
      //..................z...grid.............row............col
      mSceneGraph.setView(50, true, identifier.row, identifier.col);
      setup();
    }

    virtual void setup()  = 0;                          //<-- subclasses MUST define this
    virtual void onDraw() { mSceneGraph.onRender(); }   //<-- subclasses CAN redefine this

    virtual void onAnimate(){                           //<-- subclasses can redefine this is redefined
     // int popCount = taker.get(*state);
    }                        

    /*-----------------------------------------------------------------------------
     *  START MAIN LOOP (TO DO: launch audio)
     *-----------------------------------------------------------------------------*/
    void start(){
      initContext();     
      state = new STATE;
      taker.start();   
      while(true){       
         onFrame();
         usleep(166);
      }    
    }

    
    /*-----------------------------------------------------------------------------
     *  CALLED BY SCENEGRAPH OBJECT
     *-----------------------------------------------------------------------------*/
    void onFrame(){
      int popCount = taker.get(*state);
      onAnimate();
      onDraw(); 
      RPIContext::SwapBuffers();
    }

    void onSound(){} //..... to do
  
    /*-----------------------------------------------------------------------------
     *  CLEAN UP . . . (Q: how to clean up cuttlebone::Taker?)
     *-----------------------------------------------------------------------------*/
   ~RenderApp(){
     RPIContext::System -> Terminate();
   } 

};




}// ctl::
