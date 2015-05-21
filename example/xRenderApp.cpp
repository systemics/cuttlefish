/*
 * =====================================================================================
 *
 *       Filename:  xRenderApp.cpp
 *
 *    Description:  uses ctl_app.h as default application
 *
 *        Version:  1.0
 *        Created:  05/19/2015 18:41:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */

#include "ctl_app.h"

struct MyState{

};

struct MyApp : RenderApp<MyState> {

  MBO mbo;
  
  void setup(){
    mbo = Mesh::Circle(10);
    mSceneGraph.mMeshNode.add(&mbo);
  }

  void onAnimate(){
    RenderApp::onAnimate(); //<-- get state information
    mbo.update();
  }

};


int main(){
  MyApp app;
  app.start();
  return 0;
}
