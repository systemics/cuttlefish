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
		   /* idOf["b8:27:eb:28:63:d4"] = RC{0,0}; */
		   /* idOf["b8:27:eb:ef:8d:6b"] = RC{0,1}; */
		   /* idOf["b8:27:eb:0c:87:eb"] = RC{0,2}; */
		   /* idOf["b8:27:eb:cd:48:0c"] = RC{0,3}; */
		   /* idOf["b8:27:eb:65:d2:e6"] = RC{1,0}; */  
		   /* idOf["b8:27:eb:48:b9:b2"] = RC{1,1}; */

       // ELINGS HALL 4x4

     idOf["b8:27:eb:7f:74:ff"] = RC{0,0};// 192.168.7.3
     idOf["b8:27:eb:69:2d:6c"] = RC{0,1};// 192.168.7.4
     idOf["b8:27:eb:be:e5:83"] = RC{0,2};// 192.168.7.5
     idOf["b8:27:eb:96:79:67"] = RC{0,3};// 192.168.7.6
     idOf["b8:27:eb:42:be:e9"] = RC{1,0};// 192.168.7.7
     idOf["b8:27:eb:4a:64:ea"] = RC{1,1};// 192.168.7.8
     idOf["b8:27:eb:a9:4d:9a"] = RC{1,2};// 192.168.7.9
     idOf["b8:27:eb:7e:d9:a5"] = RC{1,3};// 192.168.7.10
     idOf["b8:27:eb:23:fc:58"] = RC{2,0};// 192.168.7.11
     idOf["b8:27:eb:0b:5a:76"] = RC{2,1};// 192.168.7.12
     idOf["b8:27:eb:ac:a2:2d"] = RC{2,2};// 192.168.7.13
     idOf["b8:27:eb:d5:b6:67"] = RC{2,3};// 192.168.7.14
     idOf["b8:27:eb:30:56:91"] = RC{3,0};// 192.168.7.15
     idOf["b8:27:eb:99:e7:4e"] = RC{3,1};// 192.168.7.16
     idOf["b8:27:eb:a5:df:df"] = RC{3,2};// 192.168.7.17
     idOf["b8:27:eb:a9:33:8f"] = RC{3,3};// 192.168.7.18
		         
		
		  ifstream foo("/sys/class/net/eth0/address");
		  char mac[256];
		  foo.getline(mac, 256); 
		
		  identifier = idOf[mac];  
		  printf("HOST MAC ADDRESS: %s HOST IDENTIFIER #: %d, %d\n", mac, identifier.row, identifier.col);
		}
	};
} //ctl::

#endif
