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
int frame_fill(struct frame *f, const char *src, size_t n) {
  if (n < sizeof(f->data)) {
    memcpy(f->data, src, n);
    f->used = n;
    return 0;
  }
  return -1;
}
