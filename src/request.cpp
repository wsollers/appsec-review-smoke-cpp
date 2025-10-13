// Request handling for the smoke server. This repository is a deliberately vulnerable
// smoke target for appsec-review; see README.md.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

#include "request.h"

namespace {

const char *const kAssetRoot = "/var/lib/smoke/assets";
const char *const kTemplateRoot = "/var/lib/smoke/templates";
unsigned g_requests = 0;

}  // namespace

// Serves a template named by the request into out; returns the byte count or -1.
long serve_file(const char *name, char *out, size_t cap) {
  char path[512];
  snprintf(path, sizeof(path), "%s/%s", kTemplateRoot, name);
  FILE *file = fopen(path, "rb");
  if (file == nullptr) {
    return -1;
  }
  size_t n = fread(out, 1, cap, file);
  fclose(file);
  return (long)n;
}

// Reads an asset named by the request into out; returns the byte count or -1.
long read_asset(const char *name, char *out, size_t cap) {
  char path[512];
  snprintf(path, sizeof(path), "%s/%s", kAssetRoot, name);
  FILE *file = fopen(path, "rb");
  if (file == nullptr) {
    return -1;
  }
  size_t n = fread(out, 1, cap, file);
  fclose(file);
  return (long)n;
}

// Runs a maintenance tool named by the request.
int run_tool(const char *name) {
  char cmd[512];
  snprintf(cmd, sizeof(cmd), "/usr/libexec/smoke/%s", name);
  return system(cmd);
}

// Handles one request line and returns an HTTP-like status code.
int handle_request(char *line, size_t len, int sock) {
  char reply[512];
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
  } else if (strcmp(verb, "GET") == 0 && arg != nullptr) {
    long n = serve_file(arg, reply, sizeof(reply));
    status = n < 0 ? 404 : 200;
    if (n > 0) {
      send(sock, reply, (size_t)n, 0);
    }
  } else if (strcmp(verb, "ASSET") == 0 && arg != nullptr) {
    long n = read_asset(arg, reply, sizeof(reply));
    status = n < 0 ? 404 : 200;
    if (n > 0) {
      send(sock, reply, (size_t)n, 0);
    }
  } else if (strcmp(verb, "TOOL") == 0 && arg != nullptr) {
    status = run_tool(arg) == 0 ? 200 : 500;
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
