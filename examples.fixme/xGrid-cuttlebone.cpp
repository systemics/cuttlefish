/*
 * =====================================================================================
 *
 *       Filename:  xGrid-cuttlebone.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/03/2014 10:52:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include "ctl_bone.h"

#include "vsr/vsr_




struct Foo {
  float value;
};

#if SIM

struct MyApp : ctl::Simulator<Foo> {
  MyApp() : Simulator<Foo>("192.168.7.255", 1 / 30.f) {
    //
    // shouldLog = true;
  }
  virtual void setup(Foo& state) { state.value = 0; }
  virtual void update(float dt, Foo& state) {
    state.value += 0.01f;
    //    LOG("update(%f) -> %f", dt, state.value);
  }
};

#else

#include "ctl_app.h"

using namespace ctl;
using namespace gfx;

struct MyApp : CuttleboneApp<Foo> {

  float v;

  MyApp() : CuttleboneApp<Foo>(Layout(4, 4), 30.0) {
    // Sound::init(1024, 48000);
  }

  virtual void setup() {}

  virtual void update(float dt, Foo& state, int popCount) {
    v = state.value - int(state.value);
  }

  virtual void onDraw() {
             
  }

  virtual void onSound(Sound::SoundData& io) {}
};

#endif

int main() { MyApp().start(); }
