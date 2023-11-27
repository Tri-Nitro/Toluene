#ifndef __ROTATION_MATRICES_H__
#define __ROTATION_MATRICES_H__

#ifdef __cplusplus
extern "C" {
#endif

void compute_polynomial(const double coefficients[], double x, int highest_order, double* value);
void compute_iau_coefficients(double tt_seconds, double iau_coefficients[]);
void compute_precession_matrix(double iau_coefficients[], double precession_matrix[]);
void compute_nutation_arguments(double tt_seconds, double nutation_arguments[]);
void compute_nutation_matrix(double nutation_arguments[], double nutation_matrix[]);
void lookup_closest_delta_t(double tt_seconds, double* delta_t);
void compute_terrestrial_matrix(double tt_seconds, double equation_of_the_equinoxes, double terrestrial_matrix[]);
void compute_polar_motion_matrix(double tt_seconds, double polar_motion_matrix[]);

#ifdef __cplusplus
}   /* extern "C" */
#endif

#endif /* __ROTATION_MATRICES_H__ */