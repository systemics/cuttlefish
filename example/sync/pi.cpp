#include "ctl_app.h"
using namespace ctl;
using namespace gfx;

#include "state.hpp"

struct MyApp : RenderApp<State> {
  void setup() { LOG("started...."); }

  virtual void onAnimate() {
    // static cuttlebone::Stats fps;
    // RenderApp::onAnimate();
    mSceneGraph.mViewNode.view->color.set((state->phase % 256) / 256.0f, 0, 0,
                                          1);
    LOG("phase: %d", state->phase);
  }
};

int main() {
  MyApp app;
  app.start();
  return 0;
}
