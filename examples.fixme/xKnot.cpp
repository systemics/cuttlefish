/*
 * =====================================================================================
 *
 *       Filename:  xKnot.cpp
 *
 *    Description:  knot demo
 *
 *        Version:  1.0
 *        Created:  05/30/2014 15:08:10
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
#include "vsr/vsr_cga3D_op.h"
#include "vsr/vsr_render.h"
#include "vsr/vsr_knot.h"


using namespace std;
using namespace ctl; 
using namespace vsr;
using namespace vsr::cga3D;

struct MyApp : public App {
    

	float time;

  TorusKnot tk;

  int pRes;

  MBO * mbo;
	  
	MyApp() : App(30,20,80), 
  time(0), 
  tk(3,2),
  pRes(100)
  { 
		init();
	}
	
	//Initialize Geometry 
  void init(){
      mbo = new MBO( Mesh::Contour(pRes).color(1,1,0).mode(GL::LS));
	}  
   
 	//Move Geometry Around
	void update(){                          		
		time+=.05;  

    Point p = Ro::null(2,0,0);

    Point np = p;
    Bst b = tk.bst();
    tk.calc();
    int iter = tk.iter();
    int n = 0;
    n = n+1 % tk.iter();

    /* for (int i=0;i<iter;++i){ */
    /*   np = Ro::loc( np.sp( b ) ); */
    /* } */
    for (int j=0;j<pRes;++j){
      mbo -> mesh[i].Pos.set( 
    }


    /* for (int i=0;i<pRes;++i){ */ 
    /*   float t = (float)i/pRes * TWOPI; */
    /*   mbo -> mesh[i].Pos = Vec3f(sin(t) * width()/10, cos(t) * height()/10, 0); */
    /* } */
    mbo -> update();
        
	}
	
  //DRAW GEOMETRY TO SCREEN  
  void onDraw(){         
      update(); 
      pipe.line(*mbo);

  } 
	
}; 

STARTANDRUN()

