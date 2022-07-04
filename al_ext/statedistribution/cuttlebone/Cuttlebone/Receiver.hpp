#ifndef __SELECTOR__
#define __SELECTOR__

#ifndef _WINDOWS
#include <arpa/inet.h>  // struct sockaddr_in
#include <sys/select.h> // select
#else
#include <WS2tcpip.h>
#include <WinSock.h>

#include <Mstcpip.h>
#endif

#include "Cuttlebone/Common.hpp"
#include "Cuttlebone/Log.hpp"
#include <cstdio>  // linux
#include <cstdlib> // linux
#include <cstring> // linux
#include <string>

namespace cuttlebone {
#ifdef _WINDOWS
DWORD secToTimeout(float t) {
  return t >= 0. ? DWORD(t * 1000. + 0.5) : 4294967295; // msec
}
#endif

struct Receiver {
  SOCKET fileDescriptor;

  void init(unsigned port, bool useLargeWindow = true) {
#ifdef _WINDOWS
    WsInit::get();
#endif
    if ((fileDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) ==
        SOCKET_ERROR) {
      perror("socket");
      exit(-1);
    }

    if (useLargeWindow) {
      // magic
      //
      int window = 16777216;
#ifndef _WINDOWS
      if (setsockopt(fileDescriptor, SOL_SOCKET, SO_RCVBUF, &window,
                     sizeof(int)) == -1) {
        fprintf(stderr, "Error setting socket opts: %s\n", strerror(errno));
      }
#else
      if (setsockopt(fileDescriptor, SOL_SOCKET, SO_RCVBUF,
                     (const char *)&window, sizeof(int)) == -1) {
        fprintf(stderr, "Error setting socket opts: %s\n", strerror(errno));
      }
      float v = 0.2;
      auto to = secToTimeout(v);
      for (auto opt : {SO_SNDTIMEO, SO_RCVTIMEO}) {
        if (v < 0) { // necessary?
#ifdef _WINDOWS
          decltype(WSABUF::len) nb = 0;
          ::ioctlsocket(fileDescriptor, FIONBIO, &nb);
#else
//                  int nb = 0;
//                  ::ioctl(mSocketHandle, FIONBIO, &nb);
#endif
        } else if (SOCKET_ERROR == ::setsockopt(fileDescriptor, SOL_SOCKET, opt,
                                                (char *)&to, sizeof(to))) {
          //          AL_WARN("unable to set timeout on socket at %s:%i: %s",
          //                  mAddress.c_str(), mPort, errorString());
        }
      }

#endif
      printf("%d byte receive buffer (aka \"window\")\n", window);
    }

    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
#ifdef _WINDOWS
    address.sin_addr.s_addr = htonl(INADDR_ANY);
#else
    address.sin_addr.s_addr = INADDR_ANY;
#endif

    // we need the :: to disambiguate between std::bind (c++11) socket.h bind
    //
    if (::bind(fileDescriptor, (sockaddr *)&address, sizeof(sockaddr)) == -1) {
      perror("bind");
      exit(-1);
    }

#ifdef _WINDOWS
    int optval = 1;
    int in;
//    WSAIoctl(fileDescriptor, SIO_RCVALL, &optval, sizeof(optval), 0, 0,
//             (LPDWORD)&in, 0, 0);

//    if (setsockopt(fileDescriptor, IPPROTO_IP, IP_HDRINCL, (char *)&optval,
//                   sizeof(optval)) == -1) {
//      printf("Error in setsockopt(): %d\n", WSAGetLastError());
//      //        getch();
//      closesocket(fileDescriptor);
//    }
#endif

    LOG("Receiver listening on port %d", port);
  }

  bool receive(void *buffer, unsigned packetSize, float timeOut) {
#ifndef _WINDOWS
    fd_set fileDescriptorSet;
    FD_ZERO(&fileDescriptorSet);
    FD_SET(fileDescriptor, &fileDescriptorSet);

    int seconds = (int)timeOut;
    int microseconds = (timeOut - (int)timeOut) * 1000000;
    if (microseconds > 999999)
      microseconds = 999999;

    struct timeval tv;
    tv.tv_sec = seconds;
    tv.tv_usec = microseconds;

    int rv = select(fileDescriptor + 1, &fileDescriptorSet, 0, 0, &tv);
    if (rv == -1) {
      LOG("select error %d", errno);
      return false;
    } else if (rv == 0) {
      return false;
    }
    int bytesReceived =
        recvfrom(fileDescriptor, (char *)buffer, packetSize, 0, 0, 0);
#else
    int bytesReceived =
        recvfrom(fileDescriptor, (char *)buffer, packetSize, 0, 0, 0);
//    int bytesReceived = recv(fileDescriptor, (char *)buffer, packetSize, 0);
#endif
    if (bytesReceived == SOCKET_ERROR) {
      perror("recvfrom");
      return false;
    } else if (bytesReceived != (int)packetSize) {
      printf("Received less than expected. Likely MTU mismatch.\n");
      return false;
    } else {
      return true;
    }
  }
};

} // namespace cuttlebone

#endif
