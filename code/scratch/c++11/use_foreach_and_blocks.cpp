// c++0x "lambda functions and expressions" are not yet available, but clang
// has Apple's "Blocks", so we can play with those.
//

#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
using namespace std;

// forEach is a template function that iterates over the given container,
// calling the given function (Block) on each element.  note that 'auto&'
// (instead of just 'auto') allows 'function' modify it's argument.  use
// forEach like this:
//
//   vector<int> v(10, 1);
//   forEach(v, ^(int& e) { e++; });
//   forEach(v, ^(int& e) { cout << e << endl; });
//
template <typename C, typename T>
void forEach(C& container, void (^function)(T&)) {
  for (auto& element : container)
    function(element);
}

// however, the prefered method is in the c++ standard library:  for_each.  get
// this by adding "#include <algorithm>" to your code.  then, use it like this:
//
//   vector<int> v(10, 1);
//   for_each(v.begin(), v.end(), ^(int& e) { e++; });
//   for_each(v.begin(), v.end(), ^(int& e) { cout << e << endl; };
//
// for_each is about like forEach, but for_each is more general because it
// accepts iterators.  forEach always iterates across the whole container,
// whereas for_each let's you choose which part of the container to work with.

int main() {

  // 'increment' is a simple block that increments a single int, in place
  //
  void (^increment)(int&) = ^(int& e) {
    e++;
  };

  // 'print' is a simple block that just prints an int to the standard out
  //
  void (^print)(int&) = ^(int& e) {
    cout << e << ' ';
  };

  // here we make a vector of 1s, increment, and print
  //
  vector<int> v(10, 0); // like MATLAB's >> v = zeros(10, 1);
  for_each(v.begin(), v.end(), increment);
  forEach(v, print);
  cout << endl;

  // same as above, but using a list<int>
  //
  list<int> l(10, 1); // like MATLAB's >> l = ones(10, 1);
  for_each(l.begin(), l.end(), increment);
  forEach(l, print);
  cout << endl;

  // same as above, but using a raw array of 1s
  //
  int a[] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
  for_each(a, a + 10, increment);
  forEach(a, print);
  cout << endl;
}
