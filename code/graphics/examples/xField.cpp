#include "cf_app.h"
#include "vsr/vsr_products.h" 
#include "vsr/vsr_op.h"
#include "vsr/vsr_render.h"
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
	    
		Par p = Tnv(0,0,1) * sin(time);
	
	   	Field<Pnt> f(10,10,1);
	
		for (int i = 0; i < f.num(); ++i){
			f[i] = f.grid(i).bst( p );
		}
	
		DRAW(f);
	 
	} 
	
}; 

STARTANDRUN()
