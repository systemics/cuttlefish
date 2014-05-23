#include "ctl_app.h"
 
using namespace ctl;

struct MyApp : public App {
   
	MBO * mboR;
	MBO * mboC;
  
	MyApp() : App(21.5,14.5){
		
		init();
		
	}

   virtual void init() {          
	
		mboR = new MBO( Mesh::Num(identifier.row).translate(-1,0,0) );
		mboC = new MBO( Mesh::Num(identifier.col).translate(1,0,0) );   
	
	
	}
   
	virtual void update() {

	}  
	
   virtual void onDraw() {
	    
	
		pipe.line( *mboR );
		pipe.line( *mboC );   
	
   }
 
};

STARTANDRUN()