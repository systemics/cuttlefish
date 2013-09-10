#include "cf_app.h"
#include "vsr/vsr_products.h" 
#include "vsr/vsr_op.h"
#include "vsr/vsr_render.h"
#include "gfx/gfx_mbo.h"

using namespace std;
using namespace ctl; 
using namespace vsr;

struct MyApp : public App {
    
	Cir cir;
	Vec vec;
	Dlp dlp; 
	Par par;
	Lin lin;
    //THIS IS THE APP SPECIFIC STUFF,    
	MyApp() : App(30,20,40){ 
		init();
	}
	 
	virtual void init(){
		cir = CXY(1);
		dlp = Dlp(0,1,0,0);
		vec = Vec(1,0,0);
	}  
	
	void update(){                          
		static float time = 0; time+=.05;      
		
		cir = CXY(1).sp( Gen::rot( Biv::xz * time) );//.sp( Gen::trs( sin(time)* width/2.0,0,0) ) ;// 
		dlp = Dlp(0,1,0,0).sp( Gen::trs(0, sin(time)*height/4.0,0) );
		vec = vec.sp( Gen::rot( Biv::xy * .1) ); 
		
		par = (cir.dual() ^ dlp).dual();
		lin = par ^ Inf(1);
	}
	
	//YOUR CODE HERE!!!!!
	virtual void onDraw(){
		
	   update();
	   
	   Mat4f m = scene.xf.modelViewMatrixf();
       pipe.bind( scene.xf );  

		    Render( cir, m, pipe, 0, 1, 0, 1 );
            Render( vec, m, pipe ); 
			Render( dlp, m, pipe, 0, 0, 1, 1 ); 
			Render( par, m, pipe, 1, 1, 0, 1 );
			Render( lin, m, pipe, 0, 0, 1, 1 );

	   pipe.unbind();
	} 
	
}; 

bool running = true;
void quit(int) {
  running = false;
}


int main(){
	
	MyApp app; 
  
	while(running){
		app.onFrame();
		usleep(166);
	}   
	
	  return 0; 
}

