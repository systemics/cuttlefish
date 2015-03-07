/*
 * =====================================================================================
 *
 *       Filename:  xElingsHallMonitorLayout.cpp
 *
 *    Description:  wall test using GFXApp only 
 *
 *        Version:  1.0
 *        Created:  02/05/2015 15:41:02
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */

#include "ctl_host.h"
#include "util/egl_window.h"
#include "gfx/gfx_app.h"
#include "gfx/gfx_monitorLayout.h"

using namespace gfx;

//#include "ctl_app.h"
//#include "gfx/gfx_mbo.h"

using namespace std;
using namespace ctl; 


struct MyApp : public GFXApp<RPIContext>, Host {
        
  //THIS IS THE APP SPECIFIC STUFF, 
  MBO * circle;  
  MBO * mboR;
  MBO * mboC;

  Layout layout;

  float timer=0;
  
  //INSTANTIATE THE APPLICATION WITH SCREEN WIDTH AND HEIGHT IN PIXELS
  MyApp(int w, int h) : GFXApp(w,h), layout(4,4, w, h) { 
    
   // cout << "ID: " << identifier.row << " " << identifier.col << endl;
    
    setView(50, true, identifier.row, identifier.col);
    
  }  
  
    //set the frustrum viewport
    void setView(float z, bool isGrid, int row=0, int col=0){
            
            
      float w = layout.screenWidth;
      float h = layout.screenHeight;   
             
     // scene.fit(w,h);

      //bottom left corner of screen in pixels
      Pose p( -w/2.0,-h/2.0, 0);

      layout.speakerL = Vec3f( -w/2.0, 0, 0);
      layout.speakerR = Vec3f( w/2.0, 0, 0);

      //If we're in multi-screen mode, RE-DO pose positions based on grid layout . . .
      if (isGrid) {
        
        p = layout.poseOf( row, col ); 

        layout.speakerL = Vec3f( 
          layout.left( row, col ), 
          layout.bottom( row, col ) + layout.screenHeight / 2.0, 0);

        layout.speakerR = Vec3f(
           layout.left( row, col ) + layout.screenWidth, 
          layout.bottom( row, col ) + layout.screenHeight / 2.0, 0);
       }


      scene.viewpose = p;
      scene.camera.pos() = Vec3f( 0, 0, z); 
      scene.camera.view = gfx::View( scene.camera.pos(), p, (float)w/h, h );

    }                                    
  

    /*-----------------------------------------------------------------------------
     *    //Setup ELEMENTS OF THE SCENE 
     *-----------------------------------------------------------------------------*/
  virtual void setup(){
    circle = new MBO( Mesh::Circle(15) );

		mboR = new MBO( Mesh::Num(identifier.row).translate(-1,0,0).moveTo( layout.speakerL + Vec3f(3,0,0) ) );
		mboC = new MBO( Mesh::Num(identifier.col).translate(1,0,0).moveTo( layout.speakerL + 
                    Vec3f(layout.screenWidth - 3,0,0) ) );  

    
    //background color
   // mColor.set( fabs( sin(timer) ) * .2, fabs( cos(timer) ) * 1, 1, 1 );    
  }  
  

  /*-----------------------------------------------------------------------------
   *   //onAnimate() CHANGES THE POSITION OF THE CIRCLE 
   *-----------------------------------------------------------------------------*/
  virtual void onAnimate(){   
    //change position of vertices
    circle -> mesh.moveTo( sin(timer) * layout.totalWidth()/2.0, cos(timer) * layout.totalHeight()/2.0, 0 );   
    circle -> mesh.color( 1, fabs( sin(timer) ), 1 );
    
    //send changes to buffer
   // circle -> update();  
    
    timer+=.01;                                 

    //setView(50*sin(timer), true, identifier.row, identifier.col);

  }

  //onDraw() Renders the meshes to the SCREEN
  virtual void onDraw(){ 

    draw(*circle);
    draw(*mboR);
    draw(*mboC);    


  } 
  
}; 
        


/*-----------------------------------------------------------------------------
 *  MAIN
 *-----------------------------------------------------------------------------*/

int main(){
  
  MyApp app(1920,1080); 
  app.start();

  return 0;

}

