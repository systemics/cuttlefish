#include "cf_app.h"
#include "vsr/vsr_products.h" 
#include "vsr/vsr_op.h"
#include "vsr/vsr_render.h"
#include "gfx/gfx_mbo.h"

using namespace std;
using namespace ctl; 
using namespace vsr;

struct MyApp : public App {
    
	//Some Geometric Elements
	Cir cir;    //A Circle
	Vec vec;    //A Vector
	Dlp dlp;    //A Dual Plane
	Par par;    //A Point Pair
	Lin lin;    //A Line
	
	float time;
	  
	MyApp() : App(30,20,80), time(0){ 
		init();
	}
	
	//Initialize Geometry 
    void init(){
		cir = CXY(1);
		dlp = Dlp(0,1,0,0);
		vec = Vec(1,0,0);
	}  
   
 	//Move Geometry Around
	void update(){                          
		
		time+=.05;      
		
		//a rotating circle
		cir = CXY(5).sp( Gen::rot( Biv::xz * time) );
		dlp = Dlp(0,1,0,0).sp( Gen::trs(0, sin(time)*height/4.0,0) );
		vec = vec.sp( Gen::rot( Biv::xy * .1) ); 
		
		par = (cir.dual() ^ dlp).dual();
		lin = par ^ Inf(1);
	}
	

	//DRAW GEOMETRY TO SCREEN  

    void onDraw(){
		
		//		  name, r, g, b
		DRAWCOLOR( cir, 0,1,0 );
		DRAWCOLOR( vec, 0,0,1 );  		
		DRAWCOLOR( dlp, 0,1,1 );
		DRAWCOLOR( par, 1,1,0 );
		DRAWCOLOR( lin, 1,0,1 );  

	} 
	
}; 

STARTANDRUN()

