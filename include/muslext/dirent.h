/* Minimal declaration of the GNU versionsort(3) extension. */

#ifndef MUSLEXT_DIRENT_H
#define MUSLEXT_DIRENT_H 1

#include <dirent.h>

#ifdef __cplusplus
extern "C" {
#endif

int versionsort (const struct dirent **a, const struct dirent **b);
int scandirat (int dirfd, const char *dirp, struct dirent ***namelist,
               int (*filter) (const struct dirent *),
               int (*compar) (const struct dirent **, const struct dirent **));

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_DIRENT_H */
