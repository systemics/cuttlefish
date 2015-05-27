

#ifndef  state_INC
#define  state_INC

#define NUMPOSE 60

struct State {
  gfx::Pose pose[NUMPOSE];
  float speed[NUMPOSE];
};

#endif   /* ----- #ifndef state_INC  ----- */


