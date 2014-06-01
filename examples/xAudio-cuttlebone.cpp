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

#include "ctl_bcm.h"
#include "ctl_host.h"
#include "ctl_sound.h"
#include "ctl_egl.h"
#include "gfx/gfx_renderer.h"  //<-- the encapsulated rendering engine
#include "Gamma/Gamma.h"

struct MyApp : ctl::BCM,
               ctl::Host,
               ctl::Renderer<Foo>,
               gfx::Renderer,
               ctl::Sound {

  ctl::EGL::Window* w;

  float v;

  gfx::MBO* mbo;
  float mix[2];  ///<-- Left/Right Sound Mix

  MyApp() : gfx::Renderer(30, 20) { Sound::init(1024, 48000); }

  virtual void firstRun() {
    w = new ctl::EGL::Window;
    initGL(gfx::Renderer::GLES, gfx::Renderer::BUFFERED, w->surface.width,
           w->surface.height);

    gfx::Mesh mesh;
    mesh.add(-1, -1, 0).add();
    mesh.add(-1, 1, 0).add();
    mesh.add(1, 1, 0).add();
    mesh.add(1, -1, 0).add();

    mbo = new gfx::MBO(mesh);
  }

  virtual void gotState(float dt, Foo& state, int popCount) {
    v = state.time - int(state.time);

    gfx::Vec3f v(cos(state.time) * layout.screenWidth / 2.0, 0, 0);
    mbo->mesh.translateA(v.x, 0, 0);
    mbo->update();

    mix[0] = lw(v);
    mix[1] = rw(v);

    this->onFrame();
  }

  float lw(const gfx::Vec3f& v) {
    float sqd = (v - layout.speakerL).sq();  // squared distance
    return CLAMP(1.0 / (fabs(1.0 - sqd) + .001), 0, 1);
  }

  float rw(const gfx::Vec3f& v) {
    float sqd = (v - layout.speakerR).sq();  // squared distance
    return CLAMP(1.0 / (fabs(1.0 - sqd) + .001), 0, 1);
  }

  virtual void onDraw() {
    // background.set(v, 1 - v, v * v * v, 1.0f);
    pipe.line(*mbo);
  }

  virtual void onFrame() {
    gfx::Renderer::clear();
    gfx::Renderer::render();
    w->swapBuffers();
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
