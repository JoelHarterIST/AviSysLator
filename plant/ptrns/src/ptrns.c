#include "ptrns.h"

ptrns_param_t ptp;
ptrns_state_t pts;

// # equation of translation with erath as point mass
// pos_dot_ = vel_
// vel_dot_ = -(MU_E/(pos^3)) pos_ + acl_
// pos_ : position vector
// pos  : position norm (distance between space craft and earth center)
// vel_ : velocity vector
// acl_ : acceleration vector
// MU_E : geocentric gravitational constant
void eq_of_system(double* x, double* dx, double* u_) {
	vec3_t _pos_ = array_to_v3(&x[0]);
	vec3_t _vel_ = array_to_v3(&x[3]);
	vec3_t _acl_ = array_to_v3(u_);
	double _pos = norm_v3(_pos_);
	double _neg_mue_pos_cube = - MU_E/(_pos*_pos*_pos);
	vec3_t _neg_mue_pos_cube_pos_ = scl_v3(_neg_mue_pos_cube, _pos_);
	vec3_t _neg_mue_xxx_acl_ = add_v3_v3(_neg_mue_pos_cube_pos_, _acl_);
	v3_to_array(_vel_, &dx[0]);
	v3_to_array(_neg_mue_xxx_acl_, &dx[3]);
}

void ptrns_main(double* t, double dt, ptrns_state_t* ppts) {
	int _step_rk4;
	int i;
	double _dt_rk4[4] = {0.0, (dt/2.0), (dt/2.0), dt};
	double _x_in[LEN_OF_STATE];
	double _x_tmp[LEN_OF_STATE];
	double _x_out[LEN_OF_STATE];
	double _u_[LEN_OF_STATE];
	double _k[4][LEN_OF_STATE];

	// integrate state by runge-kutta 4th
	v3_to_array(ppts->pos_, &_x_in[0]);
	v3_to_array(ppts->vel_, &_x_in[3]);
	v3_to_array(ppts->acl_, &_u_[0]);
	for (_step_rk4=0; _step_rk4 < 4; _step_rk4++) {
		if (0 == _step_rk4) {
			for(i=0; i<LEN_OF_STATE; i++) _x_tmp[i] = _x_in[i];
		}
		else {
			for(i=0; i<LEN_OF_STATE; i++) _x_tmp[i] = _x_in[i] + _dt_rk4[_step_rk4]*_k[_step_rk4-1][i];
		}
		eq_of_system(_x_tmp, &_k[_step_rk4][0], &_u_[0]);
	}
	for(i=0; i<LEN_OF_STATE; i++) {
		_x_out[i] = _x_in[i] + (dt/6.0)*(_k[0][i] + 2.0*_k[1][i] + 2.0*_k[2][i] + _k[3][i]);
	}

	// update time and state
	*t = *t + dt;
	ppts->pos_ = array_to_v3(&_x_out[0]);
	ppts->vel_ = array_to_v3(&_x_out[3]);
}
