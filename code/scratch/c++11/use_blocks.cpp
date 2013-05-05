// a "Block" is like a c function that can be defined and used "on the fly".
// the syntax to define a block looks like this:
//
//   returnType (^functionName)(type, type) = ^(type arg1, type arg2) {
//     return ...;
//   }
//
// this is actually 2 things smashed together, the declaration of the function
// with it's return type, name, and arguments here:
//
//   returnType (^functionName)(type, type);
//
// ...and the initialization (=) of the function with the definition of it's
// block with it's arguments here:
//
//   ^(type arg1, type arg2) {
//     return ...;
//   }
// 
#include <iostream>
using namespace std;

int main() {

  // 'minusOne' is a simple block that returns one less than that it's given.
  // here is the declaration:
  //
  int (^minusOne)(int);
  
  // here is the definition:
  //
  minusOne = ^(int i) {
    return i - 1;
  };

  // now we can use the minusOne block like a function
  //
  cout << "minusOne(10) = " <<  minusOne(10) << endl;

  // 'distance' is a block that calculates distance traveled
  //
  float (^distance) (float, float, float) =
    ^(float startingSpeed, float acceleration, float time) {
    return ((startingSpeed * time) + (0.5f * acceleration * time * time));
  };
  cout << "distance = " << distance(0.0f, 9.8f, 1.0f) << endl;

  // Blocks capture the value of variables in the outter scope
  //
  int captureThis = 9;
  int (^valueCapture)() = ^() {
    return captureThis;
  };
  cout << "valueCapture() = " << valueCapture() << endl;

  // blocks can capture and modify variables in an outter scope if the varables
  // are declared using __block.
  //
  __block int modifyThis = 9;
  int (^modifyCapture)() = ^() {
    return ++modifyThis;
  };
  cout << "modifyCapture() = " << modifyCapture() << endl;
  cout << "modifyCapture() = " << modifyCapture() << endl;
  cout << "modifyCapture() = " << modifyCapture() << endl;
}
