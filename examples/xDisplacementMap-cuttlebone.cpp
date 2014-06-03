#define SERVER_PATH "/Users/ky/code/cuttlefish/"
#define CLIENT_PATH "/home/pi/"

//#define ICOSPHERE_FILE "icosphere_2.txt"
//#define N_VERTICES (162)

//#define ICOSPHERE_FILE "icosphere_3.txt"
//#define N_VERTICES (642)

#define ICOSPHERE_FILE "icosphere_4.txt"
#define N_VERTICES (2562)

//#define ICOSPHERE_FILE "icosphere_5.txt"
//#define N_VERTICES (10242)

#include "gfx/gfx_matrix.h"

#include "temp/gfx_displacement.h"

struct Foo {
  float time;
  gfx::Vec3f position[N_VERTICES];
};

#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

float r() { return 2.0f * rand() / RAND_MAX - 1.0f; }
bool load(string fileName, Foo& state, vector<unsigned short>& triangleIndex,
          vector<unsigned short>& lineIndex,
          vector<vector<unsigned short> >& neighbor);

#if SIM

#include "ctl_bone.h"

using namespace ctl;
using namespace gfx;

#define SK (0.01f)
#define NK (0.1f)
#define D (0.93)
//#define D (0.97)
//.01, .1, .765

struct MyApp : Simulator<Foo> {
  MyApp() : Simulator<Foo>("192.168.7.255", 1 / 30.0f) {}

  float d, sk, nk;
  Foo original;
  Vec3f velocity[N_VERTICES];
  vector<vector<unsigned short> > neighbor;

  virtual void setup(Foo& state) {
    memset(&state, 0, sizeof(state));
    vector<unsigned short> tri, lin;
    if (!load(SERVER_PATH ICOSPHERE_FILE, state, tri, lin, neighbor)) {
      LOG("ERROR: failed to load %s", SERVER_PATH ICOSPHERE_FILE);
      exit(1);
    }
    memcpy(&original, &state, sizeof(Foo));
    memset(&velocity, 0, sizeof(Vec3f) * N_VERTICES);
  }

  virtual void update(float dt, Foo& state) {
    static float period = 0;
    static int simCount = 0;
    if (period > 1.0) {
      period -= 1.0;
      LOG("Sim: %d", simCount);
      simCount = 0;
    }
    period += dt;
    simCount++;

    static int n = 0;

    if ((n % 300) == 0) {
      Vec3f v{r(), r(), r()};
      v *= 4.0f;
      int randomVec3f = rand() % N_VERTICES;
      state.position[randomVec3f] += v;
      v *= 0.7f;
      for (auto n : neighbor[randomVec3f]) state.position[n] += v;
      LOG("poke!");
    }
    n++;

    for (int i = 0; i < N_VERTICES; i++) {
      Vec3f& v = state.position[i];
      Vec3f force = (v - original.position[i]) * -SK;

      for (int k = 0; k < neighbor[i].size(); k++) {
        Vec3f& n = state.position[neighbor[i][k]];
        force += (v - n) * -NK;
      }

      velocity[i] += force;
      velocity[i] *= D;
    }

    for (int i = 0; i < N_VERTICES; i++) state.position[i] += velocity[i];
    /*
    */
  }
};

#else

#include "ctl_app.h"

using namespace ctl;
using namespace gfx;

struct MyApp : CuttleboneApp<Foo> {

  MBO* lines;

  DisplacmentProcess * process;

  MyApp() : CuttleboneApp<Foo>(Layout(4, 4), 30.0) {}

  virtual void setup() {

    Foo dummyFoo;
    vector<vector<unsigned short> > dummyNeighbors;
    vector<unsigned short> triangleIndex;
    vector<unsigned short> lineIndex;

    if (!load(CLIENT_PATH ICOSPHERE_FILE, dummyFoo, triangleIndex, lineIndex,
              dummyNeighbors)) {
      LOG("ERROR: failed to load %s", CLIENT_PATH ICOSPHERE_FILE);
      exit(1);
    }

    Mesh ico;
    for (int i = 0; i < lineIndex.size(); i++) ico.add(lineIndex[i]);
    for (int i = 0; i < N_VERTICES; i++) ico.add(Vec3f(0, 0, 0));

    lines = new MBO(ico);

  }

  virtual void update(float dt, Foo& state, int popCount) {
    static float period = 0;
    static int count = 0;
    if (period > 1.0) {
      period -= 1.0;
      LOG("Ren: %d", count);
      count = 0;
    }
    period += dt;
    count++;

    for (int i = 0; i < N_VERTICES; i++) {
      Vec3f p(renderState->position[i].x * 150,
              renderState->position[i].y * 150, renderState->position[i].z);
      lines->mesh[i].Pos = p;
    }
    lines->update();

    //GFX PROCESS
    process -> blur.ux = sin(time);
    process -> blur.uy = cos(time);
    process -> blur.amt = fabs(sin(time));
    process -> dispmap.amt = sin(time);

  }

  virtual void onDraw() { 
    
    pipe.line(*lines); 
  
  }

  virtual void onFrame(){
     update();          
     Renderer::clear();
    
     (*process)();
     //Renderer::render(); 
    
     w -> swapBuffers(); 
  }

  virtual void onSound(Sound::SoundData& io) {}
};

#endif

int main() { MyApp().start(); }

bool load(string fileName, Foo& state, vector<unsigned short>& triangleIndex,
          vector<unsigned short>& lineIndex,
          vector<vector<unsigned short> >& neighbor) {

  ifstream file(fileName);
  if (!file.is_open()) return false;

  string line;
  int n = 0;
  int readState = 0;
  while (getline(file, line)) {
    if (line == "|") {
      readState++;
      continue;
    }
    switch (readState) {
      case 0: {
        vector<float> v;
        stringstream ss(line);
        float f;
        while (ss >> f) {
          v.push_back(f);
          if (ss.peek() == ',') ss.ignore();
        }
        state.position[n].x = v[0];
        state.position[n].y = v[1];
        state.position[n].z = v[2];
        n++;
        // cout << v[0] << "|" << v[1] << "|" << v[2] << endl;
      } break;

      case 1: {
        stringstream ss(line);
        int i;
        if (ss >> i) {
          triangleIndex.push_back(i);

          static int c = 0, last;
          if (c % 3) lineIndex.push_back(last);
          lineIndex.push_back(i);
          last = i;
          c++;
        } else
          return false;
        // cout << i << endl;
      } break;

      case 2: {
        vector<unsigned short> v;
        stringstream ss(line);
        int i;
        while (ss >> i) {
          v.push_back(i);
          if (ss.peek() == ',') ss.ignore();
        }
        if ((v.size() != 5) && (v.size() != 6)) return false;
        neighbor.push_back(v);
      } break;
    }
  }
  file.close();

  return true;
}

