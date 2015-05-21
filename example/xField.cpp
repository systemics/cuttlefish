
#include "ctl_app.h"
#include "vsr/draw/vsr_cga3D_render.h"

using namespace std;
using namespace ctl; 
using namespace vsr;
using namespace vsr::cga;

struct MyState{};

struct MyApp : public RenderApp<MyState> {    
  
  float time;
  
  //A 3D Field of Points
  Field<Point> field;

  //A Meshbuffer to Render
  MBO mbo;
  
  virtual void setup(){
    field.resize(10,10,10);
    mbo = meshbuffer::make( field );    
    mSceneGraph.mMeshNode.add( &mbo );
  }  
  
  virtual void onAnimate(){

        
    RenderApp::onAnimate();
  
    time +=.01;
    
    Pair p = Tnv(0,0,1) * sin(time);
    
    for (int i = 0; i < field.num(); ++i){
      mbo.mesh[i].Pos = field.grid(i).boost(p);
    }
    
    mbo.update();
   
  } 
  
}; 

int main(){
  MyApp app;
  app.start();
  return 0;
}
