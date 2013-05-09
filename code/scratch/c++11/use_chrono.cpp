#include <iostream>
#include <chrono>
#include <unistd.h>

//#include "ctl_stopwatch.h"

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

//using namespace std;
//using namespace std::chrono;
//
//template <typename CLOCK, typename UNIT>
//struct Stopwatch {
//  high_resolution_clock::time_point start;
//  high_resolution_clock::time_point stop;
//  duration<double> elapsed_time;
//
//  void tic() {
//    start = high_resolution_clock::now();
//  }
//
//  void toc() {
//    stop = high_resolution_clock::now();
//    elapsed_time = duration_cast<duration<double>>(stop - start);
//  }
//
//  void report() {
//    cout << elapsed_time.count() << endl;
//  }
//};

int main () {
  Stopwatch s;
  s.tic();
  usleep(1234);
  s.toc();
  s.report();
  return 0;
}
