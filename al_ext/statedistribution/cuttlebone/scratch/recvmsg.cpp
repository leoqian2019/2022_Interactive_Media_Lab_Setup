#include <sys/types.h>

#ifndef _WINDOWS
#include <netinet/in.h> // IPPROTO_UDP
#include <sys/socket.h>
#else
#include <WS2tcpip.h>
#include <WinSock.h>
#include <WinSock2.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <string.h> // memset

#define LOG_FILE ("recv.txt")
#include "Cuttlebone/Log.hpp"

using namespace cuttlebone;

#define N (2048)

int main() {
  int s = 0;

  if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) <= 0) {
    fprintf(stderr, "socket()\n");
    exit(1);
  }

  // magic
  //
  int window = 16777216;
#ifndef _WINDOWS
  if (setsockopt(s, SOL_SOCKET, SO_RCVBUF, &window, sizeof(int)) == -1) {
    fprintf(stderr, "Error setting socket opts: %s\n", strerror(errno));
  }
#else
  if (setsockopt(s, SOL_SOCKET, SO_RCVBUF, (const char *)&window,
                 sizeof(int)) == -1) {
    fprintf(stderr, "Error setting socket opts: %s\n", strerror(errno));
  }
#endif
  printf("%d byte receive buffer (aka \"window\")\n", window);

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons(atoi("10101"));
  server.sin_addr.s_addr = INADDR_ANY;

  if (::bind(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
    fprintf(stderr, "bind()\n");
    exit(1);
  }

  int buffer[N];
  memset(buffer, 0, sizeof(buffer));

#ifndef _WINDOWS
  struct iovec iov;
  iov.iov_base = buffer;
  iov.iov_len = N * 4;

  struct msghdr header;
  memset(&header, 0, sizeof(header));
  header.msg_namelen = sizeof(struct sockaddr);
  header.msg_iov = &iov;
  header.msg_iovlen = 1;
#endif

  LOG("started...");
#ifndef _WINDOWS
  size_t bytes = 0;
#else
  uint32_t bytes = 0;
#endif
  int last = -1;
  while (true) {
#ifndef _WINDOWS
    if ((bytes = recvmsg(s, &header, 0)) < 0) {
#else
    if (false) {
//    if (WSARecvMsg(s, &header, &bytes) != 0) {
//      int WSARecvMsg(
//          _In_    SOCKET                             s,
//          _Inout_ LPWSAMSG                           lpMsg,
//          _Out_   LPDWORD                            lpdwNumberOfBytesRecvd,
//          _In_    LPWSAOVERLAPPED                    lpOverlapped,
//          _In_    LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
//          );
#endif
      // if ((bytes = recvmsg(s, &header, MSG_WAITALL)) < 0) {
      fprintf(stderr, "recvmsg()\n");
      exit(1);
    } else {
      if (bytes != (N * 4)) {
        LOG("truncation (got %d bytes)", bytes);
      } else {
        LOG("got message %d (missed %d)", buffer[0], buffer[0] - last - 1);
        last = buffer[0];
      }
    }
  }
}
