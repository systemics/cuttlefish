#include <iostream>
using namespace std;

template <typename T>
void foo(T t) {
  cout << t << ' ';
}

template <typename F, typename A>
void forEach(F function, A argument) {
  function(argument);
}

template <typename F, typename A, typename... R>
void forEach(F function, A argument, R ...theRest) {
  function(argument);
  forEach(function, theRest...);
}

int main() {
  foo('f');
  foo("akjhsdkasd");
  foo(1.6f);
  forEach(foo, 1, 1.61803399f, 'f', "a string");
  cout << endl;

  // these would make compilation fail with an error
  //
  // forEach();
  // forEach(foo);
}
