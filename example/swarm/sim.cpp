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

#include "ctl_sim.h"  

#include "form/vsr_knot.h"
#include "form/vsr_twist.h"
#include "util/vsr_stat.h"
#include "form/vsr_group.h"

#include "swarm.h"
#include "state.hpp"
#include "ctl_sim.h"

//using namespace vsr;
//using namespace vsr::cga;

/*-----------------------------------------------------------------------------
 * DESKTOP APPLICATION
 *-----------------------------------------------------------------------------*/
struct MyApp : SimApp<State> {
 
  Population population;

  void setup(){

    population.init<Jelly>();

  }


  void onSimulate(){
    
    population.step(.01);

    for(int i=0;i<NUM_AGENTS;++i){
       for (int j=0;j<NUM_VERTEX_PER_AGENT;++j){
          int idx = i*NUM_VERTEX_PER_AGENT+j;
          state->vec[idx] = population.member[i]->pnt[j];
       }
    }
     
  }
  
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
