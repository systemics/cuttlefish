#include "ctl_sim.h"
#include "ctl_touch.h"
#include "gfx/gfx_matrix.h"
#include "Cuttlebone/Cuttlebone.hpp"
#include "state.hpp"

using namespace gfx; // Vec2f

#define FLOOR(v) ((unsigned)(v) - ((v) < 0.0 && (v) != (unsigned)(v)))
#define FRAC(v) (((v) >= 0.0) ? (v) - (unsigned)(v) : (-v) - (unsigned)(v))
//#define CLAMP(v, bottom, top) ((v < bottom) ? bottom : ((v > top) ? top : v))

struct Grid {
  float* data;
  unsigned size, C, R;
  Grid(unsigned c, unsigned r) : C(c), R(r) {
    size = C * R;
    data = new float[size];
    memset(data, 0, size * sizeof(float));
    for (unsigned i = 0; i < size; ++i)
      data[i] = 10;
      //data[i] = (float)i / size;
  }

  inline float& cell(unsigned c, unsigned r) { return data[c * R + r]; }

  float read(Vec2f& where) {
    where.x = C * (where.x + 1)/2;
    where.y = R * (where.y + 1)/2;
    unsigned xa = FLOOR(where.x);
    unsigned xb = xa + 1;
    unsigned ya = FLOOR(where.y);
    unsigned yb = ya + 1;
    float xbf = FRAC(where.x);
    float xaf = 1.f - xbf;
    float ybf = FRAC(where.y);
    float yaf = 1.f - ybf;
    float faaa = xaf * yaf;
    float faba = xaf * ybf;
    float fbaa = xbf * yaf;
    float fbba = xbf * ybf;
    float& paaa = cell(xa, ya);
    float& paba = cell(xa, yb);
    float& pbaa = cell(xb, ya);
    float& pbba = cell(xb, yb);
    return (paaa * faaa) + (pbaa * fbaa) + (paba * faba) + (pbba * fbba);
  }

  void write(Vec2f where, float f) {
    where.x = C * (where.x + 1)/2;
    where.y = R * (where.y + 1)/2;
    unsigned xa = FLOOR(where.x);
    unsigned xb = xa + 1;
    unsigned ya = FLOOR(where.y);
    unsigned yb = ya + 1;
    float xbf = FRAC(where.x);
    float xaf = 1.f - xbf;
    float ybf = FRAC(where.y);
    float yaf = 1.f - ybf;
    float faaa = xaf * yaf;
    float faba = xaf * ybf;
    float fbaa = xbf * yaf;
    float fbba = xbf * ybf;
    float& paaa = cell(xa, ya);
    float& paba = cell(xa, yb);
    float& pbaa = cell(xb, ya);
    float& pbba = cell(xb, yb);
    paaa *= 0; // f * faaa;
    pbaa *= 0; // f * fbaa;
    paba *= 0; // f * faba;
    pbba *= 0; // f * fbba;
  }
};

struct MyApp : SimApp<State>, ctl::Touch {

  Grid* grid;

  virtual void onSetup() {
    Touch::setup("/dev/input/event2");

    grid = new Grid(16, 9);
  }

  virtual void onSimulate(double dt) {
    static cuttlebone::Stats stats;
    stats(dt);
    static double t;
    t += dt;


    int N = 5;
    Vec2f touch[N];
    pollTouches();
    int k = 0;
    for (auto& e : touchPoint)
      if (e.id != 0) {
        touch[k] = Vec2f(CLAMP(e.x / 3200.0f, -1.0, 1.0), CLAMP(e.y / -2600.0f, -1.0, 1.0));
        k++;
        if (k >= N) break;
      }
    if (k) {
      float f = grid->read(touch[0]);
      LOG("%f", f);
      grid->write(touch[0], 0.0);
    }

    state->phase++;
    //LOG("phase:%d time:%lf", state->phase, t);
    //LOG("phase:%d", state->phase);
    usleep(16667);
  }
};

int main() { MyApp().start(true); } // true starts a main loop
