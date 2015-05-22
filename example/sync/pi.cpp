#include "ctl_app.h"

//#include "gfx/util/egl_window.h"
//#include "gfx/gfx_render.h"
#include "vsr/util/vsr_stat.h"

using namespace ctl;
using namespace gfx;

#include "state.hpp"

using namespace vsr;

struct MyApp : RenderApp<State> {
  MBO mbo;

  void setup() {
    mbo = MBO(Mesh::Points(NUMPOSE * 3).mode(GL::T), GL::DYNAMIC);
    mSceneGraph.mMeshNode.add(&mbo);
  }

  virtual void onAnimate() {
    RenderApp::onAnimate();
    // update mesh
    for (int i = 0; i < NUMPOSE; ++i) {
      int idx = i * 3;
      auto a = state->pose[i].pos() + state->pose[i].x();
      auto b = state->pose[i].pos() + state->pose[i].y();
      auto c = state->pose[i].pos() + state->pose[i].z();
      auto norm = (b - a).cross(c - a).unit();
      mbo.mesh[idx].Pos =
          a;  // Vec3f(Rand::Num(2), Rand::Num(2), Rand::Num(2) );
      mbo.mesh[idx + 1].Pos = b;  // state->pose[i].pos()+state->pose[i].y();
      mbo.mesh[idx + 2].Pos = c;  // state->pose[i].pos()+state->pose[i].z();
      mbo.mesh[idx].Norm = norm;
      mbo.mesh[idx + 1].Norm = norm;
      mbo.mesh[idx + 2].Norm = norm;
      mbo.mesh[idx].Col = Vec4f(state->speed[i], 1, 1 - state->speed[i], .8);
      mbo.mesh[idx + 1].Col =
          Vec4f(1, state->speed[i], 1 - state->speed[i], .8);
      mbo.mesh[idx + 2].Col =
          Vec4f(state->speed[i], 1 - state->speed[i], 1, .8);
    }
    mbo.update();
  }
};

int main() {
  MyApp app;
  app.start();

  return 0;
}
