#include "ctl_bone.h"

struct Foo {
  float time;
};

#if SIM

struct MyApp : ctl::Simulator<Foo> {
  MyApp() : Simulator<Foo>("192.168.7.255", 1 / 30.f) {
    //
    // shouldLog = true;
  }
  virtual void setup(Foo& state) { state.time = 0; }
  virtual void update(float dt, Foo& state) {
    state.time += 0.01f;
    //    LOG("update(%f) -> %f", dt, state.time);
  }
};

#else

#include "ctl_app.h"
#include "Gamma/Gamma.h"

using namespace ctl;
using namespace gam;
using namespace gfx;

struct MyApp : CuttleboneApp<Foo> {

  float f;

  MBO* mbo;
  float mix[2];  ///<-- Left/Right Sound Mix

  MyApp() : CuttleboneApp<Foo>(Layout(4, 4), 30.0) { Sound::init(1024, 48000); }

  virtual void setup() {
    Mesh mesh;
    mesh.add(-1, -1, 0).add();
    mesh.add(-1, 1, 0).add();
    mesh.add(1, 1, 0).add();
    mesh.add(1, -1, 0).add();

    mbo = new MBO(mesh);
  }

  virtual void update(float dt, Foo& state, int popCount) {
    f = state.time - int(state.time);
    background.set(f, 1 - f, f * f * f, 1.0f);

    Vec3f v(cos(state.time) * layout.screenWidth / 2.0, 0, 0);
    mbo->mesh.translateA(v.x, 0, 0);
    mbo->update();

    float t = cos(state.time);
    t = t * t * t * t * t;
    mix[0] = t > 0 ? t : 0;
    mix[1] = t < 0 ? -t : 0;
  }

  // Broken with "Layout(4, 4)"
  //  float lw(const Vec3f& v) {
  //    float sqd = (v - layout.speakerL).sq();  // squared distance
  //    return CLAMP(1.0 / (fabs(1.0 - sqd) + .001), 0, 1);
  //  }
  //
  //  float rw(const Vec3f& v) {
  //    float sqd = (v - layout.speakerR).sq();  // squared distance
  //    return CLAMP(1.0 / (fabs(1.0 - sqd) + .001), 0, 1);
  //  }

  virtual void onDraw() { pipe.line(*mbo); }

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
