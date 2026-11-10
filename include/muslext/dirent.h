/* Minimal declaration of the GNU versionsort(3) extension. */

#ifndef MUSLEXT_DIRENT_H
#define MUSLEXT_DIRENT_H 1

#include <dirent.h>

#ifdef __cplusplus
extern "C" {
#endif

int versionsort (const struct dirent **a, const struct dirent **b);

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_DIRENT_H */
