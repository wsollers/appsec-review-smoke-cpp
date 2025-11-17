// Per-connection session state for the smoke server.
#pragma once

struct Session {
  char name[32];
  unsigned hits;
  char *scratch;
};

Session *session_open(const char *name);
bool session_touch(Session *session, const char *arg);
void session_close(Session *session);
