#include "cf_app.h"
#include "vsr/vsr.h"

#include "vsr/vsr_frame.h"

using namespace std;
using namespace ctl; 
using namespace vsr;

struct MyApp : public App {
    
    //Two Coordinate Frames 
	Frame frameA, frameB;        
	        
    //a time counter
	float time;
	
	MyApp() : App(30,20,80), time(0) { 
		init();
	}	 
	
	//Initialize
	void init(){
		
		frameA.set(-10,0,0);
		frameB.set(10,0,0);
		
	}  
	
	//Update the Geometry
	void update(){   
		
		//increment time                       
		time += .05; 

		frameA.rot() = Gen::rot( Biv::xz * time ); 		
		frameB.rot() = Gen::rot( Biv::yz * time );     
		
	}
	
    //DRAW GEOMETRY TO SCREEN  
	void onDraw(){	 
		
		//Draw Frames
		DRAW( frameA );  
		DRAW( frameB );  
	    
		//Get Ratio between frame orientations
		Dll dll = Gen::log( frameB.mot() / frameA.mot() );  
		
		//Interpolate Between Frames
		for (int i = 0; i < 20; ++i){ 
			
			float t = 1.0 * i/20.0;

			DRAW( Frame( Gen::mot(dll * t) * frameA.mot() ) );
		} 
	} 
	
}; 

STARTANDRUN()