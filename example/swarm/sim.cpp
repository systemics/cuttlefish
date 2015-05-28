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
#include "ctl_desktop_sim.h"

#include "form/vsr_knot.h"
#include "form/vsr_twist.h"
#include "util/vsr_stat.h"
#include "form/vsr_group.h"

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

  MBO mbo;
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


    population.init<Jelly>();


   // jelly.init();
    mSceneRenderer.immediate(false);    

    mbo = MBO(Mesh::Points(NUM_VERTICES).mode(GL::T), GL::DYNAMIC);
    
    for (int i = 0; i<mbo.mesh.num(); ++i){
      float t = (float)i/mbo.mesh.num();
      mbo.mesh[i].Col.set( Rand::Num(), 1-t, t*.5, 1);
    }
    
  }
  
  void onAnimate(){
    population.step(.01);
    if (bReset) population.reset<Jelly>();
    onStep();

     for (int i = 0;i < NUM_VERTICES;++i){
        mbo.mesh[i].Pos = state->vec[i];
      }

      mbo.update();

  }

  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

      for (auto& i : population.member){
        render::pipe( *(Frame*)(i), this);
      }
      
      render::pipe( mbo, this );
  }

   // PointGroup3D<Vec>( );
      //population.draw();
   // else population.draw(this);

  void onSimulate(){

    for(int i=0;i<NUM_AGENTS;++i){
       for (int j=0;j<NUM_VERTEX_PER_AGENT;++j){
          int idx = i*NUM_VERTEX_PER_AGENT+j;
          state->vec[idx] = population.member[i]->pnt[j];
       }
    }
   
//       for(int i=0;i<NUM_AGENTS;++i){
//         for (int j=0;j<NUM_VERTEX_PER_AGENT;++j){
//            int idx = i*NUM_VERTEX_PER_AGENT+j;
//            state->vec[idx] = population.member[i]->pnt[j];
//         }
//         //state->pose[i].set( population.member[i]->pos(),  population.member[i]->quat() );
//         state->speed[i] = population.member[i]->neighborhood().toonear.size() * .2;
//       }
     
  }
  
  
};


int main(){
                             
  MyApp app;
  app.initState();
  app.start();

  return 0;

}
