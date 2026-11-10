/* muslext - compatibility extensions for musl libc.
 *
 * This header is the single entry point for consumers.  It includes the
 * relevant system headers and adds the declarations/macros that those
 * headers omit.  It never replaces a system header.
 */

#ifndef MUSLEXT_H
#define MUSLEXT_H 1

#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif

#include <stddef.h>
#include <wchar.h>
#include <locale.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sched.h>
#include <pthread.h>
#include <setjmp.h>
#include <signal.h>
#include <utmpx.h>
#include <shadow.h>
#include <crypt.h>
#include <dirent.h>
#include <muslext/dirent.h>
#include <sys/uio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MUSLEXT_VERSION_MAJOR 1
#define MUSLEXT_VERSION_MINOR 1
#define MUSLEXT_VERSION_PATCH 0
#define MUSLEXT_ABI_VERSION 1

#define MUSLEXT_VERSION_STRINGIFY_(x) #x
#define MUSLEXT_VERSION_STRINGIFY(x) MUSLEXT_VERSION_STRINGIFY_(x)
#define MUSLEXT_VERSION \
  MUSLEXT_VERSION_STRINGIFY (MUSLEXT_VERSION_MAJOR) "." \
  MUSLEXT_VERSION_STRINGIFY (MUSLEXT_VERSION_MINOR) "." \
  MUSLEXT_VERSION_STRINGIFY (MUSLEXT_VERSION_PATCH)

/* ------------------------------------------------------------------ */
/* GNU constants missing from musl headers.                           */

#ifndef FNM_EXTMATCH
# define FNM_EXTMATCH (1 << 5)
#endif

#ifndef GLOB_BRACE
# define GLOB_BRACE (1 << 10)
#endif
#ifndef GLOB_NOMAGIC
# define GLOB_NOMAGIC (1 << 11)
#endif
#ifndef GLOB_TILDE
# define GLOB_TILDE (1 << 12)
#endif
#ifndef GLOB_ONLYDIR
# define GLOB_ONLYDIR (1 << 13)
#endif
#ifndef GLOB_TILDE_CHECK
# define GLOB_TILDE_CHECK (1 << 14)
#endif

#ifndef ACCESSPERMS
# define ACCESSPERMS (S_IRWXU | S_IRWXG | S_IRWXO)
#endif
#ifndef ALLPERMS
# define ALLPERMS (S_ISUID | S_ISGID | S_ISVTX | ACCESSPERMS)
#endif
#ifndef DEFFILEMODE
# define DEFFILEMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)
#endif

/* ------------------------------------------------------------------ */
/* A class: symbols present in the runtime libc but absent from the    */
/* build environment.  muslext exports them and delegates at runtime.  */

#include <muslext/utmpx.h>
#include <muslext/qsort_r.h>
#include <muslext/shadow.h>
#include <muslext/getpass.h>
#include <muslext/program.h>

void *reallocarray (void *ptr, size_t nmemb, size_t size);
char *get_current_dir_name (void);
int __sigsetjmp (sigjmp_buf env, int savemask);

#ifdef CPU_SETSIZE
int pthread_setaffinity_np (pthread_t thread, size_t cpusetsize,
                            const cpu_set_t *cpuset);
int pthread_getaffinity_np (pthread_t thread, size_t cpusetsize,
                            cpu_set_t *cpuset);
#endif

int __xpg_strerror_r (int errnum, char *buf, size_t buflen);

/* B class: interfaces absent from both the stub and the runtime libc. */

char *canonicalize_file_name (const char *name);
char *crypt_r (const char *key, const char *salt, struct crypt_data *data);
char *crypt (const char *key, const char *salt);

#include <muslext/statx.h>

/* gnu_misc */
long long strtoq (const char *nptr, char **endptr, int base);
unsigned long long strtouq (const char *nptr, char **endptr, int base);
void *rawmemchr (const void *s, int c);
int strverscmp (const char *left, const char *right);
size_t wcslcpy (wchar_t *dest, const wchar_t *src, size_t size);
size_t wcslcat (wchar_t *dest, const wchar_t *src, size_t size);
long wcstol_l (const wchar_t *nptr, wchar_t **endptr, int base, locale_t locale);
unsigned long wcstoul_l (const wchar_t *nptr, wchar_t **endptr, int base,
                         locale_t locale);
double gamma (double x);
double roundeven (double x);
float roundevenf (float x);
double _Complex clog10 (double _Complex z);
char *qecvt (long double value, int ndigit, int *decpt, int *sign);
char *qfcvt (long double value, int ndigit, int *decpt, int *sign);
char *qgcvt (long double value, int ndigit, char *buffer);

/* path */
char *getwd (char *buf);
int getpt (void);
void closefrom (int lowfd);
int scandirat (int dirfd, const char *dirp, struct dirent ***namelist,
               int (*filter) (const struct dirent *),
               int (*compar) (const struct dirent **, const struct dirent **));

/* signals and errno names */
int sigblock (int mask);
int sigsetmask (int mask);
const char *sigabbrev_np (int signal);
const char *sigdescr_np (int signal);
const char *strerrorname_np (int error);
const char *strerrordesc_np (int error);

/* device numbers */
unsigned int gnu_dev_major (dev_t dev);
unsigned int gnu_dev_minor (dev_t dev);
dev_t gnu_dev_makedev (unsigned int major, unsigned int minor);

/* base64 and glob */
int b64_ntop (const unsigned char *src, size_t srclen, char *target,
              size_t targsize);
int b64_pton (const char *src, unsigned char *target, size_t targsize);
int glob_pattern_p (const char *pattern, int quote);

/* Linux mount/pidfd/vector syscalls */
int fsopen (const char *fs_name, unsigned int flags);
int fspick (int dirfd, const char *pathname, unsigned int flags);
int fsconfig (int fd, unsigned int cmd, const char *key, const void *value,
              int aux);
int fsmount (int fd, unsigned int flags, unsigned int attr_flags);
int move_mount (int from_dirfd, const char *from_pathname, int to_dirfd,
                const char *to_pathname, unsigned int flags);
int open_tree (int dirfd, const char *pathname, unsigned int flags);
int mount_setattr (int dirfd, const char *pathname, unsigned int flags,
                   void *attr, size_t size);
int pidfd_open (pid_t pid, unsigned int flags);
int pidfd_getfd (int pidfd, int targetfd, unsigned int flags);
int getcpu (unsigned int *cpu, unsigned int *node);
int ntp_adjtime (void *buf);
ssize_t __preadv64v2 (int fd, const struct iovec *iov, int iovcnt,
                      off_t offset, int flags);
ssize_t __pwritev64v2 (int fd, const struct iovec *iov, int iovcnt,
                       off_t offset, int flags);
ssize_t preadv64v2 (int fd, const struct iovec *iov, int iovcnt,
                    off_t offset, int flags);
ssize_t pwritev64v2 (int fd, const struct iovec *iov, int iovcnt,
                     off_t offset, int flags);

/* glibc internal aliases */
void *__libc_realloc (void *ptr, size_t size);
void *__libc_memalign (size_t alignment, size_t size);
float __asinf_finite (float x);
int __isinff (float x);
int __isnanf (float x);
extern unsigned long __fpu_control;
const char *gnu_get_libc_version (void);
const char *gnu_get_libc_release (void);
int malloc_trim (size_t pad);

int pthread_tryjoin_np (pthread_t thread, void **retval);
int pthread_timedjoin_np (pthread_t thread, void **retval,
                          const struct timespec *abstime);
int pthread_yield (void);

int close_range (unsigned int first, unsigned int last, int flags);

/* GNU obstack, see <obstack.h> for the full header. */
struct obstack;

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_H */
