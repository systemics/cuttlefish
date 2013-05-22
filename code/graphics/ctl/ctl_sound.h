/*
 *  ctl_sound.h
 *  Cuttlefish
 *  Created by Karl Yerkes on Wed May 22 08:59:14 PDT 2013
 *  Copyright 2013 Karl Yerkes. All rights reserved.
 *
*/
#ifndef CTL_SOUND_H_INCLUDED
#define CTL_SOUND_H_INCLUDED

#include <Gamma/Gamma.h>
#include <Gamma/AudioIO.h>
#include <Gamma/Oscillator.h>
#include <Gamma/SamplePlayer.h>
#include <Gamma/Types.h>

namespace ctl {

//using namespace gam;

void audioCB(gam::AudioIOData& io);

struct Sound {
  gam::AudioIO io;
  Sound() : io(1024, 22050, audioCB, this, 1) {
    gam::Sync::master().spu(io.framesPerSecond());
    io.start();
  }
  virtual void onSound(gam::AudioIOData& io) = 0;
};

void audioCB(gam::AudioIOData& io) {
  ((Sound*)io.user())->onSound(io);
}

}
#endif
