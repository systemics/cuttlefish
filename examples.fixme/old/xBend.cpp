#include "ctl_app.h"
#include "vsr/vsr_render.h"
#include "vsr/vsr_cga3D_frame.h"

using namespace std;
using namespace ctl; 
using namespace vsr;

struct MyApp : public App {
          
  //a time counter
  float time;
  
  MyApp() : App(30,20,80), time(0) { 
    init();
  }   
  
  //Initialize
  void init(){}  
  
  //Update the Geometry
  void update(){   
    
    //increment time                       
    time += .05;      

  }
  
    //DRAW GEOMETRY TO SCREEN  
  void onDraw(){ 
  
  
    int num = 10;  
    for (int i = 0; i < num; ++i){   
      
      float t = (float)i/num;
            
      Vec vec( sin(time) * width()/6.0, 0, 0 );
      
      //Create a complex operator
      Bst bst = Gen::bst( Par( Tnv(1,0,0) ).trs( vec )  * t ) ;
          
      //Apply operator to a circle
      Cir cir = CXY(5).sp(bst);  
      
      //      name, r, g, b
      DRAWRGB( cir, 0,1,0 ); 
      //DRAWRGB( vec, 1,1,0 );
    }

    DRAWRGB( CXY(5).trs( Vec(1,0,0) * sin(time) ),1,0,0 );
     
  } 
  
}; 

STARTANDRUN()
