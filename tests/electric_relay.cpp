#include "ctl_osc.h"


using namespace std;
using namespace ctl;

struct OscRelay : OSCPacketHandler {  
	
	float traceAmt, ux, uy, bluramt, vel, cam;
     
   lo_address t, m, b, c;  

	OscRelay() : OSCPacketHandler() {
	    
	    t = lo_address_new("pi-t.mat.ucsb.edu", "8082");
	    m = lo_address_new("pi-m.mat.ucsb.edu", "8082");
	    b = lo_address_new("pi-b.mat.ucsb.edu", "8082");
	    c = lo_address_new("pi-c.mat.ucsb.edu", "8082");
	
		addListener( GetTrace, "/trace", "f", this);
	 	addListener( GetUX,"/ux","f", this);  
	 	addListener( GetUY,"/uy","f", this);  
	   	addListener( GetBlur,"/bluramt","f", this);
	    addListener(GetVelocity, "/vel", "f", this); 
		addListener(GetCam, "/cam", "f", this);    
	}
  
  	void resendTrace(){
	    lo_send(t, "/trace", "f", traceAmt);    
	    lo_send(m, "/trace", "f", traceAmt);  
		lo_send(b, "/trace", "f", traceAmt);  
		lo_send(c, "/trace", "f", traceAmt);  
	}  

  	void resendVel(){
	    lo_send(t, "/vel", "f", vel);    
	    lo_send(m, "/vel", "f", vel);  
		lo_send(b, "/vel", "f", vel);  
		lo_send(c, "/vel", "f", vel);  
	}
	
   	void resendUX(){
	    lo_send(t, "/ux", "f", ux);    
	    lo_send(m, "/ux", "f", ux);  
		lo_send(b, "/ux", "f", ux);  
		lo_send(c, "/ux", "f", ux);  
	}   
 
  	void resendUY(){
	    lo_send(t, "/uy", "f", uy);    
	    lo_send(m, "/uy", "f", uy);  
		lo_send(b, "/uy", "f", uy);  
		lo_send(c, "/uy", "f", uy);  
	}

  	void resendBlur(){
	    lo_send(t, "/bluramt", "f", bluramt);    
	    lo_send(m, "/bluramt", "f", bluramt);  
		lo_send(b, "/bluramt", "f", bluramt);  
		lo_send(c, "/bluramt", "f", bluramt);  
	} 
	
	void resendCam(){
	    lo_send(t, "/cam", "f", cam);    
	    lo_send(m, "/cam", "f", cam);  
		lo_send(b, "/cam", "f", cam);  
		lo_send(c, "/cam", "f", cam);  
	}  
	
	static int GetTrace(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {

		OscRelay& app = *(OscRelay*)user_data;  

		app.traceAmt =  argv[0]->f;  
		cout << app.traceAmt << endl;

		app.resendTrace();
		return 0;
	}  

	static int GetUX(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {

		OscRelay& app = *(OscRelay*)user_data;  

		app.ux =  argv[0]->f;  
		cout << app.ux << endl;
         
		app.resendUX();
	    return 0;
	}

	static int GetUY(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {

		OscRelay& app = *(OscRelay*)user_data;  

		app.uy =  argv[0]->f;  
		cout << app.uy << endl;
		app.resendUY();
	    return 0;
	} 

 	static int GetBlur(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {

		OscRelay& app = *(OscRelay*)user_data;  

		app.bluramt =  argv[0]->f;  
		cout << app.bluramt << endl;
         
		app.resendBlur();
	    return 0;
	} 
	
	static int GetVelocity( const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {
	
		OscRelay& app = *(OscRelay*)user_data;    
		
		app.vel = argv[0] -> f;
		cout << app.vel << endl;
		
		app.resendVel();
		return 0;
	}

	static int GetCam( const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {
	
		OscRelay& app = *(OscRelay*)user_data;    
		
		app.cam = argv[0] -> f;
		cout << app.cam << endl;
		
		app.resendCam();
		return 0;
	}	
 
};


bool running = true;
void quit(int) {
  running = false;
}

int main(){
	
	OscRelay osc;
	
	while(running){
		sleep(1);
	}
	return 0;
}