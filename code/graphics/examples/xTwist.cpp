#include "cf_app.h"
#include "vsr/vsr.h" 

#include "vsr/vsr_twist.h"

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
		time += .05;      
		
	}
	
	void onDraw(){	              
		 
		Dll ln = DLN(0,1,0).rot( Biv::xy * time );
		//
		Dll twist = Twist::Along( ln, PI * sin(time), 1 );
		
		Cir c = CXY(1).trs( Vec(sin(time)*10, 0, 0) );
		//
		DRAWCOLOR(ln,1,0,0);
		
		//100 iterations 
		for (int i = 0; i < 10; ++i){      
			 
			float t = 1.0 * i/10.0;   ///<-- This value "t" will go from 0 to 1 as i increases from 0 to 100
		   
		 	// Twist a Circle around the line
	    	Cir cir = c.sp( Gen::mot( twist * t ) );		
		   
		 	DRAWCOLOR( cir, 0,1,0 );
		
		}
		  
	} 
	
}; 

STARTANDRUN()