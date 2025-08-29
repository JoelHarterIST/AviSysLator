/// @file ptrns.c
/// @brief plant translation


// # システムヘッダの取り込み
// none


// # ユーザ作成ヘッダの取り込み
// ## 共通ヘッダ
#include "common.h"

// ## 他モジュールヘッダ
#include "gncmath.h"
#include "guitrns_ext.h"

// ## 自モジュールヘッダ
#include "ptrns.h"
#include "ptrns_ext.h"
#include "ptrns_int.h"


// # 自ファイル内でのみ使用する#defineマクロ
#define LEN_OF_STATE (6)


// # 自ファイル内でのみ使用する#define関数マクロ
// none


// # 自ファイル内でのみ使用するtypedef定義
// none


// # 自ファイル内でのみ使用するenumタグ定義
// none


// # 自ファイル内でのみ使用するstruct/unionタグ定義
// none


// # ファイル内で共有するstatic変数宣言
// none


// # static関数宣言
// none


// # 変数定義
// ## input
ptrns_input_t pti = {
	.mode = PTRNS_MODE_OFF,
	.acl_ = (vec3_t){.x = 0.0, .y = 0.0, .z = 0.0}
};

// ## output
ptrns_output_t pto = {
	.mode = PTRNS_MODE_OFF,
	.pos_ = (vec3_t){.x = 0.0, .y = 0.0, .z = 0.0},
	.vel_ = (vec3_t){.x = 0.0, .y = 0.0, .z = 0.0}
};

// ## param
ptrns_param_t ptp = {
	.isp = 0.0 // [s]
};

// ## state
ptrns_state_t pts = {
	.mode = PTRNS_MODE_OFF,
	.pos_ = (vec3_t){.x = 0.0, .y = 0.0, .z = 0.0},
	.vel_ = (vec3_t){.x = 0.0, .y = 0.0, .z = 0.0},
	.acl_ = (vec3_t){.x = 0.0, .y = 0.0, .z = 0.0}
};


// # 関数定義
// ## 内部関数プロトタイプ宣言
void ptrns_update_mode(int32_t mode_input, int32_t* mode_state, int32_t* mode_output);
void ptrns_switch_mode(int32_t mode_state);
void ptrns_off(ptrns_input_t* ppti, ptrns_state_t* ppts, ptrns_output_t* ppto);
void ptrns_input(guitrns_output_t* pgto, ptrns_input_t* ppti);
void ptrns_output(ptrns_state_t* ppts, ptrns_output_t* ppto);
void ptrns_update_init(ptrns_input_t* ppti, ptrns_state_t* ppts);
void ptrns_update_rk4(ptrns_input_t* ppti, ptrns_state_t* ppts);
void eq_of_system(double* u, double* x, double* dx);

// ## 外部公開関数定義
/// @brief 並進プラントモデル : メイン
/// @param [in] slot_num slot_number
void ptrns_main(int32_t slot_num) {
	// 呼び出しスロット定義
	static const int32_t _slot_intvl = 1; // 呼び出しスロット間隔 [0.5ms]
	static const int32_t _slot_ofst = 0; // 呼び出しスロットオフセット [0.5ms]

	// 呼び出しスロット判定
	if ((slot_num % _slot_intvl) == _slot_ofst) {
		// モード更新
		ptrns_update_mode(pti.mode, &(pts.mode), &(pto.mode));
		// モード分岐
		ptrns_switch_mode(pts.mode);
	}
	else {
		// none
	}
}

// ## 内部関数定義
/// @brief 並進プラントモデル : モード更新
/// @param [in] mode_input
/// @param [out] mode_state
/// @param [out] mode_output
void ptrns_update_mode(int32_t mode_input, int32_t* mode_state, int32_t* mode_output) {
	*mode_state = mode_input;
	*mode_output = *mode_state;
}

/// @brief 並進プラントモデル : モード分岐
/// @param [in] mode_state
void ptrns_switch_mode(int32_t mode_state) {
	switch (mode_state) {
		case PTRNS_MODE_INIT:
			ptrns_update_init(&pti, &pts);
			ptrns_update_rk4(&pti, &pts);
			ptrns_output(&pts, &pto);
			break;
		case PTRNS_MODE_ON:
			ptrns_input(&gto, &pti);
			ptrns_update_rk4(&pti, &pts);
			ptrns_output(&pts, &pto);
			break;
		default: // including PTRNS_MODE_OFF
			ptrns_off(&pti, &pts, &pto);
			break;
	}
}


/// @brief 並進プラントモデル : 機能オフ
/// @param [out] ppti pointer to ptrns input
/// @param [out] ppts pointer to ptrns state
/// @param [out] ppto pointer to ptrns output
void ptrns_off(ptrns_input_t* ppti, ptrns_state_t* ppts, ptrns_output_t* ppto) {
	// off input
	ppti->acl_ = zeros_v3();

	// off state
	ppts->pos_ = zeros_v3();
	ppts->vel_ = zeros_v3();
	ppts->acl_ = zeros_v3();

	// off output
	ppto->pos_ = zeros_v3();
	ppto->vel_ = zeros_v3();
}

/// @brief 並進プラントモデル : 入力
/// @param [in] pgto pointer to guitrns output
/// @param [out] ppti pointer to ptrns input
void ptrns_input(guitrns_output_t* pgto, ptrns_input_t* ppti) {
	ppti->acl_ = pgto->a_thrust_;
}

/// @brief 並進プラントモデル : 出力
/// @param [in] ppts pointer to ptrns state
/// @param [out] ppto pointer to ptrns output
void ptrns_output(ptrns_state_t* ppts, ptrns_output_t* ppto) {
	ppto->pos_ = ppts->pos_;
	ppto->vel_ = ppts->vel_;
}

/// @brief 並進プラントモデル : 更新 : 初期化
/// @param [out] ppti pointer to ptrns input
/// @param [out] ppts pointer to ptrns state
void ptrns_update_init(ptrns_input_t* ppti, ptrns_state_t* ppts) {
	// init input

	// init state
}

/// @brief 回転プラントモデル : 更新 : ルンゲクッタ4次
/// @param [in] ppri pointer to plant rotation input
/// @param [in,out] pprs pointer to plant rotation state
void ptrns_update_rk4(ptrns_input_t* ppti, ptrns_state_t* ppts) {
	int _step_rk4;
	int i;
	double _dt_rk4[4] = {0.0, (DT_OF_SLOT_S/2.0), (DT_OF_SLOT_S/2.0), DT_OF_SLOT_S};
	double _x_in[LEN_OF_STATE];
	double _x_tmp[LEN_OF_STATE];
	double _x_out[LEN_OF_STATE];
	double _u_[LEN_OF_STATE];
	double _k[4][LEN_OF_STATE];

	// # input
	ppts->acl_ = ppti->acl_;

	// # convert state to array
	v3_to_array(ppts->pos_, &_x_in[0]);
	v3_to_array(ppts->vel_, &_x_in[3]);
	v3_to_array(ppts->acl_, &_u_[0]);

	// integrate state by runge-kutta 4th
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
		_x_out[i] = _x_in[i] + (DT_OF_SLOT_S/6.0)*(_k[0][i] + 2.0*_k[1][i] + 2.0*_k[2][i] + _k[3][i]);
	}

	// # update state
	ppts->pos_ = array_to_v3(&_x_out[0]);
	ppts->vel_ = array_to_v3(&_x_out[3]);
}

/// @brief 並進プラントモデル : システム方程式 (translation, 3 degree of freedom)
/// @param [in] u input vector
/// @param [in] x state vector
/// @param [out] dx derivative of state vector
/// @note equation of translation with erath as point mass
/// @note _pos_dot_ = _vel_
/// @note _vel_dot_ = -(MU_E/(_pos^3)) _pos_ + _acl_
/// @note _pos_ : position vector
/// @note _pos  : position norm (distance between space craft and earth center)
/// @note _vel_ : velocity vector
/// @note _acl_ : acceleration vector
/// @note MU_E : geocentric gravitational constant
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

