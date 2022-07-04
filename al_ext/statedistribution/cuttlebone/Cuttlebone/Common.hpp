#ifndef COMMON_HPP
#define COMMON_HPP

#ifdef _WINDOWS

#include <WS2tcpip.h>
#include <WinSock.h>

#include <stdio.h>

struct WsInit {
  WsInit() {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (0 != iResult) {
      printf("WSAStartup failed: %d\n", iResult);
    }
  }
  ~WsInit() { WSACleanup(); }
  static WsInit &get() {
    static WsInit v;
    return v;
  }
};
#else
#define SOCKET int
#endif

#define SOCKET_ERROR (-1)

#endif // COMMON_HPP
