/* Miscellaneous compatibility functions. */

#define _GNU_SOURCE 1

#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <sched.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/syscall.h>

#include <muslext/muslext.h>

#ifndef STATX_TYPE
# include <linux/stat.h>
#endif

#include "internal.h"

void *
reallocarray (void *ptr, size_t nmemb, size_t size)
{
  size_t total;

  if (__builtin_mul_overflow (nmemb, size, &total))
    {
      errno = ENOMEM;
      return NULL;
    }
  return realloc (ptr, total);
}

char *
get_current_dir_name (void)
{
  static char *(*real) (void);

  if (real == NULL)
    real = (char *(*)(void)) muslext_resolve ("get_current_dir_name",
                                              (void *) get_current_dir_name);
  if (real != NULL)
    return real ();

  return getcwd (NULL, 0);
}

char *
canonicalize_file_name (const char *name)
{
  return realpath (name, NULL);
}

int
__sigsetjmp (sigjmp_buf env, int savemask)
{
  static int (*real) (sigjmp_buf, int);

  if (real == NULL)
    real = (int (*)(sigjmp_buf, int)) muslext_resolve ("__sigsetjmp",
                                                       (void *) __sigsetjmp);
  if (real != NULL)
    return real (env, savemask);

  return sigsetjmp (env, savemask);
}

int
pthread_setaffinity_np (pthread_t thread, size_t cpusetsize,
                        const cpu_set_t *cpuset)
{
  static int (*real) (pthread_t, size_t, const cpu_set_t *);

  if (real == NULL)
    real = (int (*)(pthread_t, size_t, const cpu_set_t *))
      muslext_resolve ("pthread_setaffinity_np",
                       (void *) pthread_setaffinity_np);
  if (real != NULL)
    return real (thread, cpusetsize, cpuset);

  (void) thread;
  (void) cpusetsize;
  (void) cpuset;
  return ENOSYS;
}

int
pthread_getaffinity_np (pthread_t thread, size_t cpusetsize, cpu_set_t *cpuset)
{
  static int (*real) (pthread_t, size_t, cpu_set_t *);

  if (real == NULL)
    real = (int (*)(pthread_t, size_t, cpu_set_t *))
      muslext_resolve ("pthread_getaffinity_np",
                       (void *) pthread_getaffinity_np);
  if (real != NULL)
    return real (thread, cpusetsize, cpuset);

  (void) thread;
  (void) cpusetsize;
  (void) cpuset;
  return ENOSYS;
}

int
__xpg_strerror_r (int errnum, char *buf, size_t buflen)
{
  static int (*real) (int, char *, size_t);

  if (real == NULL)
    real = (int (*)(int, char *, size_t)) muslext_resolve ("__xpg_strerror_r",
                                                           (void *) __xpg_strerror_r);
  if (real != NULL)
    return real (errnum, buf, buflen);

  {
    const char *message = strerror (errnum);
    size_t length = strlen (message) + 1;

    if (length > buflen)
      {
        if (buflen != 0)
          {
            memcpy (buf, message, buflen - 1);
            buf[buflen - 1] = '\0';
          }
        return ERANGE;
      }
    memcpy (buf, message, length);
    return 0;
  }
}

int
statx (int dirfd, const char *pathname, int flags, unsigned int mask,
       struct statx *statxbuf)
{
  static int (*real) (int, const char *, int, unsigned int, struct statx *);

  if (real == NULL)
    real = (int (*)(int, const char *, int, unsigned int, struct statx *))
      muslext_resolve ("statx", (void *) statx);
  if (real != NULL)
    return real (dirfd, pathname, flags, mask, statxbuf);

#ifdef SYS_statx
  return (int) syscall (SYS_statx, dirfd, pathname, flags, mask, statxbuf);
#else
  (void) dirfd;
  (void) pathname;
  (void) flags;
  (void) mask;
  (void) statxbuf;
  errno = ENOSYS;
  return -1;
#endif
}

int
pthread_tryjoin_np (pthread_t thread, void **retval)
{
  int error = pthread_kill (thread, 0);

  if (error == ESRCH)
    return pthread_join (thread, retval);
  return EBUSY;
}

int
pthread_timedjoin_np (pthread_t thread, void **retval,
                      const struct timespec *abstime)
{
  struct timespec now;

  for (;;)
    {
      if (pthread_kill (thread, 0) == ESRCH)
        return pthread_join (thread, retval);

      clock_gettime (CLOCK_REALTIME, &now);
      if (abstime != NULL
          && (now.tv_sec > abstime->tv_sec
              || (now.tv_sec == abstime->tv_sec
                  && now.tv_nsec >= abstime->tv_nsec)))
        return ETIMEDOUT;

      {
        struct timespec delay = { 0, 1000000 };
        nanosleep (&delay, NULL);
      }
    }
}

int
close_range (unsigned int first, unsigned int last, int flags)
{
#ifdef SYS_close_range
  return (int) syscall (SYS_close_range, first, last, flags);
#else
  (void) first;
  (void) last;
  (void) flags;
  errno = ENOSYS;
  return -1;
#endif
}

char *
crypt_r (const char *key, const char *salt, struct crypt_data *data)
{
  static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
  static _Thread_local char result_buffer[256];
  char *result;
  size_t length;

  if (data == NULL)
    {
      errno = EINVAL;
      return NULL;
    }

  pthread_mutex_lock (&lock);
  result = crypt (key, salt);
  if (result == NULL)
    {
      pthread_mutex_unlock (&lock);
      return NULL;
    }

  length = strlen (result);
  if (length >= sizeof (result_buffer))
    {
      pthread_mutex_unlock (&lock);
      errno = ERANGE;
      return NULL;
    }

  memcpy (result_buffer, result, length + 1);
  data->initialized = 1;
  pthread_mutex_unlock (&lock);

  return result_buffer;
}
