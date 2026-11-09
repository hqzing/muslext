/* fts(3) interface, provided for systems whose libc lacks it. */

#ifndef MUSLEXT_FTS_H
#define MUSLEXT_FTS_H 1

#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FTS_COMFOLLOW 0x001
#define FTS_LOGICAL 0x002
#define FTS_NOSTAT 0x008
#define FTS_NOCHDIR 0x004
#define FTS_PHYSICAL 0x010
#define FTS_SEEDOT 0x020
#define FTS_XDEV 0x040
#define FTS_WHITEOUT 0x080
#define FTS_OPTIONMASK 0x0ff
#define FTS_NAMEONLY 0x100
#define FTS_STOP 0x200

#define FTS_AGAIN 1
#define FTS_FOLLOW 2
#define FTS_NOINSTR 3
#define FTS_SKIP 4

#define FTS_D 1
#define FTS_DC 2
#define FTS_DEFAULT 3
#define FTS_DNR 4
#define FTS_DOT 5
#define FTS_DP 6
#define FTS_ERR 7
#define FTS_F 8
#define FTS_INIT 9
#define FTS_NS 10
#define FTS_NSOK 11
#define FTS_SL 12
#define FTS_SLNONE 13
#define FTS_W 14

struct _ftsent
{
  struct _ftsent *fts_cycle;
  struct _ftsent *fts_parent;
  struct _ftsent *fts_link;
  long fts_number;
  void *fts_pointer;
  char *fts_accpath;
  char *fts_path;
  int fts_errno;
  int fts_symfd;
  size_t fts_pathlen;
  size_t fts_namelen;
  unsigned short fts_info;
  unsigned short fts_flags;
  unsigned short fts_level;
  ino_t fts_ino;
  dev_t fts_dev;
  nlink_t fts_nlink;
  struct stat fts_stat;
  struct stat *fts_statp;
  char fts_name[1];
};

typedef struct _ftsent FTSENT;

typedef struct _fts
{
  FTSENT *fts_cur;
  FTSENT *fts_child;
  FTSENT **fts_array;
  dev_t fts_dev;
  char *fts_path;
  int fts_rfd;
  int fts_cwd_fd;
  size_t fts_pathlen;
  size_t fts_nitems;
  int (*fts_compar) (const FTSENT **, const FTSENT **);
  int fts_options;
  void *fts_internal;
} FTS;

FTS *fts_open (char *const *path_argv, int options,
               int (*compar) (const FTSENT **, const FTSENT **));
FTSENT *fts_read (FTS *fts);
FTSENT *fts_children (FTS *fts, int options);
int fts_set (FTS *fts, FTSENT *entry, int options);
int fts_close (FTS *fts);

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_FTS_H */
