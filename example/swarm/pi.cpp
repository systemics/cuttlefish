#include "ctl_app.h"

#include "vsr/util/vsr_stat.h"

using namespace ctl;
using namespace gfx;

#include "state.hpp"
#include "swarm.h"
#include "ctl_grid.h"

using namespace vsr;

struct MyApp : RenderApp<State> {
 
  MBO mbo,sub;
  cuttlebone::Stats fps;

  Substrate substrate;
  ctl::Grid grid = ctl::Grid(NUM_CELLS_WIDTH_SUBSTRATE, NUM_CELLS_HEIGHT_SUBSTRATE);

  SpaceGroup2D<Vec> sg = SpaceGroup2D<Vec>(3,1,false);

  Point motif[ NUM_VERTEX_BASE_SUBSTRATE ];

  void setup() {  
    
    sg.mRatioX = 5;
    sg.mRatioY = 5;
    //motif
    for (int i =0; i< NUM_VERTEX_BASE_SUBSTRATE;++i){
      float t= TWOPI* (float)i/NUM_VERTEX_BASE_SUBSTRATE;
      motif[i] = point( CXY(1).dilate(PAO,1), t );
    }
    
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
    Mesh submesh = Mesh::Points(NUM_VERTICES_SUBSTRATE).mode(GL::T);
    for (int i = 0; i<submesh.num(); ++i){
       float t = (float)i/sub.mesh.num();
       submesh[i].Col.set( 1, 0, .2, 1);
       submesh[i].Pos = substrate.pnt[i];
     }

    sub = MBO(submesh, GL::DYNAMIC);

   mSceneGraph.mMeshNode.add(&sub);
   mSceneGraph.mMeshNode.add(&mbo);


  }

  virtual void onAnimate() {

    fps(dt);
    
    //AGENT MESH
    for (int i = 0;i < NUM_VERTICES;++i){
      mbo.mesh[i].Pos = state->vec[i];
    }
     mbo.update();


    for (int i=0;i<state->numtouches;++i){
      if (i<NUM_VERTEX_BASE_SUBSTRATE){
        motif[i] = point( state->touch[i][0] * 5, state->touch[i][1] * 5, 0 ); 
      }
    }
    
    memcpy( grid.data, state->food, sizeof(float)*NUM_CELLS_SUBSTRATE);
    
    for (int i=0;i<NUM_VERTEX_BASE_SUBSTRATE;++i){
      
      auto tmp = sg.apply( motif[i], NUM_CELLS_WIDTH_SUBSTRATE, NUM_CELLS_HEIGHT_SUBSTRATE);
 
      for (int k=0;k<NUM_CELLS_WIDTH_SUBSTRATE;++k){
      for (int m=0;m<NUM_CELLS_HEIGHT_SUBSTRATE;++m){
        
        
        int firstIn = (k*NUM_CELLS_HEIGHT_SUBSTRATE + m) * NUM_REFLECTIONS_PER_CELL;
        int firstOut = (k*NUM_CELLS_HEIGHT_SUBSTRATE + m) * NUM_VERTEX_PER_CELL + i;
        float val = grid.cell(k,m);

        for (int j =0;j<NUM_REFLECTIONS_PER_CELL;++j){
         int idx = j*NUM_VERTEX_BASE_SUBSTRATE + firstOut;
         sub.mesh[idx].Pos = tmp[firstIn+j];

        }

        int first = (k*NUM_CELLS_HEIGHT_SUBSTRATE + m) * NUM_VERTEX_PER_CELL;
        for (int j=0;j<NUM_REFLECTIONS_PER_CELL;++j){
          sub.mesh[firstOut+j].Col.set(val,val,.2,1);
        }
        
      }}
       
    }

  
    for (int i=0;i<NUM_CELLS_WIDTH_SUBSTRATE;++i){
      for (int j =0;j<NUM_CELLS_HEIGHT_SUBSTRATE;++j){
        int idx = (i*NUM_CELLS_HEIGHT_SUBSTRATE+j)*NUM_VERTEX_PER_CELL;
        float val = grid.cell(i,j);
         for (int k = 0; k<NUM_VERTEX_PER_CELL;++k){
          // sub.mesh[idx+k].Col.set(val,val,.2,1);
         }
        }
      }


    sub.update();

  }


};

int main() {
  MyApp app;
  app.start();

  return 0;
}
