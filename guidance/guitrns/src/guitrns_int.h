/// @file guitrns_int.h
/// @brief internals of guidance translation


#ifndef __GUITRNS_INT_H__
#define __GUITRNS_INT_H__


// # システムヘッダの取り込み
// none


// # ユーザ作成ヘッダの取り込み
// ## 共通ヘッダ
#include "common.h"

// ## 他モジュールヘッダ
#include "gncmath.h"

// ## 自モジュールヘッダ
// none


// # #defineマクロ
// none


// # #define関数マクロ
// none


// # typedef定義（intやcharといった基本型に対する型定義）
// ## abbreviations
// delta -> dlt
// epsiron -> eps
// gamma -> gmm
// lambda -> lmd
// theta -> tht

// ## param
typedef struct {
	double r_d;
	double v_d;
	double i_d;
	double gmm_d;
	double f_thrust;
	double mass_0;
	double mass_dot;
	double i_sp;
	double v_ex;
	double eps;
	double eps_r;
	double eps_v;
	double d_damp;
	int32_t i_corr_min;
	int32_t i_corr_max_init;
	int32_t i_corr_max_update;
	int32_t i_pred_max;
	double t_go_min;
	int32_t cyc_wait;
	double dt;
} guitrns_param_t;

// ## state
typedef struct {
	int32_t mode;
	int32_t mode_peg;
	int32_t status;
	vec3_t d_v_;
	vec3_t u_y_;
	double t_go;
	double k_time;
	vec3_t lmd_;
	vec3_t lmd_dot_;
	double lmd_dot;
	vec3_t v_p_;
	vec3_t r_p_;
	double r_p;
	vec3_t r_d_;
	vec3_t v_d_;
	double mass;
	double a_thrust;
	vec3_t dlt_v_thrust_;
	vec3_t p_;
	vec3_t f_thrust_;
	vec3_t a_thrust_;
	int32_t cyc_cnt;
	double t;
	double t_c;
} guitrns_state_t;


// # enumタグ定義（typedefを同時に行う）
// none


// # struct/unionタグ定義（typedefを同時に行う）
// none


// # extern変数宣言
extern guitrns_param_t gtp;
extern guitrns_state_t gts;


// # 関数プロトタイプ宣言
// none


// # インライン関数
// none


#endif

