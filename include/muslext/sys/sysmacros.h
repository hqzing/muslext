/* Minimal device-number helpers provided by muslext. */

#ifndef MUSLEXT_SYS_SYSMACROS_H
#define MUSLEXT_SYS_SYSMACROS_H 1

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

unsigned int gnu_dev_major (dev_t dev);
unsigned int gnu_dev_minor (dev_t dev);
dev_t gnu_dev_makedev (unsigned int major, unsigned int minor);

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_SYS_SYSMACROS_H */
