#ifndef __STOPWATCH__
#define __STOPWATCH__

#include <chrono>

namespace ctl {

struct Stopwatch {
  std::chrono::high_resolution_clock::time_point start;
  std::chrono::high_resolution_clock::time_point stop;
  std::chrono::duration<double> elapsed_time;

  void tic() {
    start = std::chrono::high_resolution_clock::now();
  }

  void toc() {
    stop = std::chrono::high_resolution_clock::now();
    elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(stop - start);
  }

  void report() {
    std::cout << elapsed_time.count() << std::endl;
  }
};

}

#endif
