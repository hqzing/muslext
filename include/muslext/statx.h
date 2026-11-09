/* Minimal declaration of statx(2). */

#ifndef MUSLEXT_STATX_H
#define MUSLEXT_STATX_H 1

#ifdef __cplusplus
extern "C" {
#endif

struct statx;

int statx (int dirfd, const char *pathname, int flags, unsigned int mask,
           struct statx *statxbuf);

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_STATX_H */
