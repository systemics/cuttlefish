#ifndef CTL_SCREEN_H_INCLUDED
#define CTL_SCREEN_H_INCLUDED  

#include <stdio.h>
#include <fstream> 
#include <map>

using namespace std;

namespace ctl {
	
   

	
  struct Host {   
	    
	   typedef struct { int row; int col; } RC;
	
	   map<string, RC> idOf;
	   
	   RC identifier;     
		
		//gets unique identifier for nth row and nth column in a grid system
		// int rc(int row, int col){
		// 	return row * numCol + col;
		// }     

		Host() {
			
          
			//ELINGS HALL
		  // idOf["b8:27:eb:4c:d3:3d"] = n++; // pi-z    1
		  // idOf["b8:27:eb:96:79:67"] = n++; // pi-t    2
		  // idOf["b8:27:eb:be:e5:83"] = n++; // pi-m    3
		  // idOf["b8:27:eb:69:2d:6c"] = n++; // pi-c    4
		  // idOf["b8:27:eb:7f:74:ff"] = n++; // pi-b    5  
		 
           //LJUBLJANA +MCAM
		   idOf["b8:27:eb:28:63:d4"] = RC{0,0};
		   idOf["b8:27:eb:ef:8d:6b"] = RC{0,1};
		   idOf["b8:27:eb:0c:87:eb"] = RC{0,2};
		   idOf["b8:27:eb:cd:48:0c"] = RC{0,3};
		   idOf["b8:27:eb:65:d2:e6"] = RC{1,0};  
		   idOf["b8:27:eb:48:b9:b2"] = RC{1,1};
		         
		

		  ifstream foo("/sys/class/net/eth0/address");
		  char mac[256];
		  foo.getline(mac, 256); 
		
		  identifier = idOf[mac];  
		  printf("MAC ADDRESS: %s HOST IDENTIFIER #: %d, %d", mac, identifier.row, identifier.col);
		}
	};
} //ctl::

#endif
