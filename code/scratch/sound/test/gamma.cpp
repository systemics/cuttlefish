#include "Sound.h"
#include "Gamma/Gamma.h"
#include "Gamma/Oscillator.h"

using namespace gam;
using namespace cf;

SineD<> bonk(555.0, 0.8, 0.5);
Accum<> timer(2);

struct Foo : Sound {
  Foo() {
    init();
  }

  virtual void onSound(SoundData& io) {
    for (int i = 0; i < io.n; ++i) {
      if (timer()) {
        printf("reset bonk()\n");
        bonk.set(rnd::uni(700.0) + 200, 0.9f, 0.3f);
      }
      float s = bonk();
      for (int j = 0; j < 2; j++) {
        *io.outputBuffer++ = (short)(s * 32767.0);
      }
    }
  }
};

int main() {
  Sync::master().spu(48000);

  Foo foo;
  getchar();
}
