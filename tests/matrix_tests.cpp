  #include "vsr_matrix.h"
 
#include <iostream>
#include "ctl_bcm.h" 

using namespace std;
using namespace vsr;

int main() { 
	 
    bcm_host_init();
	
	 Mat4f scale ( .2, 0, 0, 0,
	 				 0 , .2, 0, 0, 
	 				 0, 0, .2, 0, 
	 				0, 0, 0, 1 );   
 
	 Mat4f trans ( 1, 0, 0, 0,
					 0 , 1, 0, 0, 
					 0, 0, 1, 0, 
					2, 2, 2, 1 );  
			
	cout << "A: " << trans * scale << endl;  
	cout << "B: " << scale * trans << endl;   
	
   

 	 while(1){
		usleep(16666);
	}  
	
   //
 bcm_host_deinit();
	return 0;
}