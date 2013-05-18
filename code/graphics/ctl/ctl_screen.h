#ifndef CTL_SCREEN_H_INCLUDED
#define CTL_SCREEN_H_INCLUDED  

#include <iostream>

using namespace std;

namespace ctl {
	
	enum CTL_SCREEN_ID {
	
		A1 = 0,
		A2,
		A3,
		A4
		//...eventually we can use RowColumn indexing 
	}; 
	
	
  struct Screen {   
	
	   map<string, int> idOf;
	   int identifier; 

		Screen(){
		
		  int n = 1;
		  idOf["b8:27:eb:84:b7:37"] = n++;
		  idOf["b8:27:eb:a8:87:0b"] = n++;
		  idOf["b8:27:eb:7d:63:d7"] = n++;
		  idOf["b8:27:eb:a6:9f:1a"] = n++;   

		  ifstream foo("/sys/class/net/eth0/address");
		  char mac[256];
		  foo.getline(mac, 256); 
		
		  identifier = idOf[mac];  
		  cout << "MAC ADDRESS: " << mac << " SCREEN IDENTIFIER #: " << identifier << endl;
		}
	
	
	};
  
	
} //ctl::

#endif
