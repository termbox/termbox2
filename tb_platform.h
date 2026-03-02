#ifndef TB_PLATFORM_H
#define TB_PLATFORM_H

#include <stddef.h>
#include <sys/types.h>

/*
 * Platform interface for termbox2.
 *
 * Each platform backend allocates its own private struct (e.g. struct tb_posix,
 * struct tb_wasm) and returns it as an opaque void pointer. The core never
 * accesses platform-specific fields directly.
 */

/* Lifecycle */
void *tb_platform_open(const char *path, int *err);
void *tb_platform_open_fd(int fd, int *err);
void *tb_platform_open_rwfd(int rfd, int wfd, int *err);
void  tb_platform_shutdown(void *platform);

/* Terminal setup */
int tb_platform_enter_raw(void *platform);
int tb_platform_init(void *platform);

/* I/O */
int tb_platform_write(void *platform, const char *buf, size_t len);
int tb_platform_read(void *platform, char *buf, size_t len, ssize_t *nread);
int tb_platform_wait(void *platform, int timeout_ms, int *input, int *resize);

/* Queries */
int tb_platform_term_size(void *platform, int *w, int *h);
int tb_platform_get_fds(void *platform, int *ttyfd, int *resizefd);
int tb_platform_can_write(void *platform);
int tb_platform_drain_resize(void *platform);

#endif /* TB_PLATFORM_H */
