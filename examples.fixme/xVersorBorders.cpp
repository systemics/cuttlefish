/*
 * =====================================================================================
 *
 *       Filename:  xVersorBorders.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/02/2014 16:10:14
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include "ctl_app.h"
#include "gfx/gfx_mbo.h"

#include "vsr/vsr_products.h" 
#include "vsr/vsr_cga2D_op.h"
#include "vsr/vsr_cga2D_render.h"


using namespace std;
using namespace ctl; 
using namespace vsr;
using namespace vsr::cga2D;

struct MyApp : public App {
    
	//Some Geometric Elements
	float time;
	  
	MyApp() : App(30,20,80), time(0){ 
		init();
	}
	
	//Initialize Geometry 
  void init(){

	}  
   
 	//Move Geometry Around
	void update(){                          		
		time+=.05;      

	}
	
  //DRAW GEOMETRY TO SCREEN  
  void onDraw(){         

  } 
	
}; 

STARTANDRUN()

