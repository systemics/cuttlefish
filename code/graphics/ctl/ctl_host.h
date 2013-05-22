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
	
	
  struct Host {   
	
	   map<string, int> idOf;
	   int identifier; 

		Host() {
		
		  int n = 1;
		  // idof["b8:27:eb:84:b7:37"] = n++;
		  // idof["b8:27:eb:a8:87:0b"] = n++;
		  // idof["b8:27:eb:7d:63:d7"] = n++;
		  // idof["b8:27:eb:a6:9f:1a"] = n++;   

		  idOf["b8:27:eb:4c:d3:3d"] = n++; // pi-z    1
		  idOf["b8:27:eb:96:79:67"] = n++; // pi-t    2
		  idOf["b8:27:eb:be:e5:83"] = n++; // pi-m    3
		  idOf["b8:27:eb:69:2d:6c"] = n++; // pi-c    4
		  idOf["b8:27:eb:7f:74:ff"] = n++; // pi-b    5

		  ifstream foo("/sys/class/net/eth0/address");
		  char mac[256];
		  foo.getline(mac, 256); 
		
		  identifier = idOf[mac];  
		  cout << "MAC ADDRESS: " << mac << " HOST IDENTIFIER #: " << identifier << endl;
		}
	};
} //ctl::

#endif
