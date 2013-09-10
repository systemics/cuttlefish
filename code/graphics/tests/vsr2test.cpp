#include "cf_app.h"
#include "vsr/vsr_products.h" 
#include "vsr/vsr_render.h"
#include "gfx/gfx_mbo.h"

using namespace std;
using namespace ctl; 
using namespace vsr;

struct MyApp : public App {
        
    //THIS IS THE APP SPECIFIC STUFF, 
	//MBO * circle;  
	
	MyApp() : App(19. + 3./8., 10. + 11./16.){ 
		init();
	}
	 
	virtual void init(){
		//circle = new MBO( Mesh::Circle() );
	}  
	
	
	//YOUR CODE HERE!!!!!
	virtual void onDraw(){ 
	   
	   //Cir c = CXY(1);
       pipe.bind( scene.xf );  

			pipe.line( *circle );
	   // Render( c, scene.xf.modelViewMatrixf(), pipe );
	
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

