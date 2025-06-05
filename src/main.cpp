// Entry point of the smoke server: the parent process hands over a connected socket.
#include <cstdlib>
#include <cstring>

#include "request.h"

int main(int argc, char **argv) {
  int sock = argc > 2 ? atoi(argv[2]) : 0;
  return serve_once(sock) >= 0 ? 0 : 1;
}
