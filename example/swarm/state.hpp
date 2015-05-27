

#ifndef  state_INC
#define  state_INC

#define NUM_AGENT 60
#define NUM_FACES_PER_AGENT 

struct State {
  
  gfx::Pose pose[NUM_AGENT];

  //gfx::Vec3f vec[NUM_VERTEX_PER_AGENT];
  float speed[NUM_AGENT];
};

#endif   /* ----- #ifndef state_INC  ----- */


