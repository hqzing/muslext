/* Minimal math extensions provided by muslext. */

#ifndef MUSLEXT_MATH_H
#define MUSLEXT_MATH_H 1

#include <complex.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

float __asinf_finite (float x);
int __isinff (float x);
int __isnanf (float x);
double gamma (double x);
double roundeven (double x);
float roundevenf (float x);
double _Complex clog10 (double _Complex z);

#ifdef __cplusplus
}
#endif

#endif /* MUSLEXT_MATH_H */
