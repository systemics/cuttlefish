/*
 * =====================================================================================
 *
 *       Filename:  xBasicMotionBlur.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/17/2014 16:55:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include "ctl_appR2T.h"
#include "gfx/gfx_mbo.h"

using namespace std;
using namespace ctl; 


struct MyApp : public AppR2T {
        
    //THIS IS THE APP SPECIFIC STUFF, 
	MBO * circle;  
	
	//INSTANTIATE THE APPLICATION WITH WIDTH AND HEIGHT
	MyApp() : AppR2T(30, 20){ 
		AppR2T::init();
    this -> init();
	}                                  
	
	//INIITIALIZE ELEMENTS OF THE SCENE 
	virtual void init(){
		circle = new MBO( Mesh::Circle() );
	}  
	
	//UPDATE() CHANGES THE POSITION OF THE CIRCLE
	virtual void update(){
	   
		static float time = 0; time +=.01;
		
    //change position of vertices
		circle -> mesh.moveTo( sin(time) * width()/2.0, 0, 0 );   
		circle -> mesh.color( 1, fabs( sin(time) ), 1 );
    
    //send changes to buffer
		circle -> update();                                   
		
    //background color
		background.set( fabs( sin(time) ) * .2, fabs( cos(time) ) * 1, 1, 1 );  
	 
	}

	//THIS DRAWS THE CIRCLE TO THE SCREEN
	virtual void onDraw(){ 
		 
		pipe.line( *circle );
	
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

