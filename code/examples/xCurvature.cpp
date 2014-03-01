#include "cf_app.h"
#include "vsr/vsr.h"

#include "vsr/vsr_frame.h"

using namespace std;
using namespace ctl; 
using namespace vsr;

struct MyApp : public App {
          
    //a time counter
	float time; 
	
	
	MyApp() : App(21.5, 14.5, 20), time(0) { 
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
	   
		Field<Pnt> f(10,10);
		 
			
	} 
	
}; 

STARTANDRUN()