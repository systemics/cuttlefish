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
		
		//
		Dll twist = Twist::Along( DLN(0,1,0), PI * sin(time), 1 );
		
		//100 iterations 
		for (int i = 0; i < 10; ++i){      
			 
			float t = 1.0 * i/10.0;   ///<-- This value "t" will go from 0 to 1 as i increases from 0 to 100
		   
		 	// Twist a Circle around the line
	    	Cir cir = CXY(1).sp( Gen::mot( twist * t ) );		
		   
		 	DRAWCOLOR( cir, 0,1,0 );
		
		}
		  
	} 
	
}; 

STARTANDRUN()