#include "Cuttlebone/Receiver.hpp"
#ifndef _WINDOWS
#include <unistd.h> // sleep()
#else

#endif

using namespace cuttlebone;

unsigned char *buffer;

int main(int argc, char *argv[]) {
  unsigned packetSize = 1024;
  float timeOut = 0.8f;
  unsigned port = 8888;
  if (argc > 1)
    packetSize = atoi(argv[1]);
  if (argc > 2)
    timeOut = atof(argv[2]);
  if (argc > 3)
    port = atoi(argv[3]);

  printf("usage: %s packetSize timeOut port\n", argv[0]);
  printf("receiving %u bytes from on port %u every %f seconds\n", packetSize,
         port, timeOut);
  printf("#\n");

  Receiver receiver;
  receiver.init(port);

  std::vector<unsigned char> buffer;
  buffer.resize(packetSize);

  while (true)
    if (receiver.receive(buffer.data(), packetSize, timeOut))
      printf("%03u\n", buffer[0]);
}
