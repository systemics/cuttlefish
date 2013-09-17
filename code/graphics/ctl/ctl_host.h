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
			
		  // idof["b8:27:eb:84:b7:37"] = n++;
		  // idof["b8:27:eb:a8:87:0b"] = n++;
		  // idof["b8:27:eb:7d:63:d7"] = n++;
		  // idof["b8:27:eb:a6:9f:1a"] = n++;   
          
			//ELINGS HALL
		  // idOf["b8:27:eb:4c:d3:3d"] = n++; // pi-z    1
		  // idOf["b8:27:eb:96:79:67"] = n++; // pi-t    2
		  // idOf["b8:27:eb:be:e5:83"] = n++; // pi-m    3
		  // idOf["b8:27:eb:69:2d:6c"] = n++; // pi-c    4
		  // idOf["b8:27:eb:7f:74:ff"] = n++; // pi-b    5  
		 
		//LJUBLJANA                    numCol
		// idOf["80:1f:02:ae:f4:9f"] = RC{-1,-1};//rc(,numCol); // master.cf
		// idOf["00:0f:13:39:20:07"] = RC{0,0);//rc(0,0,numCol); // 00.cf
		// idOf["00:0f:13:40:15:da"] = RC{0,1);//rc(0,1,numCol); // 01.cf
		// idOf["00:0f:13:40:1b:4e"] = RC{0,2);//rc(0,2,numCol); // 02.cf
		// idOf["80:1f:02:ae:f4:9e"] = RC{0,3);//rc(0,3,numCol); // 03.cf
		// idOf["00:0f:13:40:0c:b1"] = RC{1,0);//rc(1,0,numCol); // 10.cf
		// idOf["80:1f:02:ae:f4:f5"] = RC{1,1);//rc(1,1,numCol); // 11.cf
		// idOf["00:0f:13:39:0a:f6"] = RC{1,2);//rc(1,2,numCol); // 12.cf
		// idOf["00:0f:13:40:17:f7"] = RC{1,3);//rc(1,3,numCol); // 13.cf
		// idOf["00:0f:13:40:01:f5"] = RC{2,0);//rc(2,0,numCol); // 20.cf
		// idOf["00:0f:13:40:10:3a"] = RC{2,1);//rc(2,1,numCol); // 21.cf   
			 
		   idOf["b8:27:eb:ef:8d:6b"] = RC{0,0};
		   idOf["b8:27:eb:0c:87:eb"] = RC{0,1};
		   idOf["b8:27:eb:cd:48:0c"] = RC{0,2};
		   idOf["b8:27:eb:65:d2:e6"] = RC{1,2};
		   idOf["b8:27:eb:48:b9:b2"] = RC{2,1}; 

		  ifstream foo("/sys/class/net/eth0/address");
		  char mac[256];
		  foo.getline(mac, 256); 
		
		  identifier = idOf[mac];  
		  printf("MAC ADDRESS: %s HOST IDENTIFIER #: %d, %d", mac, identifier.row, identifier.col);
		}
	};
} //ctl::

#endif
