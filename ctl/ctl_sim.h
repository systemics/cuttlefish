/*
 * =====================================================================================
 *
 *       Filename:  ctl_sim.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/27/2015 14:03:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */

#ifndef  ctl_sim_INC
#define  ctl_sim_INC


#include "Cuttlebone/Cuttlebone.hpp"


template<typename STATE>
struct SimApp {

  cuttlebone::Maker<STATE> maker;
  STATE * state;

  SimApp() : maker("192.168.7.255"){}

  void initState(){
    state = new STATE();
    memset(state, 0, sizeof(STATE)); 
    maker.start();  
  }

  virtual void onSimulate(){
     
  }

  void onStep(){
    onSimulate();
    maker.set(*state);   
  }


};

#endif   /* ----- #ifndef ctl_sim_INC  ----- */
