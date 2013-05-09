#include <fstream>
#include <iostream>
using namespace std;
int main() {
  ifstream foo("/sys/class/net/eth0/address");
  char mac[256];
  foo.getline(mac, 256);
  cout << mac << endl;
}
