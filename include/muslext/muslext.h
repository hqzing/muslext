/* muslext - compatibility extensions for musl libc.
 *
 * This header is the single entry point for consumers.  It includes all
 * minimal headers and the version/ABI macros.  It never replaces a system
 * header.
 */

#ifndef MUSLEXT_H
#define MUSLEXT_H 1

#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif

#include <muslext/crypt.h>
#include <muslext/dirent.h>
#include <muslext/fpu_control.h>
#include <muslext/getpass.h>
#include <muslext/glob.h>
#include <muslext/gnu.h>
#include <muslext/gnu-libc-version.h>
#include <muslext/malloc.h>
#include <muslext/math.h>
#include <muslext/program.h>
#include <muslext/pthread.h>
#include <muslext/qsort_r.h>
#include <muslext/resolv.h>
#include <muslext/sched.h>
#include <muslext/setjmp.h>
#include <muslext/shadow.h>
#include <muslext/signal.h>
#include <muslext/statx.h>
#include <muslext/stdlib.h>
#include <muslext/string.h>
#include <muslext/sys/mount.h>
#include <muslext/sys/sysmacros.h>
#include <muslext/sys/timex.h>
#include <muslext/sys/uio.h>
#include <muslext/unistd.h>
#include <muslext/utmpx.h>
#include <muslext/wchar.h>
#include <sys/pidfd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MUSLEXT_VERSION_MAJOR 1
#define MUSLEXT_VERSION_MINOR 3
#define MUSLEXT_VERSION_PATCH 0
#define MUSLEXT_ABI_VERSION 1

#define MUSLEXT_VERSION_STRINGIFY_(x) #x
#define MUSLEXT_VERSION_STRINGIFY(x) MUSLEXT_VERSION_STRINGIFY_(x)
#define MUSLEXT_VERSION \
  MUSLEXT_VERSION_STRINGIFY (MUSLEXT_VERSION_MAJOR) "." \
  MUSLEXT_VERSION_STRINGIFY (MUSLEXT_VERSION_MINOR) "." \
  MUSLEXT_VERSION_STRINGIFY (MUSLEXT_VERSION_PATCH)

/* GNU obstack, see <obstack.h> for the full header. */
struct obstack;

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_H */
