#include "ctl_sound.h"
#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <unistd.h>

namespace ctl {

unsigned int channels;
RtAudio::StreamOptions options;
unsigned int frameCounter = 0;
bool checkCount = false;
unsigned int nFrames = 0;
const unsigned int callbackReturnValue = 1;

int processSoundFrame(
  void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
  double streamTime, RtAudioStreamStatus status, void *data)
{
  unsigned int i, j;
  extern unsigned int channels;
  Sound* sound = (Sound*)data;
  sound->soundData.outputBuffer = (short*)outputBuffer;
  sound->soundData.inputBuffer = (short*)inputBuffer;
  sound->soundData.n = nBufferFrames;
  sound->onSound(sound->soundData);

  if (status)
    std::cout << "Stream underflow detected!" << std::endl;

  frameCounter += nBufferFrames;
  if (checkCount && (frameCounter >= nFrames))
    return callbackReturnValue;
  return 0;
}

struct SoundImpl {
  RtAudio dac;
  RtAudio::StreamParameters oParams;
  Sound* sound;
  SoundImpl(Sound* sound) : sound(sound) {}

  bool running() {
    return (dac.isStreamRunning() == true);
  }

  void stop() {
    try {
      dac.stopStream();
    }
    catch (RtError& e) {
      e.printMessage();
    }
  }

  void init() {
    unsigned int bufferFrames = 512, fs = 48000, device = 0, offset = 0;

    if (dac.getDeviceCount() < 1) {
      std::cout << "\nNo audio devices found!\n";
      exit(1);
    }

    channels = 2;

    dac.showWarnings(true);

    oParams.deviceId = device;
    oParams.nChannels = channels;
    oParams.firstChannel = offset;
    options.flags = RTAUDIO_HOG_DEVICE | RTAUDIO_SCHEDULE_REALTIME;

    try {
      dac.openStream(&oParams, NULL, RTAUDIO_SINT16, fs, &bufferFrames, &processSoundFrame, (void*)sound, &options);
      dac.startStream();
    }
    catch (RtError& e) {
      e.printMessage();
      if (dac.isStreamOpen())
        dac.closeStream();
      exit(1);
    }
  }
};

void Sound::init() {
  impl = new SoundImpl(this);
  impl->init();
}

void Sound::onSound(SoundData& io) {
}

}
