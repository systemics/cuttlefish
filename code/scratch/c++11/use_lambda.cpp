#include <algorithm> // for_each
#include <iostream>
using namespace std;

int main() {
  int v[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  // capture by reference
  //
  auto f = [&](int i) {
    cout << i << endl;
    cout << v[0]++ << endl;
  };
  f(9);

  // capture read-only (copy?)
  //
  auto g = [=](int i) {
    cout << i << endl;
    cout << v[0] << endl;
  };

  g(9);

  for_each(&v[0], &v[0] + sizeof(v) / sizeof(v[0]),
    [](int n) {
      cout << n << " ";
    });

  cout << endl;
}
