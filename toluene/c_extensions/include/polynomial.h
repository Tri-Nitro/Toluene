#ifndef __POLYNOMIAL_H__
#define __POLYNOMIAL_H__

#ifdef __cplusplus
extern "C" {
#endif

void compute_polynomial(const double coefficients[], double x, int highest_order, double* value);

#ifdef __cplusplus
}   /* extern "C" */
#endif

#endif /* __POLYNOMIAL_H__ */