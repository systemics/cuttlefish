#ifndef CTL_OSC_H_INCLUDED
#define CTL_OSC_H_INCLUDED

#include <iostream>
#include <lo/lo.h>       
#include <vector>

#include "ctl_bcm.h"  
//#include "gfx/gfx_matrix.h"
                 

using namespace std;

namespace ctl {
	
	struct OSCPacketHandler {
	     
		lo_server_thread st;   
		
		OSCPacketHandler() { startOSC(); }
		                                                                                                          
		//LIBLO MESSAGE SIGNATURE
		typedef int (*CB)(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data); 
		
	   //  vector<CB> callback;
	
	   //  virtual void onMessage(){} 

		void addListener( CB cb, string name, string types, void * ud) {
		 //   callback.push_back( cb );   
		    lo_server_thread_add_method( st, name.c_str(), types.c_str(), *cb, ud); 
		}
		
		void startOSC(){
			
		
	       st = lo_server_thread_new("8082", 0); 

		   lo_server_thread_start(st); 
		  
		}
	  
	};
}


#endif