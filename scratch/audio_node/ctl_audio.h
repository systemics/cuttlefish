#ifndef CF_AUDIO_H_INCLUDED
#define CF_AUDIO_H_INCLUDED

#include "RtAudio.h"
#include <iostream>
#include <cstdlib>

//namespace ctl {

int saw(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *userData);

struct Audio {
  RtAudio dac;

  unsigned blockSize, sampleRate;

  Audio(unsigned blockSize = 512, unsigned sampleRate = 48000) : blockSize(blockSize), sampleRate(sampleRate) {}

  void start() {
    if (dac.getDeviceCount() < 1) {
      printf("No audio devices found!\n");
      exit(0);
    }

    RtAudio::StreamParameters parameters;
    parameters.deviceId = dac.getDefaultOutputDevice();
    parameters.nChannels = 2;
    parameters.firstChannel = 0;

    try {
      dac.openStream(&parameters, NULL, RTAUDIO_FLOAT32, sampleRate, &blockSize,
                     &saw, (void *)this);

      dac.startStream();
    } catch (RtError &e) {
      e.printMessage();
      exit(0);
    }

    getchar();
    try {
      dac.stopStream();
    } catch (RtError &e) {
      e.printMessage();
    }
    if (dac.isStreamOpen()) dac.closeStream();
  }

  virtual void process(float *output) = 0;
};

int saw(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
        double streamTime, RtAudioStreamStatus status, void *userData) {
  ((Audio *)userData)->process((float*)outputBuffer);
  return 0;
}

//}
#endif
