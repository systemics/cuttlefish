#include "ctl_app.h"
#include "vsr/vsr_products.h" 
#include "vsr/vsr_cga3D_op.h"
//#include "vsr/vsr_cga3D_funcs.h"
#include "vsr/vsr_render.h"
#include "gfx/gfx_mbo.h"

using namespace std;
using namespace ctl; 
using namespace vsr;
using namespace vsr::cga3D;

struct MyApp : public App {
    
	//Some Geometric Elements
	Cir cir;    //A Circle
	Vec vec;    //A Vector
	Dlp dlp;    //A Plane
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
		cir = CXY(5);//.rot( Biv::xz * time );
		dlp = Dlp(0,1,0,0).trs( Vec(0, sin(time)*2.0,0) );
		//vec = vec.rot( Biv::xy * .1 ); 
		
		//scene.model.rot() = Quat(sin(time)*.2, Vec(1,0,0) );
	}
	

	//DRAW GEOMETRY TO SCREEN  

    void onDraw(){         
	
		auto a = (cir.dual() ^ dlp).dual();
		//cir.print();
    auto b = a ^ Inf(1); 
		//b.print();
		//		  name, r, g, b
		DRAWCOLOR( cir, 0,1,0 );
	//	DRAWCOLOR( vec, 0,0,1 );  		
		DRAWCOLOR( dlp, 0,1,1 );
		DRAWCOLOR( a, 1,1,0 );
	  DRAWCOLOR( b, 1,0,1 );  

	} 
	
}; 

STARTANDRUN()

