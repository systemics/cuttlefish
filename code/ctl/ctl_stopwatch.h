#ifndef __STOPWATCH__
#define __STOPWATCH__

#include <chrono>
#include <limits>

namespace ctl {

using namespace std;
using namespace std::chrono;

template <typename TYPE, unsigned LENGTH>
struct MinMeanMax {
  TYPE data[LENGTH];
  TYPE sum, min, max;
  unsigned index;

  MinMeanMax() : sum(0), index(0), min(numeric_limits<TYPE>::max()), max(numeric_limits<TYPE>::min()) {}

  void operator()(TYPE d) {
    if (d < min)
      min = d;
    if (d > max)
      max = d;
    sum += d;
    sum -= data[index];
    data[index] = d;
    index++;
    if (index == LENGTH)
      index = 0;
  }

  void report() {
    cout << "(" << min << ", " << sum / LENGTH << ", " << max << ")\n";
  }
};

template <
  typename UNIT = milliseconds,
  typename CLOCK = high_resolution_clock
>
struct StopWatch {
  typename CLOCK::time_point before, after;
  MinMeanMax<float, 10> minMeanMax;

  void start() {
    before = CLOCK::now();
  }

  void stop() {
    after = CLOCK::now();
    UNIT elapsed_time = duration_cast<UNIT>(after - before);
    minMeanMax(elapsed_time.count());
  }

  void report() {
    minMeanMax.report();
  }
};

}

#endif
