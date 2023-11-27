#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "earth_gravitational_model.h"

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif


void find_egm84_four_corners(double latitude, double longitude, char* egm84_interpolation_grid_file_path,
    double grid_spacing, double four_corners[]) {

    FILE * fp;

   fp = fopen (egm84_interpolation_grid_file_path, "r");

    if (fp == NULL) {
        return;
    }

    // Convert to a 0 to 360 degree range
    if(longitude < 0.0) {
        longitude = 360.0 + longitude;
    }

    char line[50];

    double latitude_spacing = fmod(latitude, grid_spacing);
    double longitude_spacing = fmod(longitude, grid_spacing);

    double latitude_lower = latitude - latitude_spacing;
    double latitude_upper = latitude + (grid_spacing-latitude_spacing);

    double longitude_lower = longitude - longitude_spacing;
    double longitude_upper = longitude + (grid_spacing-longitude_spacing);

    int lines_per_latitude = (int)(360.0 / grid_spacing) + 1;

    int current_line = 0;
    int value_index = 0;
    int line_number = (int)((90-latitude_upper)/grid_spacing)*lines_per_latitude + (int)(longitude_lower/grid_spacing) + 1;

    while (fgets(line, sizeof line, fp) != NULL) {
        ++current_line;

        if(current_line == line_number) {
            sscanf(line, "%lf %lf %lf",
                &four_corners[value_index*3],
                &four_corners[value_index*3+1],
                &four_corners[value_index*3+2]);
            if(value_index == 0) {
                line_number = (int)((90-latitude_upper)/grid_spacing)*lines_per_latitude + (int)(longitude_upper/grid_spacing) + 1;
                ++value_index;
            } else if(value_index == 1) {
                line_number = (int)((90-latitude_lower)/grid_spacing)*lines_per_latitude + (int)(longitude_lower/grid_spacing) + 1;
                ++value_index;
            } else if(value_index == 2) {
                line_number = (int)((90-latitude_lower)/grid_spacing)*lines_per_latitude + (int)(longitude_upper/grid_spacing) + 1;
                ++value_index;
            } else if(value_index == 3) {
                break;
            }
        }
    }

    fclose(fp);
}

#ifdef __cplusplus
extern "C"
{
#endif