#include "../../graphics/ctl/ctl_tictoc.h"
#include <unistd.h>
#include <iostream>

using namespace std;
using namespace ctl;

int main() {
  tic();
  usleep(12000);
  cout << toc() << endl;
}
