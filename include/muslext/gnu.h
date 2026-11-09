/* GNU constants missing from musl headers, safe to include alone. */

#ifndef MUSLEXT_GNU_H
#define MUSLEXT_GNU_H 1

#include <sys/stat.h>

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

#endif
