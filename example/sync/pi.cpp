#include "ctl_app.h"
//#include "gfx/util/egl_window.h"
//#include "gfx/gfx_render.h"
//#include "vsr/util/vsr_stat.h"

using namespace ctl;
using namespace gfx;

#include "state.hpp"

struct MyApp : RenderApp<State> {

  void setup() {}

  virtual void onAnimate() {
    RenderApp::onAnimate();
    mSceneGraph.mViewNode.view->color.set((state->phase % 256) / 256.0f, 0, 0, 1);
  }
};

int main() {
  MyApp app;
  app.start();
  return 0;
}
