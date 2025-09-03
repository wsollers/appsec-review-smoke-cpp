// Small C helpers shared by the smoke server.
#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct frame {
  char data[64];
  char tag[4];
  size_t used;
};

void frame_reset(struct frame *f);
int frame_fill(struct frame *f, const char *src, size_t n);

#ifdef __cplusplus
}
#endif
