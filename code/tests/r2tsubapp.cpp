

#include "ctl_render.h"
#include "ctl_appR2T.h"  

using namespace std;
using namespace ctl;


struct MyApp : AppR2T {
   
	MBO * circle;

	MyApp() : 
	AppR2T( 14. + 3./8., 10. + 11./16. )
	{   
		
		circle = new MBO( Mesh::Circle() );
	}   
	
	~MyApp(){}     
	
	virtual void drawScene(){
		pipe.line(*circle);
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