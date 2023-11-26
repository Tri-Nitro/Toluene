#ifndef __MATRIX_ROTATIONS_H__
#define __MATRIX_ROTATIONS_H__

#ifdef __cplusplus
extern "C" {
#endif

double compute_polynomial(const double coefficients[], double x, int highest_order);
void compute_iau_coefficients(double tt_seconds, double iau_coefficients[]);
void compute_precession_matrix(double iau_coefficients[], double precession_matrix[]);
void compute_nutation_arguments(double tt_seconds, double nutation_arguments[]);
void compute_nutation_matrix(double nutation_arguments[], double nutation_matrix[]);
double lookup_closest_delta_t(double tt_seconds);
void compute_terrestrial_matrix(double tt_seconds, double equation_of_the_equinoxes, double terrestrial_matrix[]);
void compute_polar_motion_matrix(double tt_seconds, double polar_motion_matrix[]);

#ifdef __cplusplus
}   /* extern "C" */
#endif

#endif /* __MATRIX_ROTATIONS_H__ */
