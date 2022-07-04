#include "Cuttlebone/Queue.hpp"

#ifndef _WINDOWS
#include <unistd.h> // usleep
#else
#endif
#include <iostream> // cout
#include <string.h>
#include <thread>
using namespace std;

using namespace cuttlebone;

//#include <cstddef>
//#include <string>
//#include <stdlib.h>

// must be a POD type
//
struct State {
  unsigned frame;
  char data[256];
  void zero() { memset(this, 0, sizeof(State)); }
};

int main(int argc, char *argv[]) {
  Queue<State> simulateBroadcast, receiveRender;
  thread simulate, broadcast, receive, render;

  unsigned simulateBroadcastWasFull = 0;
  unsigned simulateBroadcastWasEmpty = 0;
  unsigned receiveRenderWasFull = 0;
  unsigned receiveRenderWasEmpty = 0;

  bool waitingToStart = true, done = false;

  simulate = thread([&]() {
    State s;
    s.zero();
    while (waitingToStart)
      this_thread::sleep_for(std::chrono::microseconds(1000));
    while (!done) {
      sprintf(s.data, "%u", s.frame);
      if (simulateBroadcast.push(s))
        s.frame++;
      else
        simulateBroadcastWasFull++;
      this_thread::sleep_for(std::chrono::microseconds(16666));
    }
  });

  broadcast = thread([&]() {
    State s;
    while (waitingToStart)
      this_thread::sleep_for(std::chrono::microseconds(1000));
    while (!done) {
      if (simulateBroadcast.pop(s)) {
        // cut s into pieces and broadcast
      } else
        simulateBroadcastWasEmpty++;
      this_thread::sleep_for(std::chrono::microseconds(16666));
    }
  });

  receive = thread([&]() {
    State s;
    while (waitingToStart)
      this_thread::sleep_for(std::chrono::microseconds(1000));
    while (!done) {
      // select with a small timeout
      // receive lots of little pieces of s and assemble
      // indicate when we lost a piece
      // when s is ready, then push
      if (!receiveRender.push(s))
        receiveRenderWasFull++;
      this_thread::sleep_for(std::chrono::microseconds(1000));
    }
  });

  render = thread([&]() {
    State s;
    while (waitingToStart)
      this_thread::sleep_for(std::chrono::microseconds(1000));
    while (!done) {
      if (receiveRender.pop(s)) {
        // make a pretty picture with s
      } else
        receiveRenderWasEmpty++;
      this_thread::sleep_for(std::chrono::microseconds(16666));
    }
  });

  waitingToStart = false;
  getchar();
  done = true;
  simulate.join();
  broadcast.join();
  receive.join();
  render.join();

  cout << "simulateBroadcastWasFull:" << simulateBroadcastWasFull << endl;
  cout << "simulateBroadcastWasEmpty:" << simulateBroadcastWasEmpty << endl;
  cout << "receiveRenderWasFull:" << receiveRenderWasFull << endl;
  cout << "receiveRenderWasEmpty:" << receiveRenderWasEmpty << endl;
}
