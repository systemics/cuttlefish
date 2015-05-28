#ifndef  state_INC
#define  state_INC


/*-----------------------------------------------------------------------------
 *  AGENTS
 *-----------------------------------------------------------------------------*/
#define NUM_AGENTS 70
#define NUM_VERTEX_BASE 5
#define MAX_NUM_REFLECTIONS 24
#define NUM_VERTEX_TAIL 0
#define NUM_VERTEX_PER_AGENT (NUM_VERTEX_BASE * MAX_NUM_REFLECTIONS + NUM_VERTEX_TAIL) 
#define NUM_VERTICES (NUM_AGENTS * NUM_VERTEX_PER_AGENT)


/*-----------------------------------------------------------------------------
 *  FOOD SUBSTRATE
 *-----------------------------------------------------------------------------*/
#define NUM_CELLS_WIDTH_SUBSTRATE 100
#define NUM_CELLS_WIDTH_SUBSTRATE 30
#define NUM_VERTEX_BASE_SUBSTRATE 4
#define NUM_REFLECTIONS_PER_CELL 18 //for p3m1
#define NUM_VERTEX_PER_CELL (NUM_VERTEX_BASE_SUBSTRATE * NUM_REFLECTIONS_PER_CELL)
#define NUM_VERTICES_SUBSTRATE (NUM_CELLS_WIDTH_SUBSTRATE * NUM_CELLS_HEIGHT_SUBSTRATE * NUM_VERTEX_PER_CELL)

struct State {
  
  gfx::Vec3f vec[NUM_VERTICES];
  gfx::Vec2f vec2[NUM_VERTICES_SUBSTRATE];
//  float speed[NUM_AGENTS];
};

#endif   /* ----- #ifndef state_INC  ----- */


