#include "cf_app.h"
#include "gfx/gfx_mbo.h"

using namespace std;
using namespace ctl; 


struct MyApp : public App {
        
    //THIS IS THE APP SPECIFIC STUFF, 
	MBO * circle;  
	
	//INSTANTIATE THE APPLICATION WITH WIDTH AND HEIGHT
	MyApp() : App(30, 20){ 
		init();
	}                                  
	
	//INIITIALIZE ELEMENTS OF THE SCENE 
	virtual void init(){
		circle = new MBO( Mesh::Circle() );
	}  
	
	//THIS CHANGES THE POSITION OF THE CIRCLE
	void update(){
	   
		static float time = 0; time +=.01;
		
		circle -> mesh.moveTo( sin(time) * width/2.0, 0, 0 );   
		circle -> mesh.color( 0, sin(time), 1 );
		circle -> update();                                   
		
		background.set( fabs( sin(time) ) * .2, fabs( sin(time) ) * .5, 0, 1 );  
	 
	}

	//THIS DRAWS THE CIRCLE TO THE SCREEN
	virtual void onDraw(){ 
		
	   update();
	   
		
       pipe.bind( scene.xf );  

			pipe.line( *circle );
	
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

