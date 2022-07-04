#include <sys/types.h>

#ifndef _WINDOWS
#include <netinet/in.h> // IPPROTO_UDP
#include <sys/socket.h>
#else
#endif
#include <cstdio>
#include <cstdlib>
#include <string.h> // memset

#define LOG_FILE ("send.txt")
#include "Cuttlebone/Broadcaster.hpp"
#include "Cuttlebone/Log.hpp"

using namespace cuttlebone;

#define N (2048)

int main() {
  LOG("main()");
  Broadcaster broadcaster;
  broadcaster.init(N * 4, "192.168.10.255", 10101);
  int *buffer = new int[N];
  buffer[0] = 0;
  while (true) {
    LOG("send %d", buffer[0]);
    broadcaster.send((unsigned char *)buffer);
    buffer[0]++;
  }
}
