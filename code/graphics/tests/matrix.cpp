
//matrix tests
#include "vsr.h"
#include "vsr_matrix.h"
#include "vsr_xf.h"


#include <iostream>

using namespace std;
using namespace vsr;

int main() {
	
	cout << Xf::mat( Vec(1,1,1), 1  ) * Xf::mat( Vec(0,0,0), .5  ) << endl; 

}