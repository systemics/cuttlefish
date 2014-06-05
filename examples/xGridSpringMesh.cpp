#define SERVER_PATH "/home/ky/"
//#define SERVER_PATH "/Users/ky/code/cuttlefish/"
#define CLIENT_PATH "/home/pi/"

//#define SOUND_FILE "czonic.wav"
#define SOUND_FILE "distort.wav"
//#define SOUND_FILE "FLANNEL1.wav"

#include "gfx/gfx_matrix.h"

#define MAX_TOUCH (10)

#define MULTIPLY (4)
#define WIDTH (16 * MULTIPLY)
#define HEIGHT (9 * MULTIPLY)
#define N_VERTICES (WIDTH * HEIGHT)

struct Foo {
  float time;
  gfx::Vec3f position[N_VERTICES];
  gfx::Vec2f touch[MAX_TOUCH];
  int touchCount;
};

#include <vector>
using namespace std;

void generateGridSpringMesh(int width, int height, Foo& state,
                            vector<unsigned short>& triangleIndex,
                            vector<unsigned short>& lineIndex,
                            vector<vector<unsigned short>>& neighbor);

float r() { return 2.0f * rand() / RAND_MAX - 1.0f; }

#if SIM

#include "ctl_bone.h"
//#include "ctl_touch.h"
#include <string.h>

using namespace ctl;
using namespace gfx;

#define SK (0.02f)
#define NK (0.05f)
#define D (0.95)
//#define SK (0.02f)
//#define NK (0.05f)
//#define D (0.98)
//.01, .1, .765

struct MyApp : Simulator<Foo> {  // , Touch {
  MyApp() : Simulator<Foo>("192.168.7.255") {} //, 1 / 30.0f) {}

  float d, sk, nk;
  Vec3f stationary[N_VERTICES];
  Vec3f velocity[N_VERTICES];
  vector<vector<unsigned short>> neighbor;

  virtual void setup(Foo& state) {
    //    Touch::setup("/dev/input/event2");

    memset(&state, 0, sizeof(state));
    memset(&stationary, 0, sizeof(Vec3f) * N_VERTICES);
    memset(&velocity, 0, sizeof(Vec3f) * N_VERTICES);

    vector<unsigned short> triangleIndex, lineIndex;
    generateGridSpringMesh(WIDTH, HEIGHT, state, triangleIndex, lineIndex,
                           neighbor);
    memcpy(&stationary, state.position, sizeof(Vec3f) * N_VERTICES);
  }

  virtual void update(float dt, Foo& state) {

    /*
        pollTouches();
        int k = 0;
        for (auto& e : touchPoint)
          if (e.id != 0) {
            state.touch[k++] = Vec2f(e.x / 3200.0f, e.y / -2600.0f);
            if (k >= MAX_TOUCH) break;
          }
        state.touchCount = k;
        for (int i = 0; i < state.touchCount; ++i)
          LOG("%d x:%f y:%f", i, state.touch[0].x, state.touch[0].y);
    */

    state.time += dt;

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
  }
};

#else

#include "ctl_app.h"
#include "Gamma/Gamma.h"
#include "Gamma/Oscillator.h"
#include "Gamma/SamplePlayer.h"

using namespace ctl;
using namespace gfx;
using namespace gam;

struct MyApp : CuttleboneApp<Foo> {

  MBO* mbo;
  SineD<float> bonk;
  SamplePlayer<float, ipl::Cubic, tap::Wrap> play;
  float gain;

  MyApp() : CuttleboneApp<Foo>(Layout(4, 4), 30.0) {
    Sound::init(256, 48000);
    Sync::master().spu(48000);
    gain = 0;
    // bonk.set(440.0f, 0.0f, 0.0f);
    if (!play.load(CLIENT_PATH SOUND_FILE)) {
      LOG("ERROR: failed to load %s", CLIENT_PATH SOUND_FILE);
      exit(1);
    }
  }

  virtual void setup() {

    Foo state;
    vector<vector<unsigned short>> neighbor;
    vector<unsigned short> triangleIndex;
    vector<unsigned short> lineIndex;

    generateGridSpringMesh(WIDTH, HEIGHT, state, triangleIndex, lineIndex,
                           neighbor);

    Mesh mesh;
    mesh.mode(GL::L);
    for (int i = 0; i < lineIndex.size(); i++) mesh.add(lineIndex[i]);
    for (int i = 0; i < N_VERTICES; i++) mesh.add(Vec3f(0, 0, 0));

    mbo = new MBO(mesh);
  }

  virtual void update(float dt, Foo& state, int popCount) {
    static float period = 0;
    static int count = 0;
    if (period > 1.0) {
      period -= 1.0;
      LOG("Ren: %d", count);
      count = 0;
      bonk.set(440.0f, 0.8f, 0.5f);
    }
    period += dt;
    count++;

    if (renderState->touchCount) {
      gain = renderState->touch[0].y / 2 + 0.5;
      play.rate(renderState->touch[0].x);
    }

    for (int i = 0; i < N_VERTICES; i++)
      mbo->mesh[i].Pos =
          Vec3f(renderState->position[i].x, renderState->position[i].y,
                renderState->position[i].z);
    mbo->update();
  }

  virtual void onDraw() { pipe.line(*mbo); }

  virtual void onSound(Sound::SoundData& io) {
    for (int i = 0; i < io.n; ++i) {
      float s = play();
      for (int j = 0; j < 2; j++) {
        *io.outputBuffer++ = (short)(s * 32767.0) * gain;
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

  auto indexOf = [=](unsigned y, unsigned x) { return y * width + x; };

  for (int r = 0; r < height; r++)
    for (int c = 0; c < width; c++)
      state.position[indexOf(r, c)] =
          Vec3f(1.15 * (86.0f * c / width - 42.0f), 1.2 * (56.0f * r / height - 28.0f), 0.0f);

  for (int r = 0; r < height; r++)
    for (int c = 0; c < width; c++) {
      vector<unsigned short>& target(neighbor[indexOf(r, c)]);
      if (r > 0) target.push_back(indexOf(r - 1, c));
      if (c > 0) target.push_back(indexOf(r, c - 1));
      if (r < height - 1) target.push_back(indexOf(r + 1, c));
      if (c < width - 1) target.push_back(indexOf(r, c + 1));
    }

  for (int r = 0; r < height - 1; r++)
    for (int c = 0; c < width - 1; c++) {
      lineIndex.push_back(indexOf(r, c));
      lineIndex.push_back(indexOf(r, c + 1));
      lineIndex.push_back(indexOf(r, c));
      lineIndex.push_back(indexOf(r + 1, c));
    }

//  // GL_LINES
//  for (int r = 0; r < height - 1; r++)
//    for (int c = 0; c < width - 1; c++) {
//      lineIndex.push_back(indexOf(r, c));
//      lineIndex.push_back(indexOf(r, c + 1));
//    }
//  for (int c = 0; c < width - 1; c++)
//    for (int r = 0; r < height - 1; r++) {
//      lineIndex.push_back(indexOf(r, c));
//      lineIndex.push_back(indexOf(r + 1, c));
//    }

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
