/* Minimal mount API extensions provided by muslext. */

#ifndef MUSLEXT_SYS_MOUNT_H
#define MUSLEXT_SYS_MOUNT_H 1

#include <stddef.h>
#include <sys/mount.h>

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_SYS_MOUNT_H */
