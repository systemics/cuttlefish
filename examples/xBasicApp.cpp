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
#include "ctl_bone.h"
#include "ctl_timer.h"
#include "ctl_host.h"
#include "ctl_sound.h"
#include "ctl_osc.h"

#include "gfx/util/egl_window.h"  
#include "gfx/gfx_render.h"

using namespace gfx;

struct SceneGraph {
    
    Scene mScene;

    GFXRenderNode mRenderNode;
    GFXViewNode mViewNode;
    GFXShaderNode mShaderNode;
    GFXSceneNode mSceneNode;
    GFXMeshNode mMeshNode;

    void init(int w, int h){
      mSceneNode.scene(&mScene);
      mShaderNode.immediate(false);
      mRenderNode << mViewNode << mShaderNode << mSceneNode << mMeshNode;
      mRenderNode.init(w,h);
    }

    void onRender(){
      mScene.push(false);
        mRenderNode.onRender();
      mScene.pop(false);
    }

};


struct App {

  /*-----------------------------------------------------------------------------
   *  Context and Graphics
   *-----------------------------------------------------------------------------*/
    SceneGraph mSceneGraph;         //<-- OpenGL Pipeline Graph
    RPIContext mContext;            //<-- EGL Window Context 

   /*-----------------------------------------------------------------------------
    *  1. Initialize Context and Graphics
    *-----------------------------------------------------------------------------*/    
    void initContext(){
      RPIContext::System -> Initialize();
      mContext.create(1920,1080,"hullo");
      mSceneGraph.init(1920,1080);
      setup();
    }

    virtual void setup()  = 0;                          //<-- subclasses must define this
    virtual void onDraw() { mSceneGraph.onRender(); }  //<-- subclasses must define this

    virtual void onAnimate(){}

    void start(){
      initContext();
      setup();      
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
    mbo = Mesh::Circle();
    mSceneGraph.mMeshNode.mbo(&mbo);
  }
};

int main(){

  MyApp app;
  app.start();

  return 0;
}
