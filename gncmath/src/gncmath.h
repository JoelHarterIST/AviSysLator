#ifndef _GNCMATH_H
#define _GNCMATH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <math.h>

#define DBL_EPS (1E-30)

/// \brief vector (x, y, z)
///
/// 
typedef struct {
	double x;
	double y;
	double z;
} vec3_t;

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
// operation for vec3_t
// ---------------------------------------------------------------------------

/// \brief print vec3
///
/// args: vec3_t a, char* str
/// return: none
void print_v3(vec3_t a, char* str);

/// \brief return zeros vec3
///
/// args: none
/// return: vec3_t
vec3_t zeros_v3();

/// \brief return ones vec3
///
/// args: none
/// return: vec3_t
vec3_t ones_v3();

/// \brief convert value to vec3
///
/// args: double x, double y, double z
/// return: vec3_t
vec3_t val_to_v3(double x, double y, double z);

/// \brief convert array to vec3
///
/// args: double* a
/// return: vec3_t
vec3_t array_to_v3(double* a);

/// \brief convert vec3 to array
///
/// args: vec3_t v, double* a
/// return: none
void v3_to_array(vec3_t v, double* a);

/// \brief norm of vec3
///
/// args: vec3_t v
/// return: norm(v)
double norm_v3(vec3_t v);

/// \brief normaliz vec3
///
/// args: vec3_t v
/// return: v/norm(v)
vec3_t nmlz_v3(vec3_t v);

/// \brief addition of vec3 and vec3
///
/// args: vec3_t a, vec3_t a
/// return: a + b
vec3_t add_v3_v3(vec3_t a, vec3_t b);

/// \brief addition of three vec3
///
/// args: vec3_t a, vec3_t b, vec3_t c
/// return: a + b + c
vec3_t add_v3_3(vec3_t a, vec3_t b, vec3_t c);

/// \brief addition of four vec3
///
/// args: vec3_t a, vec3_t b, vec3_t c, vec3_t d
/// return: a + b + c + d
vec3_t add_v3_4(vec3_t a, vec3_t b, vec3_t c, vec3_t d);

/// \brief subtraction of vec3 and vec3
///
/// args: vec3_t a, vec3_t b
/// return: a - b
vec3_t sub_v3_v3(vec3_t a, vec3_t b);

/// \brief scalar product of scalar and vec3
///
/// args: double s, vec3_t v
/// return: s (scalar product) v
vec3_t scl_v3(double s, vec3_t v);

/// \brief hadamard product of vec3 and vec3
///
/// args: vec3_t v1, vec3_t v2
/// return: v1 (hadamard product) v2
vec3_t hdm_v3_v3(vec3_t a, vec3_t b);

/// \brief dot product of vec3 and vec3
///
/// args: vec3_t a, vec3_t b
/// return: a (dot product) b
double dot_v3_v3(vec3_t a, vec3_t b);

/// \brief cross product of mat33 and mat33
///
/// args: vec3_t a, vec3_t b
/// return: a (cross product) b
vec3_t crs_v3_v3(vec3_t a, vec3_t b);


// ---------------------------------------------------------------------------
// operation for mat33_t and vec3_t
// ---------------------------------------------------------------------------

/// \brief dot product of mat33 and vec3
///
/// args: mat33_t m, vec3_t v
/// return: m (dot product) v
vec3_t dot_m33_v3(mat33_t m, vec3_t v);


// ---------------------------------------------------------------------------
// operation for mat33_t
// ---------------------------------------------------------------------------

/// \brief print mat33
///
/// args: mat33_t m, char* str
/// return: none
void print_m33(mat33_t s, char* str);

/// \brief return zeros mat33
///
/// args: none
/// return: mat33_t
mat33_t zeros_m33();

/// \brief return ones mat33
///
/// args: none
/// return: mat33_t
mat33_t ones_m33();

/// \brief return indentity matrix of mat33
///
/// args: none
/// return: mat33_t
mat33_t eye_m33();

/// \brief convert 321 euler angle to direction cosine matrix
///
/// args: vec3_t e
/// return: dcm(e)
mat33_t euler321_to_dcm(vec3_t e);

/// \brief conver direction cosine matrix to 321 euler angle
///
/// args: mat33_t m, vec3_t v
/// return: 321 euler angle
/// remarks: v is used for error handling
vec3_t dcm_to_euler321(mat33_t m, vec3_t v);

/// \brief transpose of mat33
///
/// args: mat33_t m
/// return: trns(m)
mat33_t trns_m33(mat33_t s);

/// \brief determinant of mat33
///
/// args: mat33_t m
/// return: det(m)
double det_m33(mat33_t s);

/// \brief inverse of mat33
///
/// args: mat33_t m
/// return: inv(m)
mat33_t inv_m33(mat33_t s);

/// \brief dot product of mat33 and mat33
///
/// args: mat33_t s, mat33_t d
/// return: s (dot product) d
mat33_t dot_m33_m33(mat33_t s, mat33_t d);


// ---------------------------------------------------------------------------
// operation for quat_t
// ---------------------------------------------------------------------------

/// \brief print quat_t
///
/// args: quat_t a, char* str
/// return: none
void print_q(quat_t q, char* str);

/// \brief return ones quaternion (scaler part=1, vector part=0)
///
/// args: none
/// return: quat_t
quat_t ones_q();

/// \brief convert value to q
///
/// args: double s, double x, double y, double z
/// return: quat_t
quat_t val_to_q(double s, double x, double y, double z);

/// \brief convert array to q
///
/// args: double* a
/// return: quat_t
quat_t array_to_q(double* a);

/// \brief convert q to array
///
/// args: quat_t q, double* a
/// return: none
void q_to_array(quat_t q, double* a);

/// \brief norm of quaternion
///
/// args: quat_t q
/// return: norm(q)
double norm_q(quat_t q);

/// \brief normaliz quaternion
///
/// args: quat_t q
/// return: q/norm(q)
quat_t nmlz_q(quat_t q);

/// \brief convert 321 euler angle to quaternion
///
/// args: vec3_t e
/// return: q
quat_t euler321_to_q(vec3_t e);

/// \brief convert quaternion to 321 euler angle
///
/// args: vec3_t e
/// return: q
vec3_t q_to_euler321(quat_t q);

/// \brief convert direction cosine matrix to quaternion
///
/// args: mat33_t mat
/// return: q
quat_t dcm_to_q(mat33_t mat);

/// \brief conjugate quaternion
///
/// args: quat_t q
/// return: conjugate(q)
quat_t cnjg_q(quat_t q);

/// \brief quaternion product
///
/// args: quat_t q, quat_t p
/// return: q (quaternion product) p
quat_t qpro(quat_t q, quat_t p);

/// \brief convert quaternion to direction cosine matrix
///
/// args: quat_t q
/// return: dcm
mat33_t q_to_dcm(quat_t q);

/// \brief calculate q a->b
///
/// args: quat_t qra (q_r->a), quat_t qrb (q_r->b)
/// return: qab (q_a->b)
quat_t qerr(quat_t qra, quat_t qrb);

/// \brief calculate dq/dt
///
/// args: vec3_t w (angular velocity), quat_t q
/// return: dq/dt
quat_t dq_dt(vec3_t w, quat_t q);

#ifdef __cplusplus
} // extern "C"
#endif

#endif

