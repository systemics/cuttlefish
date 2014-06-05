#define SERVER_PATH "/home/ky/"
//#define SERVER_PATH "/Users/ky/code/cuttlefish/"
#define CLIENT_PATH "/home/pi/"

//#define SOUND_FILE "czonic.wav"
//#define SOUND_FILE "5SNCLOCKS.aif"
#define SOUND_FILE "sound.wav"
//#define SOUND_FILE "distort.wav"
//#define SOUND_FILE "FLANNEL1.wav"

#include "gfx/gfx_matrix.h"
#include "vsr/vsr_cga3D_frame.h"
#include "vsr/vsr_stat.h"

#define MAX_TOUCH (3)
#define NUMAGENTS (20)

#define MULTIPLY (4)
#define WIDTH (16 * MULTIPLY)
#define HEIGHT (9 * MULTIPLY)
#define N_VERTICES (WIDTH * HEIGHT)

struct Foo {
  float time;
  gfx::Vec3f position[N_VERTICES];
  gfx::Vec2f touch[MAX_TOUCH];
  unsigned index[MAX_TOUCH];
  gfx::Vec3f pos[NUMAGENTS];
  gfx::Vec4f rot[NUMAGENTS];
  int touchCount;
};

#include <vector>
using namespace std;

auto indexOf = [](unsigned y, unsigned x) { return y * WIDTH + x; };

void generateGridSpringMesh(int width, int height, Foo& state,
                            vector<unsigned short>& triangleIndex,
                            vector<unsigned short>& lineIndex,
                            vector<vector<unsigned short>>& neighbor);

#if SIM

#include "ctl_bone.h"
#include "ctl_touch.h"
#include <string.h>

using namespace ctl;
using namespace gfx;
using namespace vsr;

#define SK (0.02f)
#define NK (0.05f)
#define D (0.97)
//#define SK (0.02f)
//#define NK (0.05f)
//#define D (0.98)
//.01, .1, .765

struct MyApp : Simulator<Foo>, Touch {
  MyApp() : Simulator<Foo>("192.168.7.255" /*, 1 / 30.f */) {}

  //SPRINGMESH
  float d, sk, nk;
  Vec3f stationary[N_VERTICES];
  Vec3f velocity[N_VERTICES];
  vector<vector<unsigned short>> neighbor;

  //PARTICLES
  vector<Frame> frame;
  float range;
  

  virtual void setup(Foo& state) {
    Touch::setup("/dev/input/event2");

    //SPRINGMESH
    memset(&state, 0, sizeof(state));
    memset(&stationary, 0, sizeof(Vec3f) * N_VERTICES);
    memset(&velocity, 0, sizeof(Vec3f) * N_VERTICES);

    vector<unsigned short> triangleIndex, lineIndex;
    generateGridSpringMesh(WIDTH, HEIGHT, state, triangleIndex, lineIndex,
                           neighbor);
    memcpy(&stationary, state.position, sizeof(Vec3f) * N_VERTICES);

    //PARTICLES
    frame = vector<Frame>(NUMAGENTS);

    range = 8;

    Rand::Seed();
    for (auto& f : frame ){
      Vec v( Rand::Num(-1,1) , Rand::Num(-1,1), Rand::Num(-1,1));
      f.pos() = Ro::null( v * range );
      f.rot() = Gen::rot( Biv(  Rand::Num(), Rand::Num(), Rand::Num() ) );
      f.scale() = .5;
    }   
  }

  virtual void update(float dt, Foo& state) {

    // Time
    //
    state.time += dt;

    // Touches and Interaction
    //
    pollTouches();
    int k = 0;
    for (auto& e : touchPoint)
      if (e.id != 0) {
        state.touch[k++] = Vec2f(e.x / 3200.0f, e.y / -2600.0f);
        if (k >= MAX_TOUCH) break;
      }
    state.touchCount = k;

    for (int i = 0; i < state.touchCount; ++i) {
      unsigned r = (unsigned)((state.touch[i].y + 1.0f) / 2 * HEIGHT);
      unsigned c = (unsigned)((state.touch[i].x + 1.0f) / 2 * WIDTH);
      int z = indexOf(r, c);
      state.index[i] = z;
      // LOG("r:%u c:%u x:%f y:%f", r, c, state.touch[i].x, state.touch[i].y);

      Vec3f v(0.0f, 0.0f, -1.0f);
      state.position[z] += v;
      for (auto n : neighbor[z]) state.position[n] += v;
      LOG("poke!");
    }

    // Simulation Rate Info
    //
    static float period = 0;
    static int simCount = 0;
    if (period > 1.0) {
      period -= 1.0;
      LOG("Sim: %d", simCount);
      simCount = 0;
    }
    period += dt;
    simCount++;

    // Automatic Poking
    /*
        auto r = []() { return 2.0f * rand() / RAND_MAX - 1.0f; };

        static int n = 0;
        if ((n % 100) == 0) {
          Vec3f v{r(), r(), r()};
          v *= 4.0f;
          int randomVec3f = rand() % N_VERTICES;
          state.position[randomVec3f] += v;
          v *= 1.7f;
          for (auto n : neighbor[randomVec3f]) state.position[n] += v;
          LOG("poke!");
        }
        n++;
        */

    // PHYSICS
    //
    for (int i = 0; i < N_VERTICES; i++) {
      Vec3f& v = state.position[i];
      Vec3f force = (v - stationary[i]) * -SK;

      for (int k = 0; k < neighbor[i].size(); k++) {
        Vec3f& n = state.position[neighbor[i][k]];
        force += (v - n) * -NK;
      }

      velocity[i] += force;
      velocity[i] *= D;
    }

    for (int i = 0; i < N_VERTICES; i++) state.position[i] += velocity[i];

    //LOG("%f %f %f", state.position[100].x, state.position[100].y, state.position[100].z);
    //
    //PARTICLES
    
    
      //swarm -- find nearest neighbors in z direction (within halfspace of xyplane)
      float acc = .02;
      float rotAcc = .02; 

      float thresh = 20;
      float min = 2.75;


     // Line line = mouse ^ Vec::z ^ Inf(1);
      
      int numNeighbors = 3;
      for (auto& fa : frame){


        vector<Frame> nearest;
        vector<Frame> toonear;

        for (auto& fb : frame){
          float halfplane = (fb.pos() <= fa.dxy())[0];
          if ( halfplane > 0 ){
            float dist = Ro::sqd( fa.bound(), fb.bound() );
            if (dist < thresh) nearest.push_back(fb);
            if (dist < min) toonear.push_back(fb);
            if (nearest.size() == numNeighbors) break;
         }
        }
        
        Biv db; // Amount to orient
        Vec dx; // Amount to move
      
        for (int i = 0; i < state.touchCount; ++i){

          Point mouse = Ro::null( 
            state.touch[i][0] * 43,
            state.touch[i][1] * 29, 0);

          float dist = Ro::sqd(fa.pos(), mouse);
          float famt = 1.0/(.01 + (dist) );
          LOG("%f %f",dist,famt);
          Vec tv( fa.pos() - mouse );
          tv[2] = 0;
          dx += tv * famt * 30;

        }


        if (!toonear.empty()){
           db += fa.xz() * 2;
           dx += fa.z() * 10; 
        } else {

         for (auto& neigh : nearest){
           db += Gen::log( neigh.rot() ) / nearest.size();
           dx += Vec( neigh.pos() - fa.pos() ) / nearest.size();
         }

         if (nearest.empty()){
           db += fa.xz() * .1;
           dx += fa.z() * 2;
         }

        }

         dx += -Vec(fa.pos()) * .01;

         fa.db() = db * rotAcc; 
         fa.dx() = dx * acc;    

         fa.move(); fa.spin();
      }

      for (int i=0;i<NUMAGENTS;++i){
        state.pos[i].set( frame[i].pos()[0], frame[i].pos()[1], frame[i].pos()[2] );
        state.rot[i].set( 
          frame[i].rot()[0], frame[i].rot()[1], 
          frame[i].rot()[2], frame[i].rot()[3] );
      }


  }
};

#else

#include "ctl_app.h"
#include "Gamma/Gamma.h"
#include "Gamma/Oscillator.h"
#include "Gamma/SamplePlayer.h"
#include "vsr/vsr_stat.h"

#include "vsr/vsr_simplex.h"
#include "gfx/gfx_process.h"
#include "temp/hyperAmt_glsl.h"
#include "temp/gfx_hyper.h"
#include "temp/gfx_displacement.h"

using namespace ctl;
using namespace gfx;
using namespace gam;
using namespace vsr;

struct MyApp : CuttleboneApp<Foo> {

  MBO* mbo;
  SamplePlayer<float, ipl::Linear, tap::Wrap> play[MAX_TOUCH];
  float gain[MAX_TOUCH];

   HyperProcess * process;
  //DisplacementProcess* process;

  //PARTICLES
  float v;

  MBO * simplexMBO;
  HyperSimplex * particleRender;

  Simplex<4> simplex;

  //Frame frame[NUMAGENTS];
  Rot rot[NUMAGENTS];
  vsr::Vec pos[NUMAGENTS];


  MyApp() : CuttleboneApp<Foo>(Layout(4, 4), 30.0) {
    for (auto& e : gain) e = 0;

    for (int i = 0; i < MAX_TOUCH; i++)
      if (!play[i].load(CLIENT_PATH SOUND_FILE)) {
        LOG("ERROR: failed to load %s", CLIENT_PATH SOUND_FILE);
        exit(1);
      }
  }

  virtual void setup() {

    // XXX
    // put this stuff after any audio loading
    Sync::master().spu(48000);
    Sound::init(256, 48000);
    Rand::Seed();

    // SPRINGMESH
    Foo state;
    vector<vector<unsigned short>> neighbor;
    vector<unsigned short> triangleIndex;
    vector<unsigned short> lineIndex;

    generateGridSpringMesh(WIDTH, HEIGHT, state, triangleIndex, lineIndex,
                           neighbor);

    Mesh mesh;
    mesh.mode(GL::L);
    // mesh.mode(GL::L);
    for (int i = 0; i < triangleIndex.size(); i++) mesh.add(triangleIndex[i]);
    // for (int i = 0; i < lineIndex.size(); i++) mesh.add(lineIndex[i]);
    for (int i = 0; i < N_VERTICES; i++) mesh.add(Vec3f(0, 0, 0));

    mbo = new MBO(mesh, GL::DYNAMIC);

    //Particles
     Mesh particlemesh;
    for (auto& i : simplex.verts ){
      Vertex v(0,0,0);
      v.Norm = Vec3f(i[0],i[1],i[2]);
      v.Col = Vec4f(i[0],i[1],i[2],i[3]);
      particlemesh.add(v);
    }

    for (auto& i : simplex.edges ){
      particlemesh.add( i.a).add(i.b);//.add(i.c);
    }

    particlemesh.mode( GL::LS );
    simplexMBO = new MBO( particlemesh );

    particleRender = new HyperSimplex(0,0,this);

       process = new HyperProcess( w-> surface.width/2, w->surface.height/2, this);
//    process = new DisplacementProcess(w->surface.width / 2, w->surface.height / 2, this);
  }

  virtual void update(float dt, Foo& state, int popCount) {

    // TIMING
    static float period = 0;
    static int count = 0;
    if (period > 1.0) {
      period -= 1.0;
      LOG("Ren: %d", count);
      count = 0;
    }
    period += dt;
    count++;

    for (int i = 0; i < MAX_TOUCH; i++) {
      static float xLast;
      Vec3f& v = renderState->position[renderState->index[i]];
      float rate = v.x - xLast;
      rate /= 30;
      gain[i] = fabs(v.z) / 5;
      gain[i] = sqrt(gain[i]);
      play[i].rate(rate);
      //LOG("%f %f", v.z, v.x - xLast);
      xLast = v.x;
    }

    // UPDATE MESH
    for (int i = 0; i < N_VERTICES; i++) {

      float t = (float)i / N_VERTICES * Rand::Num();
      mbo->mesh[i].Pos = renderState->position[i];
      float v = renderState->position[i].z;
      bool flicker = Stat::Prob(v);
      bool flicker2 = Stat::Prob(v/2.0);
      mbo->mesh[i].Col = Vec4f((flicker ? 0 : 1), v, (flicker ? 1 : 0), (flicker ? 1 : .3));
    
    }

    //Particles
    for (int i = 0; i<NUMAGENTS; ++i){
      rot[i] = Rot( state.rot[i][0], state.rot[i][1], state.rot[i][2], state.rot[i][3]);
      pos[i] = vsr::Vec( state.pos[i][0], state.pos[i][1], state.pos[i][2]);
    }
    mbo->update();

    // UPDATE SHADER PARAMETERS
    process->blur.ux = .01;
    process->blur.uy = .01;
    process->blur.amt = .5;
    //process->dispmap.amt = .5;
    //
    process -> hypmap.amt = state.time;
  }

  virtual void onDraw() {
     pipe.line(*mbo); 
  }

  void drawAgents(){
    static float val = 0.0;
    val += .1;

    pipe.begin( *simplexMBO );

      for (int i = 0; i<NUMAGENTS; ++i){
        particleRender -> bindModelView( mvm * vsr::Xf::mat(pos[i]) );
        particleRender -> program -> uniform("amt", val );
        pipe.draw(*simplexMBO);
      }

    pipe.end( *simplexMBO );
    
  }

  virtual void onFrame(){
    Renderer::clear();

    scene.updateMatrices();
    mvm = scene.xf.modelViewMatrixf();

  //  (*process)();


  //    onDraw();
//    process -> bind();   
    particleRender -> bind( scene.xf );      
        drawAgents();
    particleRender -> unbind();

     pipe.bind(scene.xf);
      onDraw();
    pipe.unbind();


    w->swapBuffers();
  }




  virtual void onSound(Sound::SoundData& io) {
    for (int i = 0; i < io.n; ++i) {
      float s = 0;
      for (int k = 0; k < MAX_TOUCH; k++) s += play[k]() * gain[k];
      for (int j = 0; j < 2; j++) {
        *io.outputBuffer++ = (short)(s * 32767.0);
      }
    }
  }
};

#endif

int main() {
  MyApp app;
  app.start();
}

void generateGridSpringMesh(int width, int height, Foo& state,
                            vector<unsigned short>& triangleIndex,
                            vector<unsigned short>& lineIndex,
                            vector<vector<unsigned short>>& neighbor) {

  neighbor.resize(width * height);

  for (int r = 0; r < height; r++)
    for (int c = 0; c < width; c++)
      state.position[indexOf(r, c)] =
          Vec3f(1.15 * (86.0f * c / width - 42.0f),
                1.2 * (56.0f * r / height - 28.0f), 0.0f);

  for (int r = 0; r < height; r++)
    for (int c = 0; c < width; c++) {
      vector<unsigned short>& target(neighbor[indexOf(r, c)]);
      if (r > 0) target.push_back(indexOf(r - 1, c));
      if (c > 0) target.push_back(indexOf(r, c - 1));
      if (r < height - 1) target.push_back(indexOf(r + 1, c));
      if (c < width - 1) target.push_back(indexOf(r, c + 1));
    }

  for (int r = 0; r < height - 1; r++) {
    for (int c = 0; c < width - 1; c++) {
      triangleIndex.push_back(indexOf(r, c));
      triangleIndex.push_back(indexOf(r + 1, c));
      triangleIndex.push_back(indexOf(r, c + 1));
      triangleIndex.push_back(indexOf(r + 1, c));
      triangleIndex.push_back(indexOf(r, c + 1));
      triangleIndex.push_back(indexOf(r + 1, c + 1));
    }
  }

  for (int r = 0; r < height - 1; r++)
    for (int c = 0; c < width - 1; c++) {
      lineIndex.push_back(indexOf(r, c));
      lineIndex.push_back(indexOf(r, c + 1));
      lineIndex.push_back(indexOf(r, c));
      lineIndex.push_back(indexOf(r + 1, c));
    }

  /*
  for (int r = 0; r < height; r++)
    for (int c = 0; c < width; c++) {
      printf("%d %d %d (%f, %f, %f) ", indexOf(r, c), r, c,
             state.position[indexOf(r, c)].x, state.position[indexOf(r, c)].y,
             state.position[indexOf(r, c)].z);
      for (auto e : neighbor[indexOf(r, c)]) printf("%d ", e);
      printf("\n");
    }
    */
}
