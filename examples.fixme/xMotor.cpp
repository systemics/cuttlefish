#include "ctl_app.h"
#include "vsr/vsr_cga3D_op.h" 
#include "vsr/vsr_render.h"

using namespace std;
using namespace ctl; 
using namespace vsr;

struct MyApp : public App {
    
  float time;  
  
  MyApp() : App(30,20,80), time(0) { 
    init();
  }   
  
  //Initialize
  void init(){}  
  
  //Update the Geometry
  void update(){   
    
    //increment time                       
    time += .02;      
    
  }
  
  void onDraw(){                
    
    // Dual Lines
    DualLine a = DLN(0,1,0).sp( Gen::trs(8,0,0) * Gen::rot( Biv::yz * sin(time) ) );
    DualLine b = DLN(0,0,1).sp( Gen::trs(-8,0,0) * Gen::rot( Biv::xz * sin(time) ) );
 
    DRAWRGB(a, 1,1,0);
    DRAWRGB(b, 0,1,1);
        
    for (int i = 0; i < 10; ++i){      
   
      float t = 1.0 * i/10.0;   
   
       Mot m = Gen::ratio(a, b, t ); 
   
       DRAWRGB( a.sp(m), 1-t,1,t );

    }   
      
  } 
  
}; 

STARTANDRUN()
