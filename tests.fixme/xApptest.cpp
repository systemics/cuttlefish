/*
 * =====================================================================================
 *
 *       Filename:  xApptest.cpp
 *
 *    Description: test of new app class 
 *
 *        Version:  1.0
 *        Created:  02/17/2014 18:57:35
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */

#include "ctl_apptest.h"

using namespace std;
using namespace ctl;


struct MyApp : public App {
        
  //THIS IS THE APP SPECIFIC STUFF 
	MBO * circle;  
	
	//INSTANTIATE THE APPLICATION WITH WIDTH AND HEIGHT
	MyApp() : App(30, 20) { 
    
    process = new gfx::MotionTrace( this->contextWidth, this->contextHeight, this);    
    init();
	}                                  
	
  virtual void doProcess(){
    (*process)();
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

        
STARTANDRUN()


