#ifndef  ctl_sim_INC
#define  ctl_sim_INC

#include "Cuttlebone/Cuttlebone.hpp"

template<typename STATE>
struct SimApp {

  cuttlebone::Maker<STATE> maker;
  STATE * state;
  bool running;

  SimApp() : maker("192.168.7.255"){
    state = new STATE();
    memset(state, 0, sizeof(STATE));
    running = false;
  }


  void start(bool makeThread = false){
    maker.start();
    if (makeThread) {
      running = true;
      thread t([this]() {
        onSetup();
        while (running)
          onStep();
      });
      getchar();
      running = false;
      t.join();
      stop();
    }
  }

  void stop(){
    running = false;
    maker.stop();
    delete state;
  }

  virtual void onSimulate(double dt) = 0;
  virtual void onSetup() = 0;

  void onStep(){
    static cuttlebone::Timestamp<> t;
    static double time, last;
    time = t.stamp();
    onSimulate(time - last);
    last = time;
    maker.set(*state);
  }
};

#endif   /* ----- #ifndef ctl_sim_INC  ----- */
