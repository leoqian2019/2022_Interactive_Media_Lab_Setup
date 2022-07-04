//#define LOG_FILE ("/tmp/log.txt")
#include "Cuttlebone/Cuttlebone.hpp"

using namespace cuttlebone;

#ifndef _WINDOWS
#include <unistd.h> // sleep()
#else

#endif

struct State {
  int frame;
  //  char _[8 * 1024 * 1024 - 4];
  char _[8];
};

int main() {
  Maker<State, 1400> maker("192.168.0.255");
  // Maker<State, 9184> maker("192.168.10.255");
  maker.shouldLog = true;
  State *state = new State;
  state->frame = 1;
  memset(state->_, 0, 8);
  maker.start();
  printf("%d bytes\n", sizeof(State));
  while (true) {
    maker.set(*state);
    state->frame++;
    std::this_thread::sleep_for(std::chrono::microseconds(50000));
  }
}
