/* Linux syscall wrappers that fail safely on kernels without support. */

#define _GNU_SOURCE 1

#include <errno.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/uio.h>
#include <unistd.h>

#include <muslext/muslext.h>

static int
unavailable (void)
{
  errno = ENOSYS;
  return -1;
}

int
fsopen (const char *fs_name, unsigned int flags)
{
  (void) fs_name;
  (void) flags;
  return unavailable ();
}

int
fspick (int dirfd, const char *pathname, unsigned int flags)
{
  (void) dirfd;
  (void) pathname;
  (void) flags;
  return unavailable ();
}

int
fsconfig (int fd, unsigned int cmd, const char *key, const void *value,
          int aux)
{
  (void) fd;
  (void) cmd;
  (void) key;
  (void) value;
  (void) aux;
  return unavailable ();
}

int
fsmount (int fd, unsigned int flags, unsigned int attr_flags)
{
  (void) fd;
  (void) flags;
  (void) attr_flags;
  return unavailable ();
}

int
move_mount (int from_dirfd, const char *from_pathname, int to_dirfd,
            const char *to_pathname, unsigned int flags)
{
  (void) from_dirfd;
  (void) from_pathname;
  (void) to_dirfd;
  (void) to_pathname;
  (void) flags;
  return unavailable ();
}

int
open_tree (int dirfd, const char *pathname, unsigned int flags)
{
  (void) dirfd;
  (void) pathname;
  (void) flags;
  return unavailable ();
}

int
mount_setattr (int dirfd, const char *pathname, unsigned int flags,
               void *attr, size_t size)
{
  (void) dirfd;
  (void) pathname;
  (void) flags;
  (void) attr;
  (void) size;
  return unavailable ();
}

int
pidfd_open (pid_t pid, unsigned int flags)
{
#ifdef SYS_pidfd_open
  return (int) syscall (SYS_pidfd_open, pid, flags);
#else
  (void) pid;
  (void) flags;
  return unavailable ();
#endif
}

int
pidfd_getfd (int pidfd, int targetfd, unsigned int flags)
{
#ifdef SYS_pidfd_getfd
  return (int) syscall (SYS_pidfd_getfd, pidfd, targetfd, flags);
#else
  (void) pidfd;
  (void) targetfd;
  (void) flags;
  return unavailable ();
#endif
}

int
getcpu (unsigned int *cpu, unsigned int *node)
{
#ifdef SYS_getcpu
  return (int) syscall (SYS_getcpu, cpu, node, NULL);
#else
  (void) cpu;
  (void) node;
  return unavailable ();
#endif
}

ssize_t
__preadv64v2 (int fd, const struct iovec *iov, int iovcnt, off_t offset,
              int flags)
{
#ifdef SYS_preadv2
  return (ssize_t) syscall (SYS_preadv2, fd, iov, iovcnt, offset, flags);
#else
  (void) fd;
  (void) iov;
  (void) iovcnt;
  (void) offset;
  (void) flags;
  return unavailable ();
#endif
}

ssize_t
__pwritev64v2 (int fd, const struct iovec *iov, int iovcnt, off_t offset,
               int flags)
{
#ifdef SYS_pwritev2
  return (ssize_t) syscall (SYS_pwritev2, fd, iov, iovcnt, offset, flags);
#else
  (void) fd;
  (void) iov;
  (void) iovcnt;
  (void) offset;
  (void) flags;
  return unavailable ();
#endif
}

ssize_t preadv64v2 (int, const struct iovec *, int, off_t, int)
  __attribute__ ((weak, alias ("__preadv64v2")));
ssize_t pwritev64v2 (int, const struct iovec *, int, off_t, int)
  __attribute__ ((weak, alias ("__pwritev64v2")));

int
ntp_adjtime (void *buf)
{
  (void) buf;
  return unavailable ();
}
