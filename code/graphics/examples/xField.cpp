#include "cf_app.h"
#include "vsr/vsr_products.h" 
#include "vsr/vsr_op.h"
#include "vsr/vsr_render.h"
#include "gfx/gfx_mbo.h"

using namespace std;
using namespace ctl; 
using namespace vsr;

struct MyApp : public App {
    
	Field<Vec> f;
    //THIS IS THE APP SPECIFIC STUFF,    
	MyApp() : App(30,20,40), f(10,10,10){ 
		init();
	}
	 
	virtual void init(){
		cir = CXY(1);
		dlp = Dlp(0,1,0,0);
		vec = Vec(1,0,0);
	}  
	
	void update(){                          
		static float time = 0; time+=.05;      
		
		for (int i =0; i < f.num(); ++i){
			
		}
	}
	
	//YOUR CODE HERE!!!!!
	virtual void onDraw(){
		
	   update();
	   
	   Mat4f m = scene.xf.modelViewMatrixf();
       pipe.bind( scene.xf );  

		    Render( f, m, pipe, 0, 1, 0, 1 );

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

