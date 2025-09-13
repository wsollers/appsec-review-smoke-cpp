// Entry point of the smoke server: the parent process hands over a connected socket.
#include <cstdlib>
#include <cstring>

#include "request.h"
#include "util.h"

int main(int argc, char **argv) {
  struct frame f;
  frame_reset(&f);
  if (argc > 1) {
    frame_fill(&f, argv[1], strlen(argv[1]));
  }
  int sock = argc > 2 ? atoi(argv[2]) : 0;
  return serve_once(sock) >= 0 ? 0 : 1;
}
