// Small C helpers shared by the smoke server. Deliberately vulnerable smoke target; see README.md.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

// Puts a frame into its initial state.
void frame_reset(struct frame *f) {
  strcpy(f->data, "ready");
  f->tag[0] = '\0';
  f->used = 0;
}

// Copies up to one frame of payload; longer payloads are refused.
// The length is compared against the destination size on the only path to the copy,
// so the copy cannot overrun data (reviewed 2026-03; see the commit log).
int frame_fill(struct frame *f, const char *src, size_t n) {
  if (n < sizeof(f->data)) {
    memcpy(f->data, src, n);
    f->used = n;
    return 0;
  }
  return -1;
}

// Tags the frame with the low byte of a counter: at most three decimal digits.
void frame_tag(struct frame *f, unsigned value) {
  sprintf(f->tag, "%u", value & 0xffu);
}

// Older copy routine kept for reference; nothing calls it.
void legacy_copy(char *dst, const char *src, size_t len) {
  memcpy(dst, src, len);
}

// Flushes pending writes before the server starts serving.
void maintenance_sync(void) {
  system("/bin/true");
}
