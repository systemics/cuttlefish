#define SERVER_PATH "/home/ky/"
//#define SERVER_PATH "/Users/ky/code/cuttlefish/"
#define CLIENT_PATH "/home/pi/"

//#define SOUND_FILE "czonic.wav"
#define SOUND_FILE "distort.wav"
//#define SOUND_FILE "FLANNEL1.wav"

#include "gfx/gfx_matrix.h"

#define MAX_TOUCH (10)

#define WIDTH (16)
#define HEIGHT (9)
#define N_VERTICES (WIDTH* HEIGHT)

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
#include "ctl_touch.h"
#include <string.h>

using namespace ctl;
using namespace gfx;

#define SK (0.01f)
#define NK (0.1f)
#define D (0.93)
//#define D (0.97)
//.01, .1, .765

struct MyApp : Simulator<Foo>, Touch {
  MyApp() : Simulator<Foo>("192.168.7.255", 1 / 30.0f) {}

  float d, sk, nk;
  Vec3f stationary[N_VERTICES];
  Vec3f velocity[N_VERTICES];
  vector<vector<unsigned short>> neighbor;

  virtual void setup(Foo& state) {
    //    Touch::setup("/dev/input/event2");

    LOG("setup()");

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
      Vec3f force = (v - stationary[i]) * -SK;

      for (int k = 0; k < neighbor[i].size(); k++) {
        Vec3f& n = state.position[neighbor[i][k]];
        force += (v - n) * -NK;
      }

      velocity[i] += force;
      velocity[i] *= D;
    }

    for (int i = 0; i < N_VERTICES; i++) state.position[i] += velocity[i];
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

  MBO* lines;
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
      bonk.set(440.0f, 0.8f, 0.5f);
    }
    period += dt;
    count++;

    if (renderState->touchCount) {
      gain = renderState->touch[0].y / 2 + 0.5;
      play.rate(renderState->touch[0].x);
    }

    for (int i = 0; i < N_VERTICES; i++) {
      Vec3f p(renderState->position[i].x * 150,
              renderState->position[i].y * 150, renderState->position[i].z);
      lines->mesh[i].Pos = p;
    }
    lines->update();
  }

  virtual void onDraw() { pipe.line(*lines); }

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
  LOG("got here");

  MyApp().start();
}

void generateGridSpringMesh(int width, int height, Foo& state,
                            vector<unsigned short>& triangleIndex,
                            vector<unsigned short>& lineIndex,
                            vector<vector<unsigned short>>& neighbor) {

#define indexOf(x, y) (x* height + y)

  for (int c = 0; c < width; c++) {
    for (int r = 0; r < height; r++) {
      state.position[indexOf(r, c)] =
          Vec3f(86.0f * c / width - 42.0f, 56.0f * r / height - 28.0f, 0.0f);
    }
  }

  // 4 neighbors
  for (int c = 1; c < width - 1; c++) {
    for (int r = 1; r < height - 1; r++) {
      vector<unsigned short>& target(neighbor[indexOf(r, c)]);
      target.push_back(indexOf(r - 1, c));
      target.push_back(indexOf(r, c - 1));
      target.push_back(indexOf(r + 1, c));
      target.push_back(indexOf(r, c + 1));
    }
  }

  // 3 neighbors
  for (int c = 1; c < width - 1; c++) {
    int r = 0;
    vector<unsigned short>& target(neighbor[indexOf(r, c)]);
    // target.push_back(indexOf(r - 1, c));
    target.push_back(indexOf(r, c - 1));
    target.push_back(indexOf(r + 1, c));
    target.push_back(indexOf(r, c + 1));
  }

  for (int c = 1; c < width - 1; c++) {
    int r = height - 1;
    vector<unsigned short>& target(neighbor[indexOf(r, c)]);
    target.push_back(indexOf(r - 1, c));
    target.push_back(indexOf(r, c - 1));
    // target.push_back(indexOf(r + 1, c));
    target.push_back(indexOf(r, c + 1));
  }

  for (int r = 1; r < height - 1; r++) {
    int c = 0;
    vector<unsigned short>& target(neighbor[indexOf(r, c)]);
    target.push_back(indexOf(r - 1, c));
    // target.push_back(indexOf(r, c - 1));
    target.push_back(indexOf(r + 1, c));
    target.push_back(indexOf(r, c + 1));
  }

  for (int r = 1; r < height - 1; r++) {
    int c = width - 1;
    vector<unsigned short>& target(neighbor[indexOf(r, c)]);
    target.push_back(indexOf(r - 1, c));
    target.push_back(indexOf(r, c - 1));
    target.push_back(indexOf(r + 1, c));
    // target.push_back(indexOf(r, c + 1));
  }

  {
    int r = 0, c = 0;
    vector<unsigned short>& target(neighbor[indexOf(r, c)]);
    // target.push_back(indexOf(r - 1, c));
    // target.push_back(indexOf(r, c - 1));
    target.push_back(indexOf(r + 1, c));
    target.push_back(indexOf(r, c + 1));
  }

  {
    int r = width - 1, c = 0;
    vector<unsigned short>& target(neighbor[indexOf(r, c)]);
    target.push_back(indexOf(r - 1, c));
    // target.push_back(indexOf(r, c - 1));
    // target.push_back(indexOf(r + 1, c));
    target.push_back(indexOf(r, c + 1));
  }

  {
    int r = 0, c = height - 1;
    vector<unsigned short>& target(neighbor[indexOf(r, c)]);
    // target.push_back(indexOf(r - 1, c));
    target.push_back(indexOf(r, c - 1));
    target.push_back(indexOf(r + 1, c));
    // target.push_back(indexOf(r, c + 1));
  }

  {
    int r = width - 1, c = height - 1;
    vector<unsigned short>& target(neighbor[indexOf(r, c)]);
    target.push_back(indexOf(r - 1, c));
    target.push_back(indexOf(r, c - 1));
    // target.push_back(indexOf(r + 1, c));
    // target.push_back(indexOf(r, c + 1));
  }
}
