#include "ctl_app.h"
#include "vsr/vsr_cga3D_op.h"
#include "vsr/vsr_render.h"

using namespace std;
using namespace ctl; 
using namespace vsr;
using namespace vsr::cga3D;

struct MyApp : public App {    
  
  float time;
  
  MyApp() : App(30,20,40){ 
    init();
  }
   
  virtual void init(){}  
  
  void update(){                          
    time += .01;
  }
  
  //YOUR DRAWING CODE HERE!!!!!
  virtual void onDraw(){
      
    Par p = Tnv(0,0,1) * sin(time);
  
    static Field<Pnt> f(50,50,1);
    static MBO m = MakeMeshBuffer( f );

    for (int i = 0; i < f.num(); ++i){
      f[i] = f.grid(i).bst( p );
    }
  
    Render(f, m, this);
   
  } 
  
}; 

STARTANDRUN()

