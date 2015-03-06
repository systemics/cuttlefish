#include "ctl_app.h"
#include "gfx/gfx_mbo.h"

#include "vsr/vsr_products.h" 
#include "vsr/vsr_cga3D_op.h"
#include "vsr/vsr_render.h"


using namespace std;
using namespace ctl; 
using namespace vsr;
using namespace vsr::cga3D;

struct MyApp : public App {
    
	//Some Geometric Elements
	Circle cir;       //A Circle
	Vec vec;          //A Vector
	DualPlane dlp;    //A Plane
	Pair par;         //A Point Pair
	Line lin;         //A Line
	
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
		cir = CXY(5);
		dlp = Dlp(0,1,0,0).trs( Vec(0, sin(time)*2.0,0) );
	}
	
  //DRAW GEOMETRY TO SCREEN  
  void onDraw(){         

    auto a = (cir.dual() ^ dlp).dual();
    auto b = a ^ Inf(1); 

    DRAWRGB( cir, 0,1,0 );
    DRAWRGB( dlp, 0,1,1 );
    DRAWRGB( a, 1,1,0 );
    DRAWRGB( b, 1,0,1 );  

  } 
	
}; 

STARTANDRUN()

