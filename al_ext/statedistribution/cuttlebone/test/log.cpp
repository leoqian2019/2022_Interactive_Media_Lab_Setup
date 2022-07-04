#include <thread>

#ifndef _WINDOWS
#include <unistd.h>
#else
#endif

// using namespace std;

//#define LOG_FILE ("/tmp/file.txt") // LOG to a file instead of standard out
//#define DONT_COMPILE_LOG // like LOG never existed
#include "Cuttlebone/Log.hpp"

using namespace cuttlebone;

int main(int argc, char *argv[]) {
  LOG("main|started");

  std::thread audio([&]() {
    LOG("audio|started");
    for (int i = 0; i < 345; i++) {

      std::this_thread::sleep_for(std::chrono::microseconds(2902));
      LOG("audio|%d", i);
    }
    LOG("audio|ended");
  });

  std::thread graphics([&]() {
    LOG("graphics|started");
    for (int i = 0; i < 60; i++) {
      std::this_thread::sleep_for(std::chrono::microseconds(16667));
      LOG("graphics|%d", i);
    }
    LOG("graphics|ended");
  });

  std::thread network([&]() {
    LOG("network|started");
    for (int i = 0; i < 10; i++) {
      std::this_thread::sleep_for(std::chrono::microseconds(100000));
      LOG("network|%d", i);
    }
    LOG("network|ended");
  });

  LOG("main|waiting for <enter>...");
  getchar();
  audio.join();
  graphics.join();
  network.join();
  LOG("main|ended");
}
