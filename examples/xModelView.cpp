#include "ctl_app.h"
#include "vsr/vsr_cga3D_frame.h"
#include "vsr/vsr_render.h"

using namespace std;
using namespace ctl; 
using namespace vsr;

struct MyApp : public App {
          
    //a time counter
  float time; 
  
  Frame cam; 
  Frame mod;
  
  MyApp() : App(21.5, 14.5, 20), time(0) { 
    init();
  }   
  
  //Initialize
  void init(){}  
  
  //Update the Geometry
  void update(){   
    
    //increment time                       
    time += .05;      
        
    cam.pos( 0, 0, 20);
    cam.rot() = Gen::rot( Biv(0,0,1) * sin(time) * .1 );        
  
    mod.rot() = Gen::rot( Biv(0,1,0) * sin(time) * PI );  
     
    scene.camera.set( cam );
    scene.model.set( mod ); 
      
  }
  
    //DRAW GEOMETRY TO SCREEN  
  void onDraw(){ 
     
    Cir cir = CXY(1);
    
    DRAW(cir);   
      
  } 
  
}; 

STARTANDRUN()
