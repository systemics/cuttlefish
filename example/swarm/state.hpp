#ifndef  state_INC
#define  state_INC

#include "gfx/gfx_matrix.h"
/*-----------------------------------------------------------------------------
 *  AGENTS
 *-----------------------------------------------------------------------------*/
#define NUM_AGENTS 2 
#define NUM_VERTEX_BASE 5
#define MAX_NUM_REFLECTIONS 24
#define NUM_VERTEX_TAIL 0
#define NUM_VERTEX_PER_AGENT (NUM_VERTEX_BASE * MAX_NUM_REFLECTIONS + NUM_VERTEX_TAIL) 
#define NUM_VERTICES (NUM_AGENTS * NUM_VERTEX_PER_AGENT)


/*-----------------------------------------------------------------------------
 *  FOOD SUBSTRATE
 *-----------------------------------------------------------------------------*/
#define NUM_CELLS_WIDTH_SUBSTRATE 16 
#define NUM_CELLS_HEIGHT_SUBSTRATE 9
#define NUM_CELLS_SUBSTRATE (NUM_CELLS_WIDTH_SUBSTRATE*NUM_CELLS_HEIGHT_SUBSTRATE) 
#define NUM_VERTEX_BASE_SUBSTRATE 1
#define NUM_REFLECTIONS_PER_CELL 3 
#define NUM_VERTEX_PER_CELL (NUM_VERTEX_BASE_SUBSTRATE * NUM_REFLECTIONS_PER_CELL)
#define NUM_VERTICES_SUBSTRATE (NUM_CELLS_SUBSTRATE * NUM_VERTEX_PER_CELL)

#define MAX_TOUCHES 5

struct State {
  
  gfx::Vec3f vec[NUM_VERTICES];
  
  float food[NUM_CELLS_SUBSTRATE];

  gfx::Vec2f touch[MAX_TOUCHES];

  int numtouches;
};

#endif   /* ----- #ifndef state_INC  ----- */


