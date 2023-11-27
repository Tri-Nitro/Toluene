#ifndef __EARTH_GRAVITATIONAL_MODEL_H__
#define __EARTH_GRAVITATIONAL_MODEL_H__

#ifdef __cplusplus
extern "C" {
#endif

void find_egm84_four_corners(double latitude, double longitude, char* egm84_interpolation_grid_file_path,
    double grid_spacing, double four_corners[]);

#ifdef __cplusplus
}   /* extern "C" */
#endif

#endif /* __EARTH_GRAVITATIONAL_MODEL_H__ */
