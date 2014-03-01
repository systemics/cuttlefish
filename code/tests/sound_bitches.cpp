#include <Gamma/Gamma.h>
#include <Gamma/AudioIO.h>
#include <Gamma/Oscillator.h>
#include <Gamma/SamplePlayer.h>
#include <Gamma/Types.h>

#include <unistd.h>

using namespace gam;

volatile bool makeClick = false;

void audioCB(AudioIOData& io) {
  while (io()) {
    if (makeClick) {
      makeClick = false;
      io.out(0) = io.out(1) = 1;
    }
    else
      io.out(0) = io.out(1) = 0;
  }
}

int main() {
  AudioIO io(512, 44100, audioCB, NULL, 2);
  Sync::master().spu(io.framesPerSecond());
  io.start();
  for (int i = 0; i < 1000; ++i) {
    usleep(10000);
    makeClick = true;
    usleep(10);
    makeClick = false;
  }
  return 0;
}
