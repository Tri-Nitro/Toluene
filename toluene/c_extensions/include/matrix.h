#ifndef __MATRIX_H__
#define __MATRIX_H__

#ifdef __cplusplus
extern "C" {
#endif

void matrix_dot_product_3_3_in_place(double a[], double b[]);
void matrix_transpose_3_3_in_place(double a[]);

void matrix_dot_product_3_3_vector_in_place(double a[], double b[]);

#ifdef __cplusplus
}   /* extern "C" */
#endif

#endif /* __MATRIX_H__ */