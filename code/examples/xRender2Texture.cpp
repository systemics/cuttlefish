#include "cf_appR2T.h"
#include "vsr/vsr.h" 
#include "vsr/vsr_field.h"
//#include "gfx/gfx_mbo.h"

using namespace std;
using namespace ctl; 
using namespace vsr;

struct MyApp : public AppR2T {     
	  
    //a time element
	float time;
	
	MyApp() : AppR2T(30,20,80), 
		time(0),
		f(10,10,1) 
	
	{ 
		AppR2T::init();
	}	 
	
	//Initialize
	void initMeshes(){}  
	
	//Update the Geometry
	void update(){   
		
		//increment time                       
		time += .05;      
		
	}
	
    //DRAW GEOMETRY TO SCREEN  
	 virtual void drawScene(){	

		
		auto cir = Circle( Vec(0, 0, 1) );
		DRAWCOLOR( cir, 0,1,0 ); 
		        
		auto pa = Point(5,0,0);
		auto pb = Point(-5,0,0);
		
		DRAWCOLOR( pa, 0, 1, 0);
		DRAWCOLOR( pb, 0, 1, 0); 
				
		auto line = Line(pa, pb);
		DRAWCOLOR( line, 1, 1, 0);  
  
		  
	} 
	
}; 

bool running = true;
void quit(int) {
  running = false;
}

int main(){
	
	MyApp app;
	
  while(running){
 		app.onFrame();
 		usleep(1666);
 	}   
	
	  return 0; 
}