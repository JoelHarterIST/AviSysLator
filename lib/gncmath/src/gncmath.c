/// @file gncmath.c
/// @brief gncmath functions
/// @note 命名規約として下記の例外規約を適用する(計算式の途中で簡潔に記述するため)。
///       - パッケージ名は省略し「(動詞)_(対象)_(補足説明)」とする。※自明の場合、対象・補足説明は省略可
///       - 変換関数は「(変換前)_to_(変換後)_(補足説明)」とする。※自明の補足説明は省略可
///       - 一般的な汎用ライブラリで使用されており、広く認知されている関数名称があればそれを使用する。
///         - zeros_(型名), ones_(型名), eye_(型名) は「MATLAB」から流用

#include "gncmath.h"


// ---------------------------------------------------------------------------
// operation for vec3_t
// ---------------------------------------------------------------------------

/// @brief print vec3
/// @param [in] vec3_t a
/// @param [in] char* str
/// @note print "vec3_t (str)\n (a.x) (a.y) (a.z)\n"
void print_v3(vec3_t a, char* str) {
	printf("vec3_t %s\n", str);
	printf(" %+12.6e %+12.6e %+12.6e\n", a.x, a.y, a.z);

	return;
}

/// @brief return zeros vec3
/// @return vec3_t [0.0, 0.0, 0.0]
vec3_t zeros_v3(void) {
	vec3_t _ans;
	_ans.x = 0.0;
	_ans.y = 0.0;
	_ans.z = 0.0;

	return _ans;
}

/// @brief return ones vec3
/// @return vec3_t [1.0, 1.0, 1.0]
vec3_t ones_v3(void) {
	vec3_t _ans;
	_ans.x = 1.0;
	_ans.y = 1.0;
	_ans.z = 1.0;

	return _ans;
}

/// @brief convert value to vec3
/// @param [in] double x
/// @param [in] double y
/// @param [in] double z
/// @return vec3_t [x, y, z]
vec3_t val_to_v3(double x, double y, double z) {
	vec3_t _ans;
	_ans.x = x;
	_ans.y = y;
	_ans.z = z;

	return _ans;
}

/// @brief convert array to vec3
/// @param [in] double* a
/// @return vec3_t [a[0], a[1], a[2]]
vec3_t array_to_v3(double* a) {
	vec3_t _ans;
	_ans.x = a[0];
	_ans.y = a[1];
	_ans.z = a[2];

	return _ans;
}

/// @brief convert vec3 to array
/// @param [in] vec3_t v
/// @param [out] double* a
/// @note a[0] = v.x, a[1] = v.y, a[2] = v.z
void v3_to_array(vec3_t v, double* a) {
	a[0] = v.x;
	a[1] = v.y;
	a[2] = v.z;

	return;
}

/// @brief norm of vec3
/// @param [in] vec3_t v
/// @return double sqrt(v.x^2 + v.y^2 + v.z^2)
double norm_v3(vec3_t v) {
	double _ans = v.x*v.x + v.y*v.y + v.z*v.z;

	return sqrt(_ans);
}

/// @brief normaliz vec3
/// @param [in] vec3_t v
/// @return vec3_t v/norm(v)
vec3_t nmlz_v3(vec3_t v) {
	double  _r = norm_v3(v);
	vec3_t _ans;

	if (DBL_EPS > _r)  return v;

	_ans.x = v.x / _r;
	_ans.y = v.y / _r;
	_ans.z = v.z / _r;

	return _ans;
}

/// @brief addition of vec3 and vec3
/// @param [in] vec3_t a
/// @param [in] vec3_t b
/// @return vec3_t a + b
vec3_t add_v3_v3(vec3_t a, vec3_t b) {
	vec3_t _ans;
	_ans.x = a.x + b.x;
	_ans.y = a.y + b.y;
	_ans.z = a.z + b.z;

	return _ans;
}

/// @brief addition of three vec3
/// @param [in] vec3_t a
/// @param [in] vec3_t b
/// @param [in] vec3_t c
/// @return vec3_t a + b + c
vec3_t add_v3_3(vec3_t a, vec3_t b, vec3_t c) {
	vec3_t _ans;
	_ans.x = a.x + b.x + c.x;
	_ans.y = a.y + b.y + c.y;
	_ans.z = a.z + b.z + c.z;

	return _ans;
}

/// @brief addition of four vec3
/// @param [in] vec3_t a
/// @param [in] vec3_t b
/// @param [in] vec3_t c
/// @param [in] vec3_t d
/// @return vec3_t a + b + c + d
vec3_t add_v3_4(vec3_t a, vec3_t b, vec3_t c, vec3_t d) {
	vec3_t _ans;
	_ans.x = a.x + b.x + c.x + d.x;
	_ans.y = a.y + b.y + c.y + d.y;
	_ans.z = a.z + b.z + c.z + d.z;

	return _ans;
}

/// @brief subtraction of vec3 and vec3
/// @param [in] vec3_t a
/// @param [in] vec3_t b
/// @return vec3_t a - b
vec3_t sub_v3_v3(vec3_t a, vec3_t b) {
	vec3_t _ans;
	_ans.x = a.x - b.x;
	_ans.y = a.y - b.y;
	_ans.z = a.z - b.z;

	return _ans;
}

/// @brief scalar product of scalar and vec3
/// @param [in] double s
/// @param [in] vec3_t v
/// @return vec3_t s (scalar product) v
vec3_t scl_v3(double s, vec3_t v) {
	vec3_t _ans;
	_ans.x = s*v.x;
	_ans.y = s*v.y;
	_ans.z = s*v.z;

	return _ans;
}

/// @brief hadamard product of vec3 and vec3
/// @param [in] vec3_t a
/// @param [in] vec3_t b
/// @return vec3_t a (hadamard product) b
vec3_t hdm_v3_v3(vec3_t a, vec3_t b) {
	vec3_t _ans;
	_ans.x = a.x * b.x;
	_ans.y = a.y * b.y;
	_ans.z = a.z * b.z;

	return _ans;
}

/// @brief dot product of vec3 and vec3
/// @param [in] vec3_t a
/// @param [in] vec3_t b
/// @return double a (dot product) b
double dot_v3_v3(vec3_t a, vec3_t b) {
	double _ans;
	_ans =  a.x*b.x + a.y*b.y + a.z*b.z;

	return _ans;
}

/// @brief cross product
/// @param [in] vec3_t a
/// @param [in] vec3_t b
/// @return vec3_t a (cross product) b
vec3_t crs_v3_v3(vec3_t a, vec3_t b) {
	vec3_t _ans;
	_ans.x = a.y*b.z - a.z*b.y;
	_ans.y = a.z*b.x - a.x*b.z;
	_ans.z = a.x*b.y - a.y*b.x;

	return _ans;
}


// ---------------------------------------------------------------------------
// operation for mat32_t and vec2_t
// ---------------------------------------------------------------------------

/// @brief product mat32 and vec2
/// @param [in] mat32_t m
/// @param [in] vec2_t v
/// @return vec3_t m (product) v
vec3_t mult_m32_v2(mat32_t m, vec2_t v) {
	vec3_t _ans;
	_ans.x = m.a.x * v.x + m.b.x * v.y;
	_ans.y = m.a.y * v.x + m.b.y * v.y;
	_ans.z = m.a.z * v.x + m.b.z * v.y;

	return _ans;
}

// ---------------------------------------------------------------------------
// operation for mat23_t and vec3_t
// ---------------------------------------------------------------------------

/// @brief product mat23 and vec3
/// @param [in] mat23_t m
/// @param [in] vec3_t v
/// @return vec2_t m (product) v
vec2_t mult_m23_v3(mat23_t m, vec3_t v) {
	vec2_t _ans;
	_ans.x = m.a.x * v.x + m.b.x * v.y + m.c.x * v.z;
	_ans.y = m.a.y * v.x + m.b.y * v.y + m.c.y * v.z;

	return _ans;
}

// ---------------------------------------------------------------------------
// operation for mat33_t and vec3_t
// ---------------------------------------------------------------------------

/// @brief product mat33 and vec3
/// @param [in] mat33_t m
/// @param [in] vec3_t v
/// @return vec3_t m (product) v
vec3_t mult_m33_v3(mat33_t m, vec3_t v) {
	vec3_t _ans;
	_ans.x = m.a.x * v.x + m.b.x * v.y + m.c.x * v.z;
	_ans.y = m.a.y * v.x + m.b.y * v.y + m.c.y * v.z;
	_ans.z = m.a.z * v.x + m.b.z * v.y + m.c.z * v.z;

	return _ans;
}


// ---------------------------------------------------------------------------
// operation for mat33_t
// ---------------------------------------------------------------------------

/// @brief print mat33
/// @param [in] mat33_t s
/// @param [in] char* str
/// @note print "mat33_t (str)\n (s.a.x) (s.b.x) (s.c.x)\n (s.a.y) (s.b.y) (s.c.y)\n (s.a.z) (s.b.z) (s.c.z)\n"
void print_m33(mat33_t s, char* str) {
	printf("mat33_t %s\n", str);
	printf(" %+12.6e %+12.6e %+12.6e\n", s.a.x, s.b.x, s.c.x);
	printf(" %+12.6e %+12.6e %+12.6e\n", s.a.y, s.b.y, s.c.y);
	printf(" %+12.6e %+12.6e %+12.6e\n", s.a.z, s.b.z, s.c.z);

	return;
}

/// @brief return zeros mat33
/// @return mat33_t [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]]
mat33_t zeros_m33(void) {
	mat33_t _ans;
	_ans.a = zeros_v3();
	_ans.b = zeros_v3();
	_ans.c = zeros_v3();

	return _ans;
}

/// @brief return ones mat33
/// @return mat33_t [[1.0, 1.0, 1.0], [1.0, 1.0, 1.0], [1.0, 1.0, 1.0]]
mat33_t ones_m33(void) {
	mat33_t _ans;
	_ans.a = ones_v3();
	_ans.b = ones_v3();
	_ans.c = ones_v3();

	return _ans;
}

/// @brief return indentity matrix of mat33
/// @return mat33_t [[1.0, 0.0, 0.0], [0.0, 1.0, 0.0], [0.0, 0.0, 1.0]]
mat33_t eye_m33(void) {
	mat33_t _ans;
	_ans.a.x = 1.0; _ans.b.x = 0.0; _ans.c.x = 0.0;
	_ans.a.y = 0.0; _ans.b.y = 1.0; _ans.c.y = 0.0;
	_ans.a.z = 0.0; _ans.b.z = 0.0; _ans.c.z = 1.0;

	return _ans;
}

/// @brief convert 321 euler angle to direction cosine matrix
/// @param [in] vec3_t e
/// @return mat33_t dcm(e)
mat33_t euler321_to_dcm(vec3_t e) {
	mat33_t _ans;
	_ans.a.x = cos(e.z)*cos(e.y);
	_ans.a.y = cos(e.z)*sin(e.y)*sin(e.x) - sin(e.z)*cos(e.x);
	_ans.a.z = cos(e.z)*sin(e.y)*cos(e.x) + sin(e.z)*sin(e.x);
	_ans.b.x = sin(e.z)*cos(e.y);
	_ans.b.y = sin(e.z)*sin(e.y)*sin(e.x) + cos(e.z)*cos(e.x);
	_ans.b.z = sin(e.z)*sin(e.y)*cos(e.x) - cos(e.z)*sin(e.x);
	_ans.c.x = -sin(e.y);
	_ans.c.y = cos(e.y)*sin(e.x);
	_ans.c.z = cos(e.y)*cos(e.x);

	return _ans;
}

/// @brief conver direction cosine matrix to 321 euler angle
/// @param [in] mat33_t m
/// @param [in] vec3_t v
/// @return vec3_t euler321(m)
/// @note v is used to avoid singularity
vec3_t dcm_to_euler321(mat33_t m, vec3_t v) {
	double _r;
	vec3_t _ans;

	_r = sqrt(m.c.y * m.c.y + m.c.z * m.c.z);

	if (_r == 0 && m.c.x == 0) _ans.y = v.y;
	else _ans.y = atan2(-m.c.x, _r);

	if (m.c.y == 0 && m.c.z == 0) {
		_ans.x = v.x;
		_ans.z = v.z;
	}
	else {
		_ans.x = atan2(m.c.y, m.c.z);
		_ans.z = atan2(m.b.x, m.a.x);
	}

	return _ans;
}

/// @brief transpose of mat23
/// @param [in] mat23_t s
/// @return mat33_t trns(s)
mat32_t trns_m23(mat23_t s) {
	mat32_t _ans;

	_ans.a.x = s.a.x;
	_ans.b.x = s.a.y;

	_ans.a.y = s.b.x;
	_ans.b.y = s.b.y;

	_ans.a.z = s.c.x;
	_ans.b.z = s.c.y;

	return _ans;
}

/// @brief transpose of mat32
/// @param [in] mat32_t s
/// @return mat33_t trns(s)
mat23_t trns_m32(mat32_t s) {
	mat23_t _ans;

	_ans.a.x = s.a.x;
	_ans.b.x = s.a.y;
	_ans.c.x = s.a.z;

	_ans.a.y = s.b.x;
	_ans.b.y = s.b.y;
	_ans.c.y = s.b.z;

	return _ans;
}

/// @brief transpose of mat33
/// @param [in] mat33_t s
/// @return mat33_t trns(s)
mat33_t trns_m33(mat33_t s) {
	mat33_t _ans;

	_ans.a.x = s.a.x;
	_ans.b.x = s.a.y;
	_ans.c.x = s.a.z;

	_ans.a.y = s.b.x;
	_ans.b.y = s.b.y;
	_ans.c.y = s.b.z;

	_ans.a.z = s.c.x;
	_ans.b.z = s.c.y;
	_ans.c.z = s.c.z;

	return _ans;
}

/// @brief determinant of mat33
/// @param [in] mat33_t s
/// @return double det(s)
double det_m33(mat33_t s) {
	double _ans;

	_ans = s.a.x * s.b.y * s.c.z + s.c.x * s.a.y * s.b.z + s.b.x * s.c.y * s.a.z
		- s.c.x * s.b.y * s.a.z - s.a.x * s.c.y * s.b.z - s.c.z * s.b.x * s.a.y;

	return _ans;
}

/// @brief inverse of mat33
/// @param [in] mat33_t s
/// @return mat33_t inv(s)
mat33_t inv_m33(mat33_t s) {
	mat33_t _ans;
	double _det = det_m33(s);

	if (DBL_EPS > fabs(_det)) return s;

	_ans.a.x = (s.b.y*s.c.z - s.c.y*s.b.z) / _det;
	_ans.b.x = (s.b.z*s.c.x - s.c.z*s.b.x) / _det;
	_ans.c.x = (s.b.x*s.c.y - s.c.x*s.b.y) / _det;
	_ans.a.y = (s.c.y*s.a.z - s.a.y*s.c.z) / _det;
	_ans.b.y = (s.c.z*s.a.x - s.a.z*s.c.x) / _det;
	_ans.c.y = (s.c.x*s.a.y - s.a.x*s.c.y) / _det;
	_ans.a.z = (s.a.y*s.b.z - s.b.y*s.a.z) / _det;
	_ans.b.z = (s.a.z*s.b.x - s.b.z*s.a.x) / _det;
	_ans.c.z = (s.a.x*s.b.y - s.b.x*s.a.y) / _det;

	return _ans;
}

/// @brief hadamard product of mat33 and mat33
/// @param [in] mat33_t s
/// @param [in] mat33_t d
/// @return mat33_t s (hadamard product) d
mat33_t dot_m33_m33(mat33_t s, mat33_t d) {
	mat33_t _ans;

	_ans.a.x = s.a.x * d.a.x + s.b.x * d.a.y + s.c.x * d.a.z;
	_ans.b.x = s.a.x * d.b.x + s.b.x * d.b.y + s.c.x * d.b.z;
	_ans.c.x = s.a.x * d.c.x + s.b.x * d.c.y + s.c.x * d.c.z;

	_ans.a.y = s.a.y * d.a.x + s.b.y * d.a.y + s.c.y * d.a.z;
	_ans.b.y = s.a.y * d.b.x + s.b.y * d.b.y + s.c.y * d.b.z;
	_ans.c.y = s.a.y * d.c.x + s.b.y * d.c.y + s.c.y * d.c.z;

	_ans.a.z = s.a.z * d.a.x + s.b.z * d.a.y + s.c.z * d.a.z;
	_ans.b.z = s.a.z * d.b.x + s.b.z * d.b.y + s.c.z * d.b.z;
	_ans.c.z = s.a.z * d.c.x + s.b.z * d.c.y + s.c.z * d.c.z;

	return _ans;
}


// ---------------------------------------------------------------------------
// operation for quat_t
// ---------------------------------------------------------------------------

/// @brief print quat_t
/// @param [in] quat_t q
/// @param [in] char* str
/// @note print "quat_t (str)\n (q.s) (q.x) (q.y) (q.z)\n"
void print_q(quat_t q, char* str) {
	printf("quat_t %s\n", str);
	printf(" %+12.6e %+12.6e %+12.6e %+12.6e\n", q.s, q.x, q.y, q.z);

	return;
}

/// @brief return ones quaternion (scaler part=1, vector part=0)
/// @return quat_t [1.0, 0.0, 0.0, 0.0]
quat_t eye_q(void) {
	quat_t _ans;
	_ans.s = 1.0;
	_ans.x = 0.0;
	_ans.y = 0.0;
	_ans.z = 0.0;

	return _ans;
}

/// @brief convert value to q
/// @param [in] double s
/// @param [in] double x
/// @param [in] double y
/// @param [in] double z
/// @return quat_t [s, x, y, z]
quat_t val_to_q(double s, double x, double y, double z) {
	quat_t _ans;
	_ans.s = s;
	_ans.x = x;
	_ans.y = y;
	_ans.z = z;

	return _ans;
}

/// @brief convert array to q
/// @param [in] double* a
/// @return quat_t [a[0], a[1], a[2], a[3]]
quat_t array_to_q(double* a) {
	quat_t _ans;
	_ans.s = a[0];
	_ans.x = a[1];
	_ans.y = a[2];
	_ans.z = a[3];

	return _ans;
}

/// @brief convert q to array
/// @param [in] quat_t q
/// @param [out] double* a
/// @note a[0] = q.s, a[1] = q.x, a[2] = q.y, a[3] = q.z
void q_to_array(quat_t q, double* a) {
	a[0] = q.s;
	a[1] = q.x;
	a[2] = q.y;
	a[3] = q.z;

	return;
}

/// @brief norm of quaternion
/// @param [in] quat_t q
/// @return double sqrt(q.s^2 + q.x^2 + q.y^2 + q.z^2)
double norm_q(quat_t q) {
	double _ans = q.s*q.s + q.x*q.x + q.y*q.y + q.z*q.z;

	return sqrt(_ans);
}

/// @brief normalize quaternion
/// @param [in] quat_t q
/// @return quat_t q/norm(q)
quat_t nmlz_q(quat_t q) {
	double  _r = norm_q(q);
	quat_t _ans;

	if (DBL_EPS > _r)  return q;

	_ans.s = q.s / _r;
	_ans.x = q.x / _r;
	_ans.y = q.y / _r;
	_ans.z = q.z / _r;

	return _ans;
}

/// @brief convert 321 euler angle to quaternion
/// @param [in] vec3_t e
/// @return quat_t q(e)
/// @note euler321 = [roll, pitch, yaw]
quat_t euler321_to_q(vec3_t e) {
	quat_t _ans;
	_ans.s =  cos(e.x / 2.0)*cos(e.y / 2.0)*cos(e.z / 2.0) + sin(e.x / 2.0)*sin(e.y / 2.0)*sin(e.z / 2.0);
	_ans.x =  sin(e.x / 2.0)*cos(e.y / 2.0)*cos(e.z / 2.0) - cos(e.x / 2.0)*sin(e.y / 2.0)*sin(e.z / 2.0);
	_ans.y =  cos(e.x / 2.0)*sin(e.y / 2.0)*cos(e.z / 2.0) + sin(e.x / 2.0)*cos(e.y / 2.0)*sin(e.z / 2.0);
	_ans.z =  cos(e.x / 2.0)*cos(e.y / 2.0)*sin(e.z / 2.0) - sin(e.x / 2.0)*sin(e.y / 2.0)*cos(e.z / 2.0);

	return nmlz_q(_ans);
}

/// @brief convert quaternion to 321 euler angle
/// @param [in] quat_t q
/// @return vec3_t euler321(q)
/// @note euler321 = [roll, pitch, yaw]
vec3_t q_to_euler321(quat_t q) {
	vec3_t _ans;

	_ans.x = atan2(2.0*(q.s*q.x + q.y*q.z), 1 - 2.0*(q.x*q.x + q.y*q.y));
	_ans.y = asin(2.0*(q.s*q.y - q.x*q.z));
	_ans.z = atan2(2.0*(q.s*q.z + q.x*q.y), 1 - 2.0*(q.y*q.y + q.z*q.z));

	return _ans;
}

/// @brief convert direction cosine matrix to quaternion
/// @param [in] mat33_t mat
/// @return quat_t q(mat)
quat_t dcm_to_q(mat33_t mat) {
	double  _r;
	quat_t _ans;
	mat33_t _m;
	
	_m = trns_m33(mat);


	if ((_m.a.x + _m.b.y + _m.c.z) >= 0.0) {
		_r = sqrt(_m.a.x + _m.b.y + _m.c.z + 1.0);
		_ans.s = 0.5 * _r;
		_ans.x = 0.5 / _r * (_m.b.z - _m.c.y);
		_ans.y = 0.5 / _r * (_m.c.x - _m.a.z);
		_ans.z = 0.5 / _r * (_m.a.y - _m.b.x);
	}
	else {
		if ((_m.a.x >= _m.b.y) && (_m.a.x >= _m.c.z)) {
			_r = sqrt(_m.a.x - _m.b.y - _m.c.z + 1.0);
			_ans.s = 0.5 / _r * (_m.b.z - _m.c.y);
			_ans.x = 0.5 * _r;
			_ans.y = 0.5 / _r * (_m.b.x + _m.a.y);
			_ans.z = 0.5 / _r * (_m.a.z + _m.c.x);
		}
		else if ((_m.b.y >= _m.c.z) && (_m.b.y >= _m.a.x)) {
			_r = sqrt(_m.b.y - _m.c.z - _m.a.x + 1.0);
			_ans.s = 0.5 / _r * (_m.c.x - _m.a.z);
			_ans.x = 0.5 / _r * (_m.b.x + _m.a.y);
			_ans.y = 0.5 * _r;
			_ans.z = 0.5 / _r * (_m.c.y + _m.b.z);
		}
		else {
			_r = sqrt(_m.c.z - _m.a.x - _m.b.y + 1.0);
			_ans.s = 0.5 / _r * (_m.a.y - _m.b.x);
			_ans.x = 0.5 / _r * (_m.a.z + _m.c.x);
			_ans.y = 0.5 / _r * (_m.c.y + _m.b.z);
			_ans.z = 0.5 * _r;
		}
	}

	_ans = nmlz_q(_ans);

	return _ans;
}

/// @brief conjugate quaternion
/// @param [in] quat_t q
/// @return quat_t conjugate(q)
quat_t qcnjg(quat_t q) {
	quat_t _ans;
	_ans.s = q.s;
	_ans.x = -q.x;
	_ans.y = -q.y;
	_ans.z = -q.z;

	return _ans;
}

/// @brief quaternion product
/// @param [in] quat_t q
/// @param [in] quat_t p
/// @return quat_t q (quaternion product) p
quat_t qpro(quat_t q, quat_t p) {
	quat_t _ans;
	_ans.s =  q.s*p.s - q.x*p.x - q.y*p.y - q.z*p.z;
	_ans.x =  q.x*p.s + q.s*p.x - q.z*p.y + q.y*p.z;
	_ans.y =  q.y*p.s + q.z*p.x + q.s*p.y - q.x*p.z;
	_ans.z =  q.z*p.s - q.y*p.x + q.x*p.y + q.s*p.z;

	return _ans;
}

/// @brief convert quaternion to direction cosine matrix
/// @param [in] quat_t q
/// @return mat33_t dcm(q)
mat33_t q_to_dcm(quat_t q) {
	mat33_t _ans;
	double _sum;

	_sum = q.s*q.s + q.x*q.x + q.y*q.y + q.z*q.z;
	_ans.a.x = ((q.x*q.x + q.s*q.s) - (q.y*q.y + q.z*q.z)) / _sum;
	_ans.b.x = (2.0*q.x*q.y + 2.0*q.z*q.s) / _sum;
	_ans.c.x = (2.0*q.x*q.z - 2.0*q.y*q.s) / _sum;
	_ans.a.y = (2.0*q.x*q.y - 2.0*q.z*q.s) / _sum;
	_ans.b.y = ((q.y*q.y + q.s*q.s) - (q.x*q.x + q.z*q.z)) / _sum;
	_ans.c.y = (2.0*q.y*q.z + 2.0*q.x*q.s) / _sum;
	_ans.a.z = (2.0*q.x*q.z + 2.0*q.y*q.s) / _sum;
	_ans.b.z = (2.0*q.y*q.z - 2.0*q.x*q.s) / _sum;
	_ans.c.z = ((q.z*q.z + q.s*q.s) - (q.x*q.x + q.y*q.y)) / _sum;

	return _ans;
}

/// @brief calculate q a->b
/// @param [in] quat_t qra (q_r->a)
/// @param [in] quat_t qrb (q_r->b)
/// @return quat_t qab (q_a->b)
quat_t qerr(quat_t qra, quat_t qrb) {
	quat_t _qar = qcnjg(qra);
	quat_t _ans = qpro(_qar, qrb);

	return _ans;
}

/// @brief calculate dq/dt
/// @param [in] vec3_t w (angular velocity)
/// @param [in] quat_t q
/// @return quat_t dq/dt
quat_t calc_dq_dt(vec3_t w, quat_t q) {
	quat_t _ans;
	_ans.s =  0.5*(        - w.x*q.x - w.y*q.y - w.z*q.z);
	_ans.x =  0.5*(w.x*q.s           + w.z*q.y - w.y*q.z);
	_ans.y =  0.5*(w.y*q.s - w.z*q.x           + w.x*q.z);
	_ans.z =  0.5*(w.z*q.s + w.y*q.x - w.x*q.y          );

	return _ans;
}

