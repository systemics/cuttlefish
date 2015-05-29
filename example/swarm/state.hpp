#ifndef  state_INC
#define  state_INC


/*-----------------------------------------------------------------------------
 *  AGENTS
 *-----------------------------------------------------------------------------*/
#define NUM_AGENTS 30
#define NUM_VERTEX_BASE 5
#define MAX_NUM_REFLECTIONS 24
#define NUM_VERTEX_TAIL 0
#define NUM_VERTEX_PER_AGENT (NUM_VERTEX_BASE * MAX_NUM_REFLECTIONS + NUM_VERTEX_TAIL) 
#define NUM_VERTICES (NUM_AGENTS * NUM_VERTEX_PER_AGENT)


/*-----------------------------------------------------------------------------
 *  FOOD SUBSTRATE
 *-----------------------------------------------------------------------------*/
#define NUM_CELLS_WIDTH_SUBSTRATE 50
#define NUM_CELLS_HEIGHT_SUBSTRATE 20
#define NUM_CELLS_SUBSTRATE (NUM_CELLS_WIDTH_SUBSTRATE*NUM_CELLS_HEIGHT_SUBSTRATE) 
#define NUM_VERTEX_BASE_SUBSTRATE 4
#define NUM_REFLECTIONS_PER_CELL 6 
#define NUM_VERTEX_PER_CELL (NUM_VERTEX_BASE_SUBSTRATE * NUM_REFLECTIONS_PER_CELL)
#define NUM_VERTICES_SUBSTRATE (NUM_CELLS_SUBSTRATE * NUM_VERTEX_PER_CELL)

struct State {
  
  gfx::Vec3f vec[NUM_VERTICES];
//  gfx::Vec2f vec2[NUM_VERTICES_SUBSTRATE];
//  float density[NUM_VERTICES_SUBSTRATE];
};

#endif   /* ----- #ifndef state_INC  ----- */


