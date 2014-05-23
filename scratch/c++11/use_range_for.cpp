#include <iostream>
using namespace std;

int main() {

  // simple array
  //
  int array[] = {1, 1, 2, 3, 5, 8, 13, 21, 34};

  // range-based for loop using type-inference (auto)
  //
  for (auto e : array)
    cout << e << ' ';
  cout << endl;

  // use auto& if you want to modify the elements of the container
  //
  for (auto& e : array) {
    e *= 2;
    cout << e << ' ';
  }
  cout << endl;

  // simple struct array definition and initialization
  //
  struct {
    int i;
    float f;
    char c;
  } triple[] = {
    {1, 2.1f, '3'},
    {2, 3.2f, '4'},
    {3, 4.3f, '5'},
  };

  // modify and print
  //
  for (auto& e : triple) {
    e.i--;
    e.f *= 3.141592f;
    e.c += 10;
    cout << '{' << e.i << ", " << e.f << ", " << e.c << '}' << endl;
  }
}
