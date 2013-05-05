#include <iostream>
using namespace std;

// base case for variadic template
//
void output() {
   cout << endl;
}

// general case for variadic template
//
template <typename T, typename ...R>
void output(T t, R ...theRest) {
  cout << t << ' ';

  // recursive call
  //
  output(theRest...);
}

struct Chaos {
  string name;
  Chaos() : name("this is crazy!") {}
  friend ostream& operator<<(ostream& out, Chaos& chaos);
};

// there might be a better way to do this
//
ostream& operator<<(ostream &out, Chaos& chaos) {
  out << chaos.name;
  return out;
}

// type inference in c++?
//
int main() {
  auto a = '$';      // my ode to perl
  auto b = "/b/";    //
  auto c = 3.141592; // what type is this?  float or double?
  auto d = 42;       //
  auto e = Chaos();  // note that e is not a pointer
  output(a, b, c, d, e);
}
