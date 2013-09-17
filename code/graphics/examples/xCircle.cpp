#include "ctl_app.h"
#include "vsr/vsr_products.h" 
#include "vsr/vsr_op.h"
#include "vsr/vsr_render.h"
//#include "gfx/gfx_mbo.h"

using namespace std;
using namespace ctl; 
using namespace vsr;

struct MyApp : public App {
    
    //A Circle 
	Cir cir;        
	        
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
		   	
		//name =  	circle in xy plane with radius 5; that is spun ( around  ( the xz plane ) );
		cir 	= 		CXY(5).sp ( Gen::rot( Biv::xz * time) );
		
	}
	
    //DRAW GEOMETRY TO SCREEN  
	void onDraw(){	
		//		  name, r, g, b
		DRAWCOLOR( cir, 0,1,0 );  
	} 
	
}; 

STARTANDRUN() 