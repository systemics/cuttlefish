#ifndef __CTL_TICTOC__
#define __CTL_TICTOC__

#include <chrono>
using namespace std;
using namespace std::chrono;

namespace ctl {

typedef duration<float> TIME_UNIT;
//typedef duration<float, milli> TIME_UNIT;

template <typename CLOCK = high_resolution_clock>
struct TicToc {
  time_point<CLOCK> before, after;

  void tic() {
    before = CLOCK::now();
  }

  float toc() {
    after = CLOCK::now();
    return duration_cast<TIME_UNIT>(after - before).count();
  }
};

TicToc<> _ticToc;

void tic() {
  _ticToc.tic();
}

float toc() {
  return _ticToc.toc();
}

} // namespace ctl;

#endif
