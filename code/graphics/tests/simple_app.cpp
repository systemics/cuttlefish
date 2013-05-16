 

#include "ctl_app.h"
#include "ctl_gl_mbo.h"

using namespace std;
using namespace ctl;

struct MyApp : App {
        

	MBO * circle;

    MyApp() : App(14. + 3./8., 10. + 11./16.) {
	
		init();
	}

	virtual void init(){
		cout << "init buffer objects" << endl;
		
		circle = new MBO( Mesh::Circle() );
		
	}
	
	virtual void onDraw(){
	   // cout << "onDraw()" << endl; 
		pipe.bind( scene.xf ); 
  	
			pipe.line( *circle );
			
	    pipe.unbind( );
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