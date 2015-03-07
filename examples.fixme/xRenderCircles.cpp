/*
 * =====================================================================================
 *
 *       Filename:  xRenderCircles.cpp
 *
 *    Description:  draw a bunch of circles
 *
 *        Version:  1.0
 *        Created:  06/02/2014 16:49:29
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

#include "vsr/vsr_cga2D_types.h"


using namespace std;
using namespace ctl;

struct MyApp : public App {

  // THIS IS THE APP SPECIFIC STUFF,
  MBO * circle;

  int numCircles;

  float time;


  // INSTANTIATE THE SINGLE MONITOR APPLICATION WITH WIDTH AND HEIGHT IN INCHES
  MyApp() : App((Layout(4,4))), time(0), numCircles(10) {

    init();
  }


  // INIITIALIZE ELEMENTS OF THE SCENE
  virtual void init() {
      
    circle = new MBO( Mesh::Circle() );

  }

  // THIS CHANGES vertex data
  void update() {

    time += .01;


  }

  // THIS DRAWS THE CIRCLE TO THE SCREEN
  virtual void onDraw() { 
  
      float tw = layout.totalWidth();
      float th = layout.totalHeight();

      pipe.begin(*circle);    
      for (int i = 0; i < numCircles; ++i){
        for (int j = 0; j< numCircles;++j){
            
             float scale = (float)(i*numCircles +j)/(numCircles*numCircles);
             Vec2f pos( -tw/2.0 + tw * (float)i/numCircles, -th/2.0 + th * (float)j/numCircles);
             modelview( gfx::XMat::translateScale2D( pos, scale) );
             pipe.draw(*circle); 

        }
      }
      pipe.end(*circle);
   }
};

STARTANDRUN()

