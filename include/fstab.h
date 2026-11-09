/* fstab database interface. */

#ifndef MUSLEXT_FSTAB_H
#define MUSLEXT_FSTAB_H 1

#ifdef __cplusplus
extern "C" {
#endif

struct fstab
{
  char *fs_spec;
  char *fs_file;
  char *fs_vfstype;
  char *fs_mntops;
  const char *fs_type;
  int fs_freq;
  int fs_passno;
};

int setfsent (void);
struct fstab *getfsent (void);
void endfsent (void);
struct fstab *getfsfile (const char *file);
struct fstab *getfsspec (const char *spec);

#define FSTAB_RW "rw"
#define FSTAB_RO "ro"
#define FSTAB_SW "sw"
#define FSTAB_XX "xx"
#define FSTAB_RQ "rq"

#ifdef __cplusplus
}
#endif

#endif
