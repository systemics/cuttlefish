#include "Sound.h"
#include <stdio.h>
using namespace cf;

struct Foo : Sound {
  Foo() {
    init();
  }

  virtual void onSound(SoundData& io) {
    int i = 0;
    while (io()) {
      io.out(0) = io.out(1) = i / io.n;
    }

    //for (int i = 0; i < io.n; ++i) {
    //  float s = i / (float)io.n;
    //  for (int j = 0; j < 2; j++) {
    //    *io.outputBuffer++ = (short)(s * 32767.0);
    //  }
    //}
  }
};

int main() {
  Foo foo;
  getchar();
}
