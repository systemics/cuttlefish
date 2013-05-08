#include <iostream>
#include <chrono>

//#include "ctl_stopwatch.h"

using namespace std;
using namespace std::chrono;

template <typename CLOCK, typename UNIT>
struct Stopwatch {
  high_resolution_clock::time_point start;
  high_resolution_clock::time_point stop;
  duration<double> elapsed_time;

  void tic() {
    start = high_resolution_clock::now();
  }

  void toc() {
    stop = high_resolution_clock::now();
    elapsed_time = duration_cast<duration<double>>(stop - start);
  }

  void report() {
    cout << elapsed_time.count() << endl;
  }
};

int main () {
  Stopwatch<double, high_resolution_clock> s;
  s.tic();
  usleep(1234);
  s.toc();
  s.report();
  return 0;
}
