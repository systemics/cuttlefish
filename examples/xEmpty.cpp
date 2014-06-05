/*
 * =====================================================================================
 *
 *       Filename:  xEmpty.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/04/2014 12:15:32
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */

#include "ctl_app.h"

using namespace std;
using namespace ctl; 


struct MyApp : public App {
        
  //THIS IS THE APP SPECIFIC STUFF, 

  
  //INSTANTIATE THE APPLICATION WITH WIDTH AND HEIGHT
  MyApp() : App( Layout(4,4) ) { 
    init();
  }                                  
  
  //INIITIALIZE ELEMENTS OF THE SCENE 
  virtual void init(){

  }  
  
  //UPDATE() CHANGES THE DATA
   virtual void update(){

  }

  //THIS DRAWS
  virtual void onDraw(){ 

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

