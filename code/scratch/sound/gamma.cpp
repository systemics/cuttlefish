#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include "Gamma/Gamma.h"
#include "Gamma/Oscillator.h"

typedef signed short SAMPLE;
#define FORMAT RTAUDIO_SINT16
#define SCALE  32767.0

unsigned int channels;
RtAudio::StreamOptions options;
unsigned int frameCounter = 0;
bool checkCount = false;
unsigned int nFrames = 0;
const unsigned int callbackReturnValue = 1;

using namespace gam;

SineD<> bonk(555.0, 0.8, 0.5);
Accum<> timer(2);

int gamma( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *data )
{
  unsigned int i, j;
  extern unsigned int channels;
  SAMPLE *buffer = (SAMPLE *) outputBuffer;
  //double *lastValues = (double *) data;

  if ( status )
    std::cout << "Stream underflow detected!" << std::endl;

  double s;

  for ( i=0; i<nBufferFrames; i++ ) {
		if (timer()) {
      printf("reset bonk()\n");
			bonk.set(rnd::uni(700.0) + 200, 0.9f, 0.3f);
    }
    s = bonk();
    for ( j=0; j<channels; j++ ) {
      *buffer++ = (SAMPLE) (s * SCALE);
    }
  }

  //printf("%g\n", s);

  frameCounter += nBufferFrames;
  if ( checkCount && ( frameCounter >= nFrames ) )
    return callbackReturnValue;
  return 0;
}

int main( int argc, char *argv[] ) {
  Sync::master().spu(48000);

  unsigned int bufferFrames = 512, fs = 48000, device = 0, offset = 0;

  RtAudio dac;
  if ( dac.getDeviceCount() < 1 ) {
    std::cout << "\nNo audio devices found!\n";
    exit( 1 );
  }

  channels = 2;

  //double *data = (double *) calloc( channels, sizeof( double ) );

  // Let RtAudio print messages to stderr.
  dac.showWarnings( true );

  // Set our stream parameters for output only.
  RtAudio::StreamParameters oParams;
  oParams.deviceId = device;
  oParams.nChannels = channels;
  oParams.firstChannel = offset;
  options.flags = RTAUDIO_HOG_DEVICE | RTAUDIO_SCHEDULE_REALTIME;

  try {
    dac.openStream( &oParams, NULL, FORMAT, fs, &bufferFrames, &gamma, NULL, &options );
    //dac.openStream( &oParams, NULL, FORMAT, fs, &bufferFrames, &gamma, (void *)data, &options );
    dac.startStream();
  }
  catch ( RtError& e ) {
    e.printMessage();
    goto cleanup;
  }

  if ( checkCount ) {
    while ( dac.isStreamRunning() == true )
      usleep(100 * 1000);
  }
  else {
    char input;
    //std::cout << "Stream latency = " << dac.getStreamLatency() << "\n" << std::endl;
    std::cout << "\nPlaying ... press <enter> to quit (buffer size = " << bufferFrames << ").\n";
    std::cin.get( input );

    try {
      // Stop the stream
      dac.stopStream();
    }
    catch ( RtError& e ) {
      e.printMessage();
    }
  }

cleanup:
  if ( dac.isStreamOpen() ) dac.closeStream();
  //free( data );

  return 0;
}
