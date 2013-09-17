#include "cf_app.h"
#include "vsr/vsr.h"
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
		Pnt pb = Ro::null(0,1,0);
		Pnt pc = Ro::null(0,-1,0).trs( Vec(0, sin(time), 0) ); 
		
		Cir c = pa ^ pb ^ pc; 
	    
		//Cir c = CXY(5).trs(Vec( sin(time), 0, 0));
		Dll d = DLN(0,1,0).trs( Vec(5,0,0)).rot( Biv::xy * 2 * time);   
		
		DRAW(pa); DRAW(pb); DRAW(pc);
		DRAWCOLOR(c,1,0,0);
		DRAWCOLOR(d,0,1,0);           		
		DRAWCOLOR( Cir(d.dual()).re(c), 0,0,1);

	} 
	
}; 

STARTANDRUN()

