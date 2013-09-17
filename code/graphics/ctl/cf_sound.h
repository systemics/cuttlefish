/*
 *  cf_sound.h
 *  Cuttlefish
 *  Created by Karl Yerkes on Mon Sep 16 12:40:08 PDT 2013
 *  Copyright 2013 Karl Yerkes. All rights reserved.
 *
*/
#ifndef CF_SOUND_H_INCLUDED
#define CF_SOUND_H_INCLUDED

namespace cf {

struct SoundImpl;
struct Sound {
  SoundImpl* impl;
  struct SoundData {
    short* outputBuffer;
    short* inputBuffer;
    short n;
  } soundData;
  void init();
  virtual void onSound(SoundData& io);
};

}
#endif
