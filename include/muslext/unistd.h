/* Minimal unistd extensions provided by muslext. */

#ifndef MUSLEXT_UNISTD_H
#define MUSLEXT_UNISTD_H 1

#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

char *get_current_dir_name (void);
char *getwd (char *buf);
int getpt (void);
void closefrom (int lowfd);
int close_range (unsigned int first, unsigned int last, int flags);

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_UNISTD_H */
