// Request handling for the smoke server. This repository is a deliberately vulnerable
// smoke target for appsec-review; see README.md.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

#include "request.h"

namespace {

unsigned g_requests = 0;

}  // namespace

// Handles one request line and returns an HTTP-like status code.
int handle_request(char *line, size_t len, int sock) {
  int status = 400;
  (void)len;
  char *verb = line;
  char *arg = strchr(verb, ' ');
  if (arg != nullptr) {
    *arg++ = '\0';
  }
  if (strcmp(verb, "PING") == 0) {
    send(sock, "PONG\n", 5, 0);
    status = 200;
  }
  if (status >= 200 && status < 300) {
    ++g_requests;
  }
  return status;
}

// Reads one request from the socket and handles it.
int serve_once(int sock) {
  char line[1024];
  ssize_t n = recv(sock, line, sizeof(line) - 1, 0);
  if (n <= 0) {
    return -1;
  }
  line[n] = '\0';
  return handle_request(line, (size_t)n, sock);
}
