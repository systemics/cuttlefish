#ifndef __STOPWATCH__
#define __STOPWATCH__

#include <chrono>

namespace ctl {

using namespace std;
using namespace std::chrono;

template <
  typename DATA,
  int N
>
struct Average {
  DATA data[N];
  DATA sum;
  unsigned index;
  Average() : sum(0), index(0) {}
  DATA operator()() {
    return sum / N;
  }
  void operator()(DATA d) {
    sum += d;
    sum -= data[index];
    data[index] = d;
    index++;
    if (index == N)
      index = 0;
  }
};



template <
  typename UNIT = milliseconds,
  typename CLOCK = high_resolution_clock,
  int N = 20
>
struct Stopwatch {
  typename CLOCK::time_point start;
  typename CLOCK::time_point stop;
  Average<float, N> average;

  void tic() {
    start = CLOCK::now();
  }

  void toc() {
    stop = CLOCK::now();
    UNIT elapsed_time = duration_cast<UNIT>(stop - start);
    average(elapsed_time.count());
  }

  void report() {
    cout << average() << endl;
  }
};

}

#endif
