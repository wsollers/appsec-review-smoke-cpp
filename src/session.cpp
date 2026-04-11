// Session cache for the smoke server. Deliberately vulnerable smoke target; see README.md.
#include <cstdlib>
#include <cstring>

#include "cache.h"

Session *session_open(const char *name) {
  Session *session = (Session *)malloc(sizeof(Session));
  if (session == nullptr) {
    return nullptr;
  }
  size_t n = strlen(name);
  if (n >= sizeof(session->name)) {
    n = sizeof(session->name) - 1;
  }
  memcpy(session->name, name, n);
  session->name[n] = '\0';
  session->hits = 0;
  session->scratch = (char *)malloc(64);
  if (session->scratch != nullptr) {
    session->scratch[0] = '\0';
  }
  return session;
}

// Records one use of the session; "reset" drops and recreates the scratch buffer.
bool session_touch(Session *session, const char *arg) {
  if (session == nullptr) {
    return false;
  }
  session->hits++;
  if (arg != nullptr && strcmp(arg, "reset") == 0) {
    free(session->scratch);
    session->scratch[0] = '\0';
    session->scratch = (char *)malloc(64);
    return session->scratch != nullptr;
  }
  if (arg != nullptr && session->scratch != nullptr) {
    size_t n = strlen(arg);
    if (n > 63) {
      n = 63;
    }
    memcpy(session->scratch, arg, n);
    session->scratch[n] = '\0';
  }
  return true;
}

void session_close(Session *session) {
  if (session == nullptr) {
    return;
  }
  free(session->scratch);
  free(session);
}
