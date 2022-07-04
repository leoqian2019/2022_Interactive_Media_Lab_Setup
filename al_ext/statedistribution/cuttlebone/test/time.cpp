#include "Cuttlebone/Time.hpp"
#include <chrono>
#include <iostream> // cout, endl
#include <thread>
using namespace std;

using namespace cuttlebone;

int main() {
  Timestamp<> t;
  cout << t.stamp() << endl;
  Stopwatch<> s;
  for (int i = 0; i < 100; ++i) {
    s.tic();
    this_thread::sleep_for(std::chrono::microseconds(1000));
    cout << s.toc() << endl;
  }
}
