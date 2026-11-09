/* Minimal utmpx declarations provided by muslext. */

#ifndef MUSLEXT_UTMPX_H
#define MUSLEXT_UTMPX_H 1

#include <utmpx.h>

#ifdef __cplusplus
extern "C" {
#endif

struct utmpx *getutxent (void);
void setutxent (void);
void endutxent (void);
struct utmpx *getutxid (const struct utmpx *id);
struct utmpx *getutxline (const struct utmpx *line);
struct utmpx *pututxline (const struct utmpx *utmpx);
int utmpxname (const char *file);
void updwtmpx (const char *file, const struct utmpx *entry);

#ifndef WTMPX_FILE
# define WTMPX_FILE "/var/log/wtmp"
#endif

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_UTMPX_H */
