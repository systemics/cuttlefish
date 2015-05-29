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
  PointGroup2D<Vec> pg = PointGroup2D<Vec>(3,false);

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
      mesh[i].Col.set( Rand::Num() *.5 , 1+t*.2, 1-(t), 2);
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

 //   for (int k =0;k<NUM_CELLS_WIDTH_SUBSTRATE;++k){
 //    for (int m=0;m<NUM_CELLS_HEIGHT_SUBSTRATE;++m){
 //     float val = grid.cell(k,m);
 //     for (int j=0;j<NUM_VERTEX_BASE_SUBSTRATE;++j){
 //      auto mote =  pg( motif[j] );
 //      for (int l=0;l<NUM_REFLECTIONS_PER_CELL;++l){
 //       int ix = j*NUM_REFLECTIONS_PER_CELL + l;
 //       int idx = (k*NUM_CELLS_HEIGHT_SUBSTRATE +m) * NUM_VERTEX_PER_CELL + ix ;
 //       sub.mesh[idx].Col.set(val,val,.2,1);
 //       sub.mesh[idx].Pos = substrate.pnt[idx] + mote[l];
 //     
 //     }
 //    }
 //   }
 //   }


      
//    for (int k =0;k<NUM_CELLS_WIDTH_SUBSTRATE;++k){
//     for (int m=0;m<NUM_CELLS_HEIGHT_SUBSTRATE;++m){
//      float val = grid.cell(k,m);
//      for (int j=0;j<NUM_VERTEX_PER_CELL;++j){
//
//        int idx = (k*NUM_CELLS_HEIGHT_SUBSTRATE +m) * NUM_VERTEX_PER_CELL + j ;
//        sub.mesh[idx].Col.set(val,val,.2,1);
//        sub.mesh[idx].Pos = substrate.pnt[idx];
//      
//      }
//     }
//    }
//
 //     for (int j=0;j<NUM_VERTEX_BASE_SUBSTRATE;++j){
 //       for (int l=0;l<NUM_REFLECTIONS_PER_CELL;++l){
 //        int tmp = j+NUM_REFLECTIONS_PER_CELL+l;
 //        int idx = (k*NUM_CELLS_HEIGHT_SUBSTRATE +m) * NUM_VERTEX_PER_CELL + tmp ;
 //        sub.mesh[idx].Col.set(val,val,.2,1);
 //        sub.mesh[idx].Pos = substrate.pnt[idx] + motif[];
 //       }
 //      }
 //     }
 //   }

 
   for (int i=0;i<NUM_VERTEX_BASE_SUBSTRATE;++i){
     
     auto tmp = sg.apply( motif[i], NUM_CELLS_WIDTH_SUBSTRATE, NUM_CELLS_HEIGHT_SUBSTRATE);

     for (int k=0;k<NUM_CELLS_WIDTH_SUBSTRATE;++k){
     for (int m=0;m<NUM_CELLS_HEIGHT_SUBSTRATE;++m){
       
       int first = (k*NUM_CELLS_HEIGHT_SUBSTRATE + m) * NUM_VERTEX_PER_CELL; 
       int firstIn = (k*NUM_CELLS_HEIGHT_SUBSTRATE + m) * NUM_REFLECTIONS_PER_CELL;
       int firstOut = (k*NUM_CELLS_HEIGHT_SUBSTRATE + m) * NUM_VERTEX_PER_CELL + i;
       float val = grid.cell(k,m);

       for (int j =0;j<NUM_REFLECTIONS_PER_CELL;++j){
        int idx = j*NUM_VERTEX_BASE_SUBSTRATE + firstOut;
        int idx2 = first + j*NUM_VERTEX_PER_CELL;
        sub.mesh[idx].Pos = tmp[firstIn+j];
        sub.mesh[firstIn].Col.set(val,val,.2,1);
       }
       
     }}
      
   }
//
//
//  for (int i=0; i<NUM_VERTEX_BASE_SUBSTRATE;++i){
//      for (int k =0;k<NUM_CELLS_WIDTH_SUBSTRATE;++k){
//       for (int m=0;m<NUM_CELLS_HEIGHT_SUBSTRATE;++m){
//          for (int j=0;j<NUM_REFLECTIONS_PER_CELL;++j){
//           float val = grid.cell(k,m);
//           int idx = i * NUM_REFLECTIONS_PER_CELL*NUM_CELLS_SUBSTRATE + j;
//           sub.mesh[idx].Col.set(val,val,.2,.3); 
//         }
//      }
//    }
//  }

    sub.update();

  }


};

int main() {
  MyApp app;
  app.start();

  return 0;
}
