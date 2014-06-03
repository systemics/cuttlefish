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

#include "ctl_app.h"
#include "Gamma/Gamma.h"

using namespace ctl;
using namespace gfx;
using namespace gam;

struct MyApp : CuttleboneApp<Foo> {

  float mix[2];  ///<-- Left/Right Sound Mix

  MyApp() : CuttleboneApp<Foo>(Layout(4, 4), 30.0) { Sound::init(1024, 48000); }

  virtual void setup() {}

  virtual void update(float dt, Foo& state, int popCount) {
    float t = cos(state.time);
    t = t * t * t * t * t;
    mix[0] = t > 0 ? t : 0;
    mix[1] = t < 0 ? -t : 0;
  }

  virtual void onDraw() {
    // fyi, you can access current state like this:
    //
    // LOG("time is %f", renderState->time);
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
