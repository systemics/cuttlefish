#include <unordered_map>
#include <string>
#include <iostream>
using namespace std;
int main() {
  unordered_map<string, int> map;
  map["this"] = 0;
  cout << map["this"] << endl;
}
