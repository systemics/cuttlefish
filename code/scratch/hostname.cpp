#include <unistd.h>
#include <map>
#include <assert.h>
#include <iostream>
#include <string>
using namespace std;

int identifier;

int main() {

  map<string, int> idOf;

  int n = 1;
  idOf["pi-b"] = n++;
  idOf["pi-c"] = n++;
  idOf["pi-l"] = n++;
  idOf["pi-r"] = n++;

  char hostname[100];
  assert(gethostname(hostname, 100) == 0);
  identifier = idOf[hostname];

  cout << hostname << ", " << identifier << endl;
}
