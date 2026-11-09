/* Signal and errno name/description helpers. */

#define _GNU_SOURCE 1

#include <errno.h>
#include <signal.h>
#include <stddef.h>
#include <string.h>

#include <muslext/muslext.h>

struct signal_name
{
  int number;
  const char *abbreviation;
};

static const struct signal_name signal_names[] =
{
  { SIGHUP, "HUP" }, { SIGINT, "INT" }, { SIGQUIT, "QUIT" },
  { SIGILL, "ILL" }, { SIGTRAP, "TRAP" }, { SIGABRT, "ABRT" },
  { SIGBUS, "BUS" }, { SIGFPE, "FPE" }, { SIGKILL, "KILL" },
  { SIGUSR1, "USR1" }, { SIGSEGV, "SEGV" }, { SIGUSR2, "USR2" },
  { SIGPIPE, "PIPE" }, { SIGALRM, "ALRM" }, { SIGTERM, "TERM" },
  { SIGSTKFLT, "STKFLT" }, { SIGCHLD, "CHLD" }, { SIGCONT, "CONT" },
  { SIGSTOP, "STOP" }, { SIGTSTP, "TSTP" }, { SIGTTIN, "TTIN" },
  { SIGTTOU, "TTOU" }, { SIGURG, "URG" }, { SIGXCPU, "XCPU" },
  { SIGXFSZ, "XFSZ" }, { SIGVTALRM, "VTALRM" }, { SIGPROF, "PROF" },
  { SIGWINCH, "WINCH" }, { SIGIO, "IO" }, { SIGPWR, "PWR" },
  { SIGSYS, "SYS" },
};

static int
mask_to_sigset (int mask, sigset_t *set)
{
  int signal;

  if (sigemptyset (set) != 0)
    return -1;
  for (signal = 1; signal < 32; signal++)
    if ((mask & (1 << signal)) != 0 && sigaddset (set, signal) != 0)
      return -1;
  return 0;
}

static int
sigset_to_mask (const sigset_t *set)
{
  int mask = 0;
  int signal;

  for (signal = 1; signal < 32; signal++)
    if (sigismember (set, signal) == 1)
      mask |= 1 << signal;
  return mask;
}

int
sigblock (int mask)
{
  sigset_t set;
  sigset_t old;
  int result;

  if (mask_to_sigset (mask, &set) != 0)
    return -1;
  if (sigprocmask (SIG_BLOCK, &set, &old) != 0)
    return -1;
  result = sigset_to_mask (&old);
  return result;
}

int
sigsetmask (int mask)
{
  sigset_t set;
  sigset_t old;
  int result;

  if (mask_to_sigset (mask, &set) != 0)
    return -1;
  if (sigprocmask (SIG_SETMASK, &set, &old) != 0)
    return -1;
  result = sigset_to_mask (&old);
  return result;
}

const char *
sigabbrev_np (int signal)
{
  size_t i;

  for (i = 0; i < sizeof (signal_names) / sizeof (signal_names[0]); i++)
    if (signal_names[i].number == signal)
      return signal_names[i].abbreviation;
  return NULL;
}

const char *
sigdescr_np (int signal)
{
  if (sigabbrev_np (signal) == NULL)
    return NULL;
  return strsignal (signal);
}

struct errno_name
{
  int number;
  const char *name;
};

#define E(name) { name, #name }
static const struct errno_name errno_names[] =
{
  E(EPERM), E(ENOENT), E(ESRCH), E(EINTR), E(EIO), E(ENXIO), E(E2BIG),
  E(ENOEXEC), E(EBADF), E(ECHILD), E(EAGAIN), E(ENOMEM), E(EACCES),
  E(EFAULT), E(EBUSY), E(EEXIST), E(EXDEV), E(ENODEV), E(ENOTDIR),
  E(EISDIR), E(EINVAL), E(ENFILE), E(EMFILE), E(ENOTTY), E(ETXTBSY),
  E(EFBIG), E(ENOSPC), E(ESPIPE), E(EROFS), E(EMLINK), E(EPIPE),
  E(EDOM), E(ERANGE), E(EDEADLK), E(ENAMETOOLONG), E(ENOLCK),
  E(ENOSYS), E(ENOTEMPTY), E(ELOOP), E(ENOMSG), E(EIDRM),
  E(EOVERFLOW), E(ENOTSOCK), E(EDESTADDRREQ), E(EMSGSIZE),
  E(EPROTOTYPE), E(ENOPROTOOPT), E(EPROTONOSUPPORT),
  E(ESOCKTNOSUPPORT), E(EOPNOTSUPP), E(EAFNOSUPPORT),
  E(EADDRINUSE), E(EADDRNOTAVAIL), E(ENETDOWN), E(ENETUNREACH),
  E(ENETRESET), E(ECONNABORTED), E(ECONNRESET), E(ENOBUFS),
  E(EISCONN), E(ENOTCONN), E(ETIMEDOUT), E(ECONNREFUSED),
  E(EHOSTDOWN), E(EHOSTUNREACH), E(EALREADY), E(EINPROGRESS),
  E(ESTALE), E(EDQUOT), E(ECANCELED), E(EOWNERDEAD),
  E(ENOTRECOVERABLE),
};

const char *
strerrorname_np (int error)
{
  size_t i;

  for (i = 0; i < sizeof (errno_names) / sizeof (errno_names[0]); i++)
    if (errno_names[i].number == error)
      return errno_names[i].name;
  return NULL;
}

const char *
strerrordesc_np (int error)
{
  return strerror (error);
}
