#include "ctl_bcm.h"
#include "ctl_bone.h"
#include "ctl_sound.h"
#include "ctl_egl.h"

struct State {
  float value;
};

#if SIM

struct MyApp : Simulator<State> {
  MyApp() : Simulator<State>("192.168.7.255", 1 / 30.f) {}
  virtual void setup(State& state) { state.value = 0; }
  virtual void update(float dt, State& state) {
    state.value += 0.05f;
    LOG("update(%f) -> %f", dt, state.value);
  }
};

#else

struct MyApp : Renderer<State> {
  virtual void gotState(float dt, State& state, int popCount) {
    LOG("gotState(%f) -> %f", dt, state.value);
  }
};

#endif

int main() { MyApp().start(); }
