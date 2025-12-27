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
void frame_tag(struct frame *f, unsigned value);
void legacy_copy(char *dst, const char *src, size_t len);

#ifdef __cplusplus
}
#endif
