#ifndef __GRID__
#define __GRID__

#include "gfx/gfx_matrix.h" // Vec2f

#define __FLOOR(v) ((unsigned)(v) - ((v) < 0.0 && (v) != (unsigned)(v)))
#define __FRAC(v) (((v) >= 0.0) ? (v) - (unsigned)(v) : (-v) - (unsigned)(v))
#define __CLAMP(v, bottom, top) ((v < bottom) ? bottom : ((v > top) ? top : v))

struct Grid {
  float* data;
  unsigned size, C, R;
  Grid(unsigned c, unsigned r) : C(c), R(r) {
    size = C * R;
    data = new float[size];
    memset(data, 0, size * sizeof(float));
    // for (unsigned i = 0; i < size; ++i) data[i] = 1.0f;
    // data[i] = (float)i / size;
  }

  void print() {
    int n = 0;
    for (unsigned i = 0; i < size; ++i) {
      printf("%.2f ", data[i]);
      n++;
      if (n == C) {
        n = 0;
        printf("\n");
      }
    }
    printf("\n");
  }

  inline float& cell(unsigned c, unsigned r) { return data[r * C + c]; }

  float read(const gfx::Vec2f& where) {
    float x = C * where.x;
    float y = R * where.y;
    unsigned xa = __FLOOR(x);
    unsigned xb = xa + 1;
    unsigned ya = __FLOOR(y);
    unsigned yb = ya + 1;
    float xbf = __FRAC(x);
    float xaf = 1.f - xbf;
    float ybf = __FRAC(y);
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

  void add(const gfx::Vec2f& where, float f) {
    float x = C * where.x;
    float y = R * where.y;
    unsigned xa = __FLOOR(x);
    unsigned xb = xa + 1;
    unsigned ya = __FLOOR(y);
    unsigned yb = ya + 1;
    float xbf = __FRAC(x);
    float xaf = 1.f - xbf;
    float ybf = __FRAC(y);
    float yaf = 1.f - ybf;
    float faaa = xaf * yaf;
    float faba = xaf * ybf;
    float fbaa = xbf * yaf;
    float fbba = xbf * ybf;
    float& paaa = cell(xa, ya);
    float& paba = cell(xa, yb);
    float& pbaa = cell(xb, ya);
    float& pbba = cell(xb, yb);
    paaa += f * faaa;
    pbaa += f * fbaa;
    paba += f * faba;
    pbba += f * fbba;
    paaa = __CLAMP(paaa, 0.0f, 1.0f);
    pbaa = __CLAMP(pbaa, 0.0f, 1.0f);
    paba = __CLAMP(paba, 0.0f, 1.0f);
    pbba = __CLAMP(pbba, 0.0f, 1.0f);
  }
};

#endif

/*
int main() {
  Grid grid(10, 10);
  Vec2f v(0.5, 0.5);

  float f;
  grid.print();

  cout << "------------------------------" << endl;
  f = grid.read(v);
  grid.add(v, 0.5);
  grid.print();
  cout << "value: " << grid.read(v) << endl;

  cout << "------------------------------" << endl;
  f = grid.read(v);
  grid.add(v, -0.1);
  grid.print();
  cout << "value: " << grid.read(v) << endl;

  cout << "------------------------------" << endl;
  f = grid.read(v);
  grid.add(v, -0.1);
  grid.print();
  cout << "value: " << grid.read(v) << endl;
}
*/
