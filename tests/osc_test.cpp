#include <iostream>
#include <lo/lo.h>

#include "ctl_bcm.h"  
#include "vsr_matrix.h" 

#include "ctl_osc.h"  

//#include "ctl_app.h"

using namespace std;
using namespace ctl; 
using vsr::Vec3f;               

int running = true;
void quit(){
	running = false;
} 

struct AppParameters{
   
	float traceAmt;
	Vec3f pos;
};  



struct OSCApp : OSCPacketHandler {
    
	AppParameters param;
	
	OSCApp() : OSCPacketHandler() { initOSC(); }
	
	void initOSC(){
		addListener( GetTrace, "/trace", "f", this);
	}

    static int GetTrace(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {

		OSCApp& app = *(OSCApp*)user_data;  
		
		app.param.traceAmt =  argv[0]->f;  
		cout << app.param.traceAmt << endl;
		
	    return 0;
	}

	
};

int main() {
  
	BCM bcm;
 
	//OSCPacketHandler osc;
	OSCApp osc;
	
  //  osc.addListener(OSCApp::GetTrace,"/trace","f", &osc);
    
	while(running){
		usleep(1666);
	}
       
	return 0;  
}