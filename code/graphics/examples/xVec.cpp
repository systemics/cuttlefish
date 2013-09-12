#include "cf_app.h"
#include "vsr/vsr.h"

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
	   
			Vec vec( sin(time) * height/4.0, cos(time) * height/4.0, 0 );
			DRAWCOLOR( vec, 1,1,0 );  
			
			Biv b = Biv::xz.sp( Gen::rot( Biv::xy * time ) ); //A Bivector (plane) Element
			
			Vec rvec = vec.re( b );
			DRAWCOLOR( rvec,0,1,1);   
			
			DRAWCOLOR( b, 1,0,0);   
			
	} 
	
}; 

STARTANDRUN()