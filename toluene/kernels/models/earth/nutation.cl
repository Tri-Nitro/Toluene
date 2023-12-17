__kernel void nutation_values_of_date(__global const double* nutation_critical_arguments,
    __global const double* nutation_coefficients, __global const double* time,
    __global double* nutation_values, __global int* size) {

    int tid = get_global_id(0);

    double ai = nutation_coefficients[tid*20] * nutation_critical_arguments[0] +
        nutation_coefficients[tid*20+1] * nutation_critical_arguments[1] +
        nutation_coefficients[tid*20+2] * nutation_critical_arguments[2] +
        nutation_coefficients[tid*20+3] * nutation_critical_arguments[3] +
        nutation_coefficients[tid*20+4] * nutation_critical_arguments[4] +
        nutation_coefficients[tid*20+5] * nutation_critical_arguments[5] +
        nutation_coefficients[tid*20+6] * nutation_critical_arguments[6] +
        nutation_coefficients[tid*20+7] * nutation_critical_arguments[7] +
        nutation_coefficients[tid*20+8] * nutation_critical_arguments[8] +
        nutation_coefficients[tid*20+9] * nutation_critical_arguments[9] +
        nutation_coefficients[tid*20+10] * nutation_critical_arguments[10] +
        nutation_coefficients[tid*20+11] * nutation_critical_arguments[11] +
        nutation_coefficients[tid*20+12] * nutation_critical_arguments[12] +
        nutation_coefficients[tid*20+13] * nutation_critical_arguments[13];

    double sin_ai = sinpi(ai/648000.0);
    double cos_ai = cospi(ai/648000.0);

    nutation_values[tid*3] = (nutation_coefficients[tid*20+14] + nutation_coefficients[tid*20+15] * time[0]) * sin_ai
        + nutation_coefficients[tid*20+16] * cos_ai;
    nutation_values[tid*3+1] = (nutation_coefficients[tid*20+17] + nutation_coefficients[tid*20+18] * time[0]) * cos_ai
        + nutation_coefficients[tid*20+19] * sin_ai;
    nutation_values[tid*3+2] = nutation_coefficients[tid*20+19] * sin_ai + nutation_coefficients[tid*20+16] * cos_ai;

    barrier( CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE );
    if(tid == 0) {
        for(int i = 1; i < size[0]; i++) {
            nutation_values[0] += nutation_values[i*3];
            nutation_values[1] += nutation_values[i*3+1];
            nutation_values[2] += nutation_values[i*3+2];
        }
    }

}