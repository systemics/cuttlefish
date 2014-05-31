
#include "ctl_app.h"
//#include "gfx/gfx_mbo.h"

using namespace std;
using namespace ctl; 


struct MyApp : public App {
        
  //THIS IS THE APP SPECIFIC STUFF, 
  MBO * circle;  
  MBO * mboR;
  MBO * mboC;
  
  //INSTANTIATE THE APPLICATION WITH WIDTH AND HEIGHT
  MyApp() : App( Layout(4,4) ) { 
    init();
  }                                  
  
  //INIITIALIZE ELEMENTS OF THE SCENE 
  virtual void init(){
    circle = new MBO( Mesh::Circle(6) );
    circle = new MBO( Mesh::Circle(6) );
    circle = new MBO( Mesh::Circle(6) );

  }  
  
  //UPDATE() CHANGES THE POSITION OF THE CIRCLE
  virtual void update(){
     
    static float time = 0; time +=.01;
    
    //change position of vertices
    circle -> mesh.moveTo( sin(time) * layout.totalWidth()/2.0, cos(time) * layout.totalHeight()/2.0, 0 );   
    circle -> mesh.color( 1, fabs( sin(time) ), 1 );
    
    //send changes to buffer
    circle -> update();                                   
    
		mboR = new MBO( Mesh::Num(identifier.row).translate(-1,0,0).moveTo( layout.speakerL + Vec3f(3,0,0) ) );
		mboC = new MBO( Mesh::Num(identifier.col).translate(1,0,0).moveTo( layout.speakerL + 
                    Vec3f(layout.screenWidth - 3,0,0) ) );  

    

    //background color
    background.set( fabs( sin(time) ) * .2, fabs( cos(time) ) * 1, 1, 1 );  
   
  }

  //THIS DRAWS THE CIRCLE TO THE SCREEN
  virtual void onDraw(){ 
    pipe.line( *circle );
    pipe.line( *mboR );
    pipe.line( *mboC );
  } 
  
}; 
        


bool running = true;
void quit(int) {
  running = false;
}

int main(){
  
  MyApp app; 
  
  while(running){
    app.onFrame();
    usleep(166);
  }   
  
    return 0; 
}

