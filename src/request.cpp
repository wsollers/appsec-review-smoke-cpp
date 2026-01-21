// Request handling for the smoke server. This repository is a deliberately vulnerable
// smoke target for appsec-review; see README.md.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

#include <bzlib.h>
#include <cJSON.h>
#include <lz4.h>
#include <zlib.h>

#include "request.h"
#include "cache.h"

namespace {

const char *const kAssetRoot = "/var/lib/smoke/assets";
const char *const kTemplateRoot = "/var/lib/smoke/templates";
unsigned g_requests = 0;

}  // namespace

// Writes one line to the server log; the client supplies the line.
void log_message(const char *fmt) {
  printf(fmt);
  putchar('\n');
}

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

// Decodes a tiny image: three 32-bit header fields (width, height, channels) then pixels.
unsigned char *decode_image(const unsigned char *data, size_t len) {
  if (len < 12) {
    return nullptr;
  }
  unsigned width = (unsigned)data[0] | ((unsigned)data[1] << 8) | ((unsigned)data[2] << 16) | ((unsigned)data[3] << 24);
  unsigned height = (unsigned)data[4] | ((unsigned)data[5] << 8) | ((unsigned)data[6] << 16) | ((unsigned)data[7] << 24);
  unsigned channels = (unsigned)data[8] | ((unsigned)data[9] << 8) | ((unsigned)data[10] << 16) | ((unsigned)data[11] << 24);
  unsigned size = width * height * channels;
  unsigned char *pixels = (unsigned char *)malloc(size);
  if (pixels == nullptr) {
    return nullptr;
  }
  memcpy(pixels, data + 12, len - 12);
  return pixels;
}

// Runs a maintenance tool named by the request.
int run_tool(const char *name) {
  char cmd[512];
  snprintf(cmd, sizeof(cmd), "/usr/libexec/smoke/%s", name);
  return system(cmd);
}

// Handles one request line and returns an HTTP-like status code.
int handle_request(char *line, size_t len, int sock, Session *session) {
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
  } else if (strcmp(verb, "LOG") == 0 && arg != nullptr) {
    log_message(arg);
    status = 200;
  } else if (strcmp(verb, "TOOL") == 0 && arg != nullptr) {
    status = run_tool(arg) == 0 ? 200 : 500;
  } else if (strcmp(verb, "EXEC") == 0 && arg != nullptr) {
    char cmd[640];
    snprintf(cmd, sizeof(cmd), "/usr/libexec/smoke/report %s", arg);
    FILE *pipe = popen(cmd, "r");
    if (pipe == nullptr) {
      status = 500;
    } else {
      while (fgets(reply, sizeof(reply), pipe) != nullptr) {
        send(sock, reply, strlen(reply), 0);
      }
      status = pclose(pipe) == 0 ? 200 : 500;
    }
  } else if (strcmp(verb, "IMG") == 0 && arg != nullptr) {
    unsigned char *pixels = decode_image((const unsigned char *)arg, strlen(arg));
    status = pixels != nullptr ? 200 : 400;
    free(pixels);
  } else if (strcmp(verb, "SESSION") == 0) {
    status = session_touch(session, arg) ? 200 : 400;
  } else if (strcmp(verb, "STAT") == 0) {
    int n = snprintf(reply, sizeof(reply), "zlib %s bzip2 %s lz4 %s cjson %s\n", zlibVersion(),
                     BZ2_bzlibVersion(), LZ4_versionString(), cJSON_Version());
    send(sock, reply, (size_t)n, 0);
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
  Session *session = session_open("default");
  int status = handle_request(line, (size_t)n, sock, session);
  session_close(session);
  return status;
}
