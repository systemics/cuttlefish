#include "cf_app.h"
#include "gfx/gfx_mbo.h"

using namespace std;
using namespace ctl; 

struct MyApp : public App {
        
    //THIS IS THE APP SPECIFIC STUFF, 
	MBO * mbo;  
	
	float time = 0; 
	
	//INSTANTIATE THE APPLICATION WITH WIDTH AND HEIGHT
	MyApp() : App(30, 20){ 
		init();
	}                                  
	
	//INIITIALIZE ELEMENTS OF THE SCENE 
	virtual void init(){
		
		//Make a Mesh and add vertices
		Mesh mesh;
		mesh.add ( -1, -1, 0);
		mesh.add ( -1, 1, 0);
		mesh.add ( 1, 1,  0);
		mesh.add ( 1, -1, 0);  
		
		//add order vertices should be drawn
		mesh.add(0).add(1).add(2).add(3);
	   
	    // mesh.mode( GL::LS );
				
		mbo = new MBO( mesh );
	}  
	
	//THIS CHANGES THE POSITION OF THE CIRCLE
	void update(){
	   
		time +=.01; 
	     
		mbo -> mesh.moveTo( )
	}

	//THIS DRAWS THE CIRCLE TO THE SCREEN
	virtual void onDraw(){ 
		 
		pipe.line( *mbo );
	
	} 
	
}; 

STARTANDRUN()

