#define ICOSPHERE_FILE "/home/pi/icosphere_2.txt"
#define N_VERTICES (162)

//#define ICOSPHERE_FILE "/home/pi/icosphere_3.txt"
//#define N_VERTICES (642)

//#define ICOSPHERE_FILE "/home/pi/icosphere_4.txt"
//#define N_VERTICES (2562)

//#define ICOSPHERE_FILE "/icosphere_5.txt"
//#define N_VERTICES (10242)

//#include "gfx/gfx_matrix.h" // Vec3f

struct Vertex {
  float x, y, z;
  Vertex& operator=(const Vertex& that) {
    this->x = that.x;
    this->y = that.y;
    this->z = that.z;
    return *this;
  }
  Vertex& operator+=(const Vertex& that) {
    this->x += that.x;
    this->y += that.y;
    this->z += that.z;
    return *this;
  }
  Vertex& operator-=(const Vertex& that) {
    this->x -= that.x;
    this->y -= that.y;
    this->z -= that.z;
    return *this;
  }
  Vertex& operator*=(const float that) {
    this->x *= that;
    this->y *= that;
    this->z *= that;
    return *this;
  }
  Vertex& operator/=(const float that) {
    this->x /= that;
    this->y /= that;
    this->z /= that;
    return *this;
  }
};

Vertex operator+(const Vertex& a, const Vertex& b) {
  Vertex v{a.x + b.x, a.y + b.y, a.z + b.z};
  return v;
}

Vertex operator-(const Vertex& a, const Vertex& b) {
  Vertex v{a.x - b.x, a.y - b.y, a.z - b.z};
  return v;
}

Vertex operator*(const Vertex& a, const float b) {
  Vertex v{a.x* b, a.y* b, a.z* b};
  return v;
}

struct Foo {
  float time;
  Vertex position[N_VERTICES];
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

#define SK (0.01f)
#define NK (0.1f)
#define D (0.97)
//.01, .1, .765

struct MyApp : Simulator<Foo> {
  MyApp() : Simulator<Foo>("192.168.7.255", 1 / 30.0f) {}

  float d, sk, nk;
  Foo original;
  Vertex velocity[N_VERTICES];
  vector<vector<unsigned short> > neighbor;

  virtual void setup(Foo& state) {
    memset(&state, 0, sizeof(state));
    vector<unsigned short> tri, lin;
    load(ICOSPHERE_FILE, state, tri, lin, neighbor);
    memcpy(&original, &state, sizeof(Foo));
    memset(&velocity, 0, sizeof(Vertex) * N_VERTICES);
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
      Vertex v{r(), r(), r()};
      v *= 4.0f;
      int randomVertex = rand() % N_VERTICES;
      state.position[randomVertex] += v;
      v *= 0.7f;
      for (auto n : neighbor[randomVertex]) state.position[n] += v;
      LOG("poke!");
    }
    n++;

    for (int i = 0; i < N_VERTICES; i++) {
      Vertex& v = state.position[i];
      Vertex force = (v - original.position[i]) * -SK;

      for (int k = 0; k < neighbor[i].size(); k++) {
        Vertex& n = state.position[neighbor[i][k]];
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

  MBO* circle;

  MyApp() : CuttleboneApp<Foo>(Layout(4, 4), 30.0) {}

  virtual void setup() { circle = new MBO(Mesh::Circle(0.1f)); }

  virtual void update(float dt, Foo& state, int popCount) {
  }

  virtual void onDraw() {
    pipe.begin(*circle);
    for (int i = 0; i < N_VERTICES; i++) {
      Vec3f p(renderState->position[i].x, renderState->position[i].y, renderState->position[i].z - 1);
      p *= 57.0;
      modelview(gfx::XMat::trans(p));
      pipe.draw(*circle);
    }
    pipe.end(*circle);
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
