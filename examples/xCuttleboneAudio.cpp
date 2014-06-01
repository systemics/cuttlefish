#include "ctl_bone.h"

struct Foo {
  float time;
};

#if SIM

struct MyApp : ctl::Simulator<Foo> {
  MyApp() : Simulator<Foo>("192.168.7.255", 1 / 30.f) {}
  virtual void setup(Foo& state) { state.time = 0; }
  virtual void update(float dt, Foo& state) { state.time += 0.01f; }
};

#else

#include "ctl_bcm.h"
#include "ctl_host.h"
#include "ctl_sound.h"
#include "ctl_egl.h"
#include "gfx/gfx_renderer.h"  //<-- the encapsulated rendering engine
#include "Gamma/Gamma.h"

struct MyApp : ctl::BCM, ctl::Host, ctl::Renderer<Foo>, ctl::Sound {

  float mix[2];  ///<-- Left/Right Sound Mix

  MyApp() { Sound::init(1024, 48000); }

  virtual void firstRun() {}

  virtual void gotState(float dt, Foo& state, int popCount) {
    float t = cos(state.time);
    t = t * t * t * t * t;
    mix[0] = t > 0 ? t : 0;
    mix[1] = t < 0 ? -t : 0;
  }

  virtual void onSound(SoundData& io) {
    for (int i = 0; i < io.n; ++i) {
      float s = i / (float)io.n;
      for (int j = 0; j < 2; j++) {
        *io.outputBuffer++ = (short)(s * 32767.0) * mix[j];
      }
    }
  }
};

#endif

int main() { MyApp().start(); }
