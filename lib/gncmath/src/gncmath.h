/// @file gncmath.h
/// @brief functions and structures of gncmath

#ifndef __GNCMATH_H__
#define __GNCMATH_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <math.h>

#define DBL_EPS (1E-30)

/// \brief vector (x, y)
///
/// 
typedef struct {
	double x;
	double y;
} vec2_t;

/// \brief vector (x, y, z)
///
/// 
typedef struct {
	double x;
	double y;
	double z;
} vec3_t;

/// \brief matrix (2 by 3)
///
/// 
typedef struct {
	vec2_t a;
	vec2_t b;
	vec2_t c;
} mat23_t;

/// \brief matrix (3 by 2)
///
/// 
typedef struct {
	vec3_t a;
	vec3_t b;
} mat32_t;

/// \brief matrix (3 by 3)
///
/// 
typedef struct {
	vec3_t a;
	vec3_t b;
	vec3_t c;
} mat33_t;

/// \brief quaternion (s, x, y, z)
///
/// 
typedef struct {
	double s;
	double x;
	double y;
	double z;
} quat_t;


// ---------------------------------------------------------------------------
// operation for vec2_t
// ---------------------------------------------------------------------------
vec2_t zeros_v2(void);
vec2_t scl_v2(double s, vec2_t v);
double norm_v2(vec2_t v);
vec2_t nmlz_v2(vec2_t v);


// ---------------------------------------------------------------------------
// operation for vec3_t
// ---------------------------------------------------------------------------
void print_v3(vec3_t a, char* str);
vec3_t zeros_v3(void);
vec3_t ones_v3(void);
vec3_t val_to_v3(double x, double y, double z);
vec3_t array_to_v3(double* a);
void v3_to_array(vec3_t v, double* a);
double norm_v3(vec3_t v);
vec3_t nmlz_v3(vec3_t v);
vec3_t add_v3_v3(vec3_t a, vec3_t b);
vec3_t add_v3_3(vec3_t a, vec3_t b, vec3_t c);
vec3_t add_v3_4(vec3_t a, vec3_t b, vec3_t c, vec3_t d);
vec3_t sub_v3_v3(vec3_t a, vec3_t b);
vec3_t scl_v3(double s, vec3_t v);
vec3_t hdm_v3_v3(vec3_t a, vec3_t b);
double dot_v3_v3(vec3_t a, vec3_t b);
vec3_t crs_v3_v3(vec3_t a, vec3_t b);


// ---------------------------------------------------------------------------
// operations dealing with 2D vectors and arrays with length or width of 2
// ---------------------------------------------------------------------------
mat32_t trns_m23(mat23_t s);
mat23_t trns_m32(mat32_t s);
vec3_t mult_m32_v2(mat32_t m, vec2_t v);
vec2_t mult_m23_v3(mat23_t m, vec3_t v);


// ---------------------------------------------------------------------------
// operation for mat33_t and vec3_t
// ---------------------------------------------------------------------------
vec3_t mult_m33_v3(mat33_t m, vec3_t v);


// ---------------------------------------------------------------------------
// operation for mat33_t
// ---------------------------------------------------------------------------
void print_m33(mat33_t s, char* str);
mat33_t zeros_m33(void);
mat33_t ones_m33(void);
mat33_t eye_m33(void);
mat33_t euler321_to_dcm(vec3_t e);
vec3_t dcm_to_euler321(mat33_t m, vec3_t v);
mat33_t trns_m33(mat33_t s);
double det_m33(mat33_t s);
mat33_t inv_m33(mat33_t s);
mat33_t dot_m33_m33(mat33_t s, mat33_t d);


// ---------------------------------------------------------------------------
// operation for quat_t
// ---------------------------------------------------------------------------
void print_q(quat_t q, char* str);
quat_t eye_q(void);
quat_t val_to_q(double s, double x, double y, double z);
quat_t array_to_q(double* a);
void q_to_array(quat_t q, double* a);
double norm_q(quat_t q);
quat_t nmlz_q(quat_t q);
quat_t euler321_to_q(vec3_t e);
vec3_t q_to_euler321(quat_t q);
quat_t dcm_to_q(mat33_t mat);
quat_t qcnjg(quat_t q);
quat_t qpro(quat_t q, quat_t p);
mat33_t q_to_dcm(quat_t q);
quat_t qerr(quat_t qra, quat_t qrb);
quat_t calc_dq_dt(vec3_t w, quat_t q);


#ifdef __cplusplus
} // extern "C"
#endif

#endif

