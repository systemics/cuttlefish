#ifndef __STOPWATCH__
#define __STOPWATCH__

#include <chrono>

namespace ctl {

using namespace std;
using namespace std::chrono;

template <
  typename UNIT = milliseconds,
  typename CLOCK = high_resolution_clock,
  int N = 10
>
struct Stopwatch {
  typename CLOCK::time_point start;
  typename CLOCK::time_point stop;
  duration<UNIT> elapsed_time;

  void tic() {
    start = CLOCK::now();
  }

  void toc() {
    stop = CLOCK::now();
    elapsed_time = duration_cast<duration<UNIT>>(stop - start);
  }

  void report() {
    cout << elapsed_time.count() << N << endl;
  }
};

}

#endif
