#include "cf_app.h"
#include "vsr/vsr.h"

#include "vsr/vsr_twist.h"
#include "gfx/gfx_mbo.h"

using namespace std;
using namespace ctl; 
using namespace vsr;

struct MyApp : public App {    
	
	float time;
	
    //THIS IS THE APP SPECIFIC STUFF,    
	MyApp() : App(30,20,40){ 
		init();
	}
	 
	virtual void init(){

	}  
	
	void update(){                          
		time += .01;
	}
	
	//YOUR CODE HERE!!!!!
	virtual void onDraw(){ 
		
		Pnt pa = Ro::null(1,0,0);
		Pnt pb = Ro::null(0,-1,0).trs( Vec(cos(time), sin(time), 0) );  
		
		Lin lin = pa ^ pb ^ Inf(1); 
		
		Dll dll = Twist::Along( lin.dual(), sin(time), 1 );
		
		DRAW(pa);
		DRAW(pb);
		DRAW(dll);
		
		for (int i = 0; i < 10; ++i){
			float t = 1.0 * i/10;
			DRAWCOLOR( CXY(2).mot( dll * t), 0, t, 1-t );
		}
		
		


	} 
	
}; 

STARTANDRUN()

