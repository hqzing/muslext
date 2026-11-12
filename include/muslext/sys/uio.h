/* Minimal vector-I/O extensions provided by muslext. */

#ifndef MUSLEXT_SYS_UIO_H
#define MUSLEXT_SYS_UIO_H 1

#include <sys/types.h>
#include <sys/uio.h>

#ifdef __cplusplus
extern "C" {
#endif

ssize_t __preadv64v2 (int fd, const struct iovec *iov, int iovcnt,
                      off_t offset, int flags);
ssize_t __pwritev64v2 (int fd, const struct iovec *iov, int iovcnt,
                       off_t offset, int flags);
ssize_t preadv64v2 (int fd, const struct iovec *iov, int iovcnt,
                    off_t offset, int flags);
ssize_t pwritev64v2 (int fd, const struct iovec *iov, int iovcnt,
                     off_t offset, int flags);

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_SYS_UIO_H */
