// run this along program along side test/taker_app.cpp
//

#include "Cuttlebone/Cuttlebone.hpp"
using namespace cuttlebone;

#ifndef _WINDOWS
#include <unistd.h> // sleep()
#else

#endif

struct State {
  int data[100];
};

struct App : MakerApp<State> {
  // this is how you would configure Maker to broadcast to your network.
  //
  // App() : MakerApp<State>("192.168.7.255") {}

  // initialize the state in some sane/useful way
  //
  virtual void init(State &state) {
    state.data[0] = 1;
    LOG("init() state.data[0] = %d", state.data[0]);
  }

  // update (modify in place) your state. after this method completes, the
  // updated state is broadcast to your network.
  //
  virtual void update(State &state) {
    LOG("update() state.data[0] = %d", state.data[0]);
    state.data[0]++;

    this_thread::sleep_for(std::chrono::microseconds(200000));
  }
};

int main() { App().start(); }
