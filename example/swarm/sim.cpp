
/*
 * =====================================================================================
 *
 *       Filename:  xJellies.cpp
 *
 *    Description:  jellyfish organisms
 *
 *        Version:  1.0
 *        Created:  05/26/2015 11:12:06
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */

#include "vsr_app.h"  
#include "ctl_sim.h"


#include "form/vsr_knot.h"
#include "form/vsr_twist.h"
#include "util/vsr_stat.h"

#include "swarm.h"
#include "state.hpp"
#include "ctl_sim.h"

using namespace vsr;
using namespace vsr::cga;

/*-----------------------------------------------------------------------------
 * DESKTOP APPLICATION
 *-----------------------------------------------------------------------------*/
struct MyApp : App, SimApp<State> {
 
  //Some Variables
  bool bReset = false;
  float amt = 0;

  //Jelly jelly;
  Population population;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
       
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(bReset,"bReset");

    gui(population.minDistance, "mindist",0,100);
    gui(population.maxDistance, "maxdist",0,100);
    gui(population.globalFlockRotVel, "globalFlockRotVel",0,100);
    gui(population.globalSourceRotVel, "globalSourceRotVel",0,100);
    gui(population.globalAvertRotVel, "globalAvertRotVel",0,100);

    population.init();
   // jelly.init();
   // mSceneRenderer.immediate(false);    
    
  }

  void onSimulate(){
       for(int i=0;i<NUM_AGENTS;++i){
         state->pose[i].set( population.member[i]->pos(),  population.member[i]->quat() );
         state->speed[i] = population.member[i]->neighborhood().toonear.size() * .2;
       }
  }
  
  void onAnimate(){
    population.step(.01);
    if (bReset) population.reset();
    onStep();
  }

  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    if (mSceneRenderer.immediate() ){
      for (auto& i : population.member){
        Draw( *(Frame*)(i) );
      }
    }
      //population.draw();
   // else population.draw(this);

  }
  
};


int main(){
                             
  MyApp app;
  app.initState();
  app.start();

  return 0;

}
