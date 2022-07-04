#ifndef __BROADCASTER__
#define __BROADCASTER__

#ifndef _WINDOWS
#include <arpa/inet.h> // struct sockaddr_in
#endif
#include "Cuttlebone/Common.hpp"
#include "Cuttlebone/Log.hpp"
#include <cassert>
#include <cstdio>  // linux
#include <cstdlib> // linux
#include <cstring> // linux
#include <string>
#include <thread>

#ifndef _WINDOWS
#include <unistd.h> // sleep()
#else
#include <WS2tcpip.h>
#include <WinSock.h>
//#include <WinSock2.h>
#endif

namespace cuttlebone {

struct Broadcaster {
  struct sockaddr_in address;
  int fileDescriptor;
  unsigned packetSize;

  void init(unsigned packetSize, const char *ip, int port,
            bool useLargeWindow = true) {
#ifdef _WINDOWS
    WsInit::get();
#endif

    this->packetSize = packetSize;

    fileDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    assert(fileDescriptor != SOCKET_ERROR);

    /* XXX i'd like to do this, but it seems that OSX does not provide a way to
       ask what the MTU is.

        int optval, optlen;
        optlen = sizeof(optval);
        if (getsockopt(fileDescriptor, SOL_SOCKET, SO_MAX_MSG_SIZE,
       (char*)&optval, &optlen) == 0) if (optlen == sizeof(int)) { LOG("MTU:
       %d", optval);
          }
    */
#ifndef _WINDOWS
    int broadcast = 1;
    setsockopt(fileDescriptor, SOL_SOCKET, SO_BROADCAST, &broadcast,
               sizeof(broadcast));
#else
    char broadcast = 1;
    setsockopt(fileDescriptor, SOL_SOCKET, SO_BROADCAST, &broadcast,
               sizeof(broadcast));
#endif

    // XXX check on broadcast with getsockopt

    if (useLargeWindow) {
      int window = 16777216;
#ifndef _WINDOWS
      if (setsockopt(fileDescriptor, SOL_SOCKET, SO_SNDBUF, &window,
                     sizeof(int)) == -1) {
        fprintf(stderr, "Error setting socket opts: %s\n", strerror(errno));
      }
#else
      if (setsockopt(fileDescriptor, SOL_SOCKET, SO_SNDBUF,
                     (const char *)&window, sizeof(int)) == -1) {
        fprintf(stderr, "Error setting socket opts: %s\n", strerror(errno));
      }
#endif
      printf("%d byte send buffer (aka \"window\")\n", window);
    }

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip);

    LOG("broadcasting to %s on port %d", ip, port);
  }

  void send(unsigned char *data) {

    if (sendto(fileDescriptor, (const char *)data, packetSize, 0,
               (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) {
      LOG("failed to SEND packet. make sure MTU is larger than %u", packetSize);
    }
  }
};

} // namespace cuttlebone

#endif
