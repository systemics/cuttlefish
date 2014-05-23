#include "cf_app.h"
#include "vsr/vsr.h" 

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
		
		//
		Dll a = DLN(0,1,0).sp( Gen::trs(8,0,0) * Gen::rot( Biv::yz * sin(time) ) );
		Dll b = DLN(0,0,1).sp(  Gen::trs(-8,0,0) * Gen::rot( Biv::xz * sin(time) ) );
 
		DRAWCOLOR(a, 1,1,0);
		DRAWCOLOR(b, 0,1,1);
		  	
		for (int i = 0; i < 10; ++i){      
	 
			float t = 1.0 * i/10.0;   
 	
        	Mot m = Gen::ratio(a, b, t ); 
   
		 	DRAWCOLOR( a.sp(m), 1-t,1,t );

		}   
		  
	} 
	
}; 

STARTANDRUN()