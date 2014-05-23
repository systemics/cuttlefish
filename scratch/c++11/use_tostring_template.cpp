#include <iostream>
using namespace std;

// this template will force "cout << object" to call object.toString(), so if
// object does not have a toString() method, you'll get a compiler error.  this
// might make Java people feel more at home.
//
template <typename T>
ostream& operator<<(ostream &out, T& t) {
  return out << t.toString();
}

struct Foo {
  string toString() {
    return "oh, yeah!";
  }
};

int main() {
  Foo f;
  cout << f << endl;
}
