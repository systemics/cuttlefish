#include "ctl_app.h"

#include "vsr/util/vsr_stat.h"

using namespace ctl;
using namespace gfx;

#include "state.hpp"

using namespace vsr;

struct MyApp : RenderApp<State> {
 
  MBO mbo;

  void setup() {
    
    mbo = MBO(Mesh::Points(NUM_VERTICES).mode(GL::T), GL::DYNAMIC);
    
    for (int i = 0; i<mbo.mesh.num(); ++i){
      float t = (float)i/mbo.mesh.num();
      mbo.mesh[i].Col.set( Rand::Num(), 1-t, t*.5, 1);
    }
    
    mSceneGraph.mMeshNode.add(&mbo);

  }

  virtual void onAnimate() {

    // update mesh
    for (int i = 0; i < NUM_VERTICES; ++i) {  
      int idx = i * 3;
      mbo.mesh[i].Pos = state->vec[i];
    }
    
    mbo.update();
  }
};

int main() {
  MyApp app;
  app.start();

  return 0;
}
