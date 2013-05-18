 

#include "ctl_app.h"
#include "ctl_gl_mbo.h"

using namespace std;
using namespace ctl;

struct MyApp : App {
        

	MBO 	* circles;   	  //<-- Circle Objects
	Vec3f 	* touches;        //<-- Vec3f touch posiitons
	
	int num;   					

    MyApp() : App(14. + 3./8., 10. + 11./16.) {
	
		init();
	}

	virtual void init(){
		cout << "init buffer objects" << endl;
		 
		num = 10;  
		touches = new Vec3f[num];
		circles = new MBO[num];   
		for (int i = 0; i < num; ++i){                
			double t = TWOPI * i/num;        
			
		    touches[i] = Vec3f( sin(t), cos(t), 0);     //<-- Default Positions Init  
			circles[i] = MBO( Mesh::Circle() );    		//<-- Circle Buffers Init  

		}
		      
	} 
	
	//moves circles to values stored in touches.
	void updateMeshes(){
		for (int i = 0; i < num; ++i){
			circles[i].mesh.moveTo( touches[i] );  //<-- Update Mesh Vertices
			circles[i].update();				   //<-- Update Buffer
		}  
		
	}
	
	virtual void onDraw(){
	   // cout << "onDraw()" << endl;  
	
	    updateMeshes();
		pipe.bind( scene.xf ); 
  	        
			for (int i = 0; i < num; ++i){
				pipe.line( circles[i] );
			}
			
	    pipe.unbind( );
	}  
	
	void onTouch(){
		// update touches[i] here 
		for (int i = 0; i < num; ++i) {
		   // touches[i] = 
		}
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
		usleep(16666);
	}   
	
	  return 0; 
}
