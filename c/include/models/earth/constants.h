/*
 * MIT License
 *
 * Copyright (c) 2023 Tri-Nitro
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * */
#ifndef __MODELS_EARTH_CONSTANTS_H__
#define __MODELS_EARTH_CONSTANTS_H__

#ifdef __cplusplus
extern "C" {
#endif

extern const long double MEAN_OBLIQUITY_EARTH[6];

extern const long double GENERAL_PRECESSION_LONGITUDE[3];
extern const long double PRECESSION_EQUATOR[6];
extern const long double OBLIQUITY_MEAN_EQUATOR[6];
extern const long double PRECESSION_ECLIPTIC_MEAN_EQUATOR[6];

extern const long double ICRS_X_POLE_OFFSET;
extern const long double ICRS_Y_POLE_OFFSET;
extern const long double ICRS_RIGHT_ASCENSION_OFFSET;

extern const long double GMST_FUNCTION_JULIAN_DU[6];
extern const long double ERA_DUT1[2];
extern const long double EQUATION_OF_ORIGINS[544];

extern const long double CHANDLER_WOBBLE;
extern const long double ANNUAL_WOBBLE;

#ifdef __cplusplus
}   /* extern "C" */
#endif /* __cplusplus */


#endif /* __MODELS_EARTH_CONSTANTS_H__ */