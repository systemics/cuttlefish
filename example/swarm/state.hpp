

#ifndef  state_INC
#define  state_INC

#define NUM_AGENTS 60
#define NUM_VERTEX_BASE 3
#define MAX_NUM_REFLECTIONS 24
#define NUM_VERTEX_PER_AGENT (NUM_VERTEX_BASE * MAX_NUM_REFLECTIONS) 
#define NUM_VERTICES (NUM_AGENTS * NUM_VERTEX_PER_AGENT)


struct State {
  
 // gfx::Pose pose[NUM_AGENTS];

  gfx::Vec3f vec[NUM_VERTICES];
  float speed[NUM_AGENTS];
};

#endif   /* ----- #ifndef state_INC  ----- */


