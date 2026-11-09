/* Minimal declaration of getpass(3). */

#ifndef MUSLEXT_GETPASS_H
#define MUSLEXT_GETPASS_H 1

#ifdef __cplusplus
extern "C" {
#endif

char *getpass (const char *prompt);

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_GETPASS_H */
