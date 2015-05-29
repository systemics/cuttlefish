#include "ctl_app.h"

#include "vsr/util/vsr_stat.h"

using namespace ctl;
using namespace gfx;

#include "state.hpp"

using namespace vsr;

struct MyApp : RenderApp<State> {
 
  MBO mbo,sub;
  cuttlebone::Stats fps;

  void setup() {  
    
    ///POPULATION MESH
    Mesh mesh = Mesh::Points(NUM_VERTICES).mode(GL::L);
    mesh.index().clear();
    for (int i = 0; i<NUM_AGENTS; ++i){
      for (int j=0; j<MAX_NUM_REFLECTIONS; ++j){
        int first = i * NUM_VERTEX_PER_AGENT + j * NUM_VERTEX_BASE;
        for (int k=0;k<NUM_VERTEX_BASE;++k){
         int idx = first+k;
         int nxt = k<NUM_VERTEX_BASE-1 ? idx + 1 : first;
         mesh.add(idx).add(nxt);
        }
      }
    }

    for (int i = 0; i<mesh.num(); ++i){
      float t = (float)i/mesh.num();
      mesh[i].Col.set( Rand::Num(), 1-(t), t*.5, 1);
    }

    mbo = MBO(mesh, GL::DYNAMIC);
    
    ///BACKGROUND "SUBSTRATE" MESH
    // Mesh submesh = Mesh::Points(NUM_VERTICES_SUBSTRATE).mode(GL::L);
    // for (int i = 0; i<submesh.num(); ++i){
    //   float t = (float)i/sub.mesh.num();
    //   submesh[i].Col.set( 1, 0, .2, 1);
    // }
    // sub = MBO(submesh, GL::DYNAMIC);

   mSceneGraph.mMeshNode.add(&mbo);
//   mSceneGraph.mMeshNode.add(&sub);

  }

  virtual void onAnimate() {


    fps(dt);
    
    //AGENT MESH
    for (int i = 0;i < NUM_VERTICES;++i){
      mbo.mesh[i].Pos = state->vec[i];
    }
    mbo.update();

    //SUBSTRATE MESH
    for (int i = 0;i < NUM_VERTICES_SUBSTRATE;++i){
   //   sub.mesh[i].Pos = Vec3f(state->vec2[i][0], state->vec2[i][1],0);
    }
   // sub.update();
  }

//  virtual void onDraw(){
//    render::pipe(mbo, &mSceneGraph.mSceneNode);
//    render::pipe(sub, &mSceneGraph.mSceneNode);
//  }
};

int main() {
  MyApp app;
  app.start();

  return 0;
}
