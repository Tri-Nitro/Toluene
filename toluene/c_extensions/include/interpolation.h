#ifndef __INTERPOLATION_H__
#define __INTERPOLATION_H__

#ifdef __cplusplus
extern "C" {
#endif

void bilinear_interpolation(double x, double y, double corners[], double* value);

#ifdef __cplusplus
}   /* extern "C" */
#endif

#endif /* __EARTH_GRAVITATIONAL_MODEL_H__ */