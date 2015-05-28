

#ifndef  state_INC
#define  state_INC

#define NUM_AGENTS 60
#define NUM_FACES_PER_AGENT 

struct State {
  
  gfx::Pose pose[NUM_AGENTS];

  //gfx::Vec3f vec[NUM_VERTEX_PER_AGENT];
  float speed[NUM_AGENTS];
};

#endif   /* ----- #ifndef state_INC  ----- */


