#include "cf_app.h"
#include "vsr/vsr.h"

#include "vsr/vsr_frame.h"

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
		
		for (int i = 0; i < 10; ++i){   
			
			float t = 1.0 * i/10.0;
            
			Vec vec( sin(time) * width/6.0, 0, 0 );
			
			//Create a complex operator
			Bst bst = Gen::bst( Par( Tnv(1,0,0) ).sp( Gen::trs( vec ) ) * t );
			    
			//Apply operator to a circle
			Cir cir = CXY(5).sp(bst);  
			
			//		  name, r, g, b
			DRAWCOLOR( cir, 0,1,0 ); 
			DRAWCOLOR( vec, 1,1,0 );
		}
		 
	} 
	
}; 

STARTANDRUN()