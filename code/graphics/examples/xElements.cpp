#include "cf_app.h"
#include "vsr/vsr.h"
//#include "gfx/gfx_mbo.h"

using namespace std;
using namespace ctl; 
using namespace vsr;

struct MyApp : public App {     
	        
    //a time element
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

STARTANDRUN()