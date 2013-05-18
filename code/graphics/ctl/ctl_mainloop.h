/*
 *  ctl_mainloop.h
 *  Cuttlefish
 *  Created by Karl Yerkes on Sat May 18 05:30:18 PDT 2013
 *  Copyright 2013 Karl Yerkes. All rights reserved.
 *
*/
#ifndef CTL_MAINLOOP_H_INCLUDED
#define CTL_MAINLOOP_H_INCLUDED

#include <signal.h>
#include <unistd.h>
#include <iostream>
using namespace std;

namespace ctl {

void quit(int);
struct MainLoop;
MainLoop* mainLoop = nullptr;

struct MainLoop {

  bool running;
  virtual void onLoop() = 0;

  MainLoop() {
    cout << "MainLoop()\n";
    signal(SIGABRT, quit);
    signal(SIGTERM, quit);
    signal(SIGINT, quit);
    running = false;
    mainLoop = this;
  };

  virtual ~MainLoop() {
    cout << "~MainLoop()\n";
    usleep(100000);
  }

  void start() {
    cout << "MainLoop::start()\n";
    running = true;
    while (running)
      onLoop();
  }

  void stop() {
    cout << "MainLoop::stop()\n";
    running = false;
  }
};

void quit(int) {
  mainLoop->stop();
}

} //ctl::
#endif
