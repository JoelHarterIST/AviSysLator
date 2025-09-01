/// @file guitrns.c
/// @brief guidance translation


// # システムヘッダの取り込み
// none


// # ユーザ作成ヘッダの取り込み
// ## 共通ヘッダ
#include "common.h"

// ## 他モジュールヘッダ
#include "gncmath.h"
#include "ptrns_ext.h"

// ## 自モジュールヘッダ
#include "guitrns.h"
#include "guitrns_ext.h"
#include "guitrns_int.h"


// # 自ファイル内でのみ使用する#defineマクロ
#define GUITRNS_EXIT_FAILURE_PEG_PRED_MAX (-1)
#define GUITRNS_EXIT_FAILURE_PEG_CORR_MAX (-2)
#define GUITRNS_EXIT_FAILURE_PEG_T_GO_MIN (-3)

#define GUITRNS_MODE_PEG_OFF (0)
#define GUITRNS_MODE_PEG_INIT (1)
#define GUITRNS_MODE_PEG_UPDATE (2)
#define GUITRNS_MODE_PEG_WAIT (3)
#define GUITRNS_MODE_PEG_FINAL (4)

#define GUITRNS_PEG_INIT_OFF (0)
#define GUITRNS_PEG_INIT_ON (1)


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
guitrns_input_t gti = {0};

// ## output
guitrns_output_t gto = {0};

// ## param
guitrns_param_t gtp = {0};

// ## state
guitrns_state_t gts = {0};


// # 関数定義
// ## 内部関数プロトタイプ宣言
void guitrns_update_mode(int32_t mode_input, int32_t* mode_state, int32_t* mode_output);
void guitrns_switch_mode(int32_t mode_state);
void guitrns_off(guitrns_input_t* pgti, guitrns_state_t* pgts, guitrns_output_t* pgto);
void guitrns_input(ptrns_output_t* ppto, guitrns_input_t* pgti);
void guitrns_output(guitrns_state_t* pgts, guitrns_output_t* pgto);
// int guitrns_main_(guitrns_input_t* pgti, guitrns_state_t* pgts);
void guitrns_update_peg(guitrns_input_t* pgti, guitrns_state_t* pgts);
void guitrns_update_peg_init(guitrns_state_t* pgts);
int guitrns_update_peg_core(int32_t init, vec3_t r_, vec3_t v_, guitrns_state_t* pgts);
void guitrns_update_peg_cmd_on(guitrns_state_t* pgts);
void guitrns_update_peg_cmd_off(guitrns_state_t* pgts);

// ## 外部公開関数定義
/// @brief 並進誘導 : メイン
/// @param [in] slot_num slot_number
void guitrns_main(int32_t slot_num) {
	// 呼び出しスロット定義
	static const int32_t _slot_intvl = 1; // 呼び出しスロット間隔 [0.5ms]
	static const int32_t _slot_ofst = 0; // 呼び出しスロットオフセット [0.5ms]

	// 呼び出しスロット判定
	if ((slot_num % _slot_intvl) == _slot_ofst) {
		// モード更新
		guitrns_update_mode(gti.mode, &(gts.mode), &(gto.mode));
		// モード分岐
		guitrns_switch_mode(gts.mode);
	}
	else {
		// none
	}
}

// ## 内部関数定義
/// @brief 並進誘導 : モード更新
/// @param [in] mode_input
/// @param [out] mode_state
/// @param [out] mode_output
void guitrns_update_mode(int32_t mode_input, int32_t* mode_state, int32_t* mode_output) {
	*mode_state = mode_input;
	*mode_output = *mode_state;
}

/// @brief 並進誘導 : モード分岐
/// @param [in] mode_state
void guitrns_switch_mode(int32_t mode_state) {
	// init gts.status
	gts.status = 0;

	switch (mode_state) {
		case GUITRNS_MODE_STAGE_1_INIT:
			// to be implemented
		case GUITRNS_MODE_STAGE_1:
			// to be implemented
			break;
		case GUITRNS_MODE_STAGE_2_INIT:
			gts.mode_peg = GUITRNS_MODE_PEG_INIT;
			gti.mode = GUITRNS_MODE_STAGE_2; // set mode for next cycle
		case GUITRNS_MODE_STAGE_2:
			guitrns_input(&pto, &gti);
			guitrns_update_peg(&gti, &gts);
			guitrns_output(&gts, &gto);
			break;
		default: // including GUITRNS_MODE_OFF
			guitrns_off(&gti, &gts, &gto);
			break;
	}
}

/// @brief 並進誘導 : 機能オフ
/// @param [out] pgti pointer to guitrns input
/// @param [out] pgts pointer to guitrns state
/// @param [out] pgto pointer to guitrns output
void guitrns_off(guitrns_input_t* pgti, guitrns_state_t* pgts, guitrns_output_t* pgto) {
	// off input
	// pgti->mode is excluded
	pgti->pos_ = zeros_v3();
	pgti->vel_ = zeros_v3();

	// off state
	// pgts->mode is excluded
	pgts->mode_peg = 0;
	pgts->status = 0;
	pgts->d_v_ = zeros_v3();
	pgts->u_y_ = zeros_v3();
	pgts->t_go = 0.0;
	pgts->k_time = 0.0;
	pgts->lmd_ = zeros_v3();
	pgts->lmd_dot_ = zeros_v3();
	pgts->lmd_dot = 0.0;
	pgts->v_p_ = zeros_v3();
	pgts->r_p_ = zeros_v3();
	pgts->r_p = 0.0;
	pgts->r_d_ = zeros_v3();
	pgts->v_d_ = zeros_v3();
	pgts->mass = 0.0;
	pgts->a_thrust = 0.0;
	pgts->dlt_v_thrust_ = zeros_v3();
	pgts->p_ = zeros_v3();
	pgts->f_thrust_ = zeros_v3();
	pgts->a_thrust_ = zeros_v3();
	pgts->cyc_cnt = 0;
	pgts->t = 0.0;
	pgts->t_c = 0.0;

	// off output
	// pgto->mode is excluded
	pgto->a_thrust_ = zeros_v3();
}

/// @brief 並進誘導 : 入力
/// @param [in] ppto pointer to ptrns output
/// @param [out] pgti pointer to guitrns input
void guitrns_input(ptrns_output_t* ppto, guitrns_input_t* pgti) {
	pgti->pos_ = ppto->pos_;
	pgti->vel_ = ppto->vel_;
}

/// @brief 並進誘導 : 出力
/// @param [in] pgts pointer to guitrns state
/// @param [out] pgto pointer to guitrns output
void guitrns_output(guitrns_state_t* pgts, guitrns_output_t* pgto) {
	pgto->a_thrust_ = pgts->a_thrust_;
}

/// @brief 並進誘導 : 更新：PEG
/// @param [in] pgti pointer to guitrns input
/// @param [in,out] pgts pointer to guitrns state
void guitrns_update_peg(guitrns_input_t* pgti, guitrns_state_t* pgts) {
	// init pgts->status
	pgts->status = 0;

	switch (pgts->mode_peg) {
		case GUITRNS_MODE_PEG_INIT:
			guitrns_update_peg_init(pgts);
			pgts->status = guitrns_update_peg_core(1, pgti->pos_, pgti->vel_, pgts);
			if (0 > pgts->status) {
				// error handle future implementation
			}
			pgts->t = 0.0;
			pgts->t_c = 0.0;
			pgts->cyc_cnt = 0;
			guitrns_update_peg_cmd_on(&gts);
			pgts->mode_peg = GUITRNS_MODE_PEG_WAIT; // set mode for next cycle
			break;
		case GUITRNS_MODE_PEG_UPDATE:
			pgts->a_thrust = gtp.f_thrust/pgts->mass;
			pgts->status = guitrns_update_peg_core(0, pgti->pos_, pgti->vel_, &gts);
			if (0 > pgts->status) {
				// error handle future implementation
			}
			pgts->t_c = 0.0;
			pgts->cyc_cnt = 0;
			pgts->dlt_v_thrust_ = zeros_v3();
			guitrns_update_peg_cmd_on(&gts);
			pgts->mode_peg = GUITRNS_MODE_PEG_WAIT; // set mode for next cycle
			break;
		case GUITRNS_MODE_PEG_WAIT:
			guitrns_update_peg_cmd_on(&gts);
			pgts->cyc_cnt++;
			if (gtp.cyc_wait <= pgts->cyc_cnt) {
				if (gtp.t_go_min < (pgts->t_go - 1.0)) {
					pgts->mode_peg = GUITRNS_MODE_PEG_UPDATE; // set mode for next cycle
				}
				else {
					pgts->mode_peg = GUITRNS_MODE_PEG_FINAL; // set mode for next cycle
				}
			}
			break;
		case GUITRNS_MODE_PEG_FINAL:
			if (gtp.t_go_min >= pgts->t_c) {
				guitrns_update_peg_cmd_on(&gts);
			}
			else {
				guitrns_update_peg_cmd_off(&gts);
				pgts->mode_peg = GUITRNS_MODE_PEG_OFF; // set mode for next cycle
			}
			break;
		default: // including GUITRNS_MODE_PEG_OFF
			guitrns_update_peg_cmd_off(&gts);
			break;
	}
}

/// @brief 並進誘導 : 更新 : PEG初期化
/// @param [out] pgts pointer to guitrns state
void guitrns_update_peg_init(guitrns_state_t* pgts) {
	// # set guitrns parameter
	// a = 6378137.0;% [m] semi-major axis <WGS-84>
	// r_D = Constants.a+230e3;  % [m] desired orbital radius
	gtp.r_d = 6378137.0 + 230e3;
	// i_D = 42.2;      % [deg] desired orbital inclination
	gtp.i_d = 42.2*DEG_RAD;
	// V_D = sqrt(Constants.mu/R_D);% [m/s] use default orbital speed for a circular orbit by default
	gtp.v_d = sqrt(MU_E/gtp.r_d);
	// s.orbitFlightPathAngle  (1,1)   double  = 0 % [deg] desired flight path angle at orbit insertion point; defaults to 0 (circular orbit)
	// gamma_D=s.orbitFlightPathAngle;
	gtp.gmm_d = 0.0;
	// from loaded init file
	gtp.i_sp = 344.32;
	// V_ex = g_0*I_sp;
	gtp.v_ex = G_E*gtp.i_sp;
	// from loaded init file
	gtp.f_thrust = 141260;
	// from loaded init file
	gtp.mass_0 = 12951.952;
	// m_dot = -F_T/g_0/I_sp;
	gtp.mass_dot = -gtp.f_thrust/G_E/gtp.i_sp;
	// s.epsilon               (1,1)   double  = 5e-2 % convergence criterion for F_V in the corrector
	gtp.eps = 5e-2;
	// s.epsilon_R             (1,1)   double  = 30   % [m] convergence criterion for stabilization of R_p_, approximately the same as suggested value of 100 ft <ref1 p577>
	gtp.eps_r = 30;
	// s.epsilon_V             (1,1)   double  = 1e-2 % [unitless] convergence criterion for D_V <ref1 p576>
	gtp.eps_v = 1e-2;
	// s.D                     (1,1)   double  = 0.5  % [unitless] damping factor for convergence of U_y_ <ref1 p577>
	gtp.d_damp = 0.5;
	// s.i_corr_min            (1,1)   double  = 3    % minimum number of iterations within corrector.
	gtp.i_corr_min = 2;
	// s.i_corr_max_initial    (1,1)   double  = 50   % maximum number of iterations within corrector
	gtp.i_corr_max_init = 50;
	// s.i_corr_max_update     (1,1)   double  = 1    % maximum number of iterations within corrector for all calls after first call (ref1 suggests only 1 iteration is necessary)
	gtp.i_corr_max_update = 1;
	// s.I_max                 (1,1)   double  = 10   % maximum number of iterations for R_p_ to converge. Source recommends I_max=5 but I have found I_max=10 to be better <ref1 p577>
	gtp.i_pred_max = 10;
	// s.T_GO_min              (1,1)   double  = 4    % [s] minimum time to go; warn user if T_GO drops below this value
	gtp.t_go_min = 4;

	gtp.cyc_wait = 99;

	gtp.dt = 0.01;

	// # set guitrns state
	pgts->a_thrust = gtp.f_thrust/gtp.mass_0;
	pgts->mass = gtp.mass_0;

	// # set guitrns mode peg
	pgts->mode_peg = GUITRNS_MODE_PEG_INIT;
}

/// @brief 並進誘導 : 更新 : PEGコア
/// @param [in] init initialization flag (1: needs to be initialize for first call, 0: otherwise)
/// @param [in] r_ vec3_t position vector [m]
/// @param [in] v_ vec3_t velocity vector [m/s]
/// @param [in,out] pgts pointer to guitrns state
int guitrns_update_peg_core(int32_t init, vec3_t r_, vec3_t v_, guitrns_state_t* pgts) {
	// # declaration
	// ## declaration for iteration counter
	int32_t i_corr = 0;
	int32_t i_pred = 0;

	// ## declaration for initialize
	double _r = 0.0;
	vec3_t _dlt_v_s_ = zeros_v3();
	int32_t _i_corr_max = 0;
	int32_t _skip = 0;

	// ## declaration for update
	vec3_t _d_v_ = zeros_v3();

	// ## declaration for time-to-go
	double _d_v = 0.0;
	double _l = 0.0;
	double _a_thrust = 0.0;
	double _tau = 0.0;
	double _t_b = 0.0;
	double _t_go = 0.0;

	// ## declaration for integrals
	double _s = 0.0;
	double _j = 0.0;
	double _q = 0.0;
	double _k = 0.0;
	double _k_p = 0.0;
	double _tht = 0.0;
	double _f_1 = 0.0;
	double _f_2 = 0.0;
	double _dlt = 0.0;
	double _cap_f_1 = 0.0;
	double _cap_f_2 = 0.0;
	double _cap_f_3 = 0.0;
	double _q_t = 0.0;
	double _s_t = 0.0;

	// ## declaration for range-to-go
	vec3_t _v_go_ = zeros_v3();
	double _mu_l = 0.0;
	double _r_f1 = 0.0;
	double _r_f2 = 0.0;
	double _tht_1 = 0.0;
	double _tht_2 = 0.0;
	double _b_1 = 0.0;
	double _b_2 = 0.0;
	double _t_1 = 0.0;
	double _t_2 = 0.0;
	vec3_t _r_d_t_2_ = zeros_v3();
	vec3_t _r_t_1_ = zeros_v3();
	vec3_t _v_v_d_v_go_ = zeros_v3();
	vec3_t _t_go_2_v_v_d_v_go_ = zeros_v3();			
	double _lmd_r_go = 0.0;
	double _s_t_lmd_r_go = 0.0;
	vec3_t _s_t_lmd_r_go_lmd_ = zeros_v3();
	vec3_t _r_go_ = zeros_v3();

	// ## declaration for predictor
	double _r_p = 0.0;
	double _r_p_cube = 0.0;
	double _a_1 = 0.0;
	double _a_2 = 0.0;
	double _a_3 = 0.0;
	double _a_4 = 0.0;
	vec3_t _a_3_r_ = zeros_v3();
	vec3_t _1_t_2_t_1_v_ = zeros_v3();
	vec3_t _t_2_1_v_go_ = zeros_v3();
	vec3_t _1_t_2_xxx_v_go_ = zeros_v3();
	vec3_t _t_go_xxx_v_go_ = zeros_v3();
	vec3_t _a_3_r_xxx_r_go_ = zeros_v3();
	vec3_t _r_p_ = zeros_v3();
	vec3_t _t_2_t_1_v_ = zeros_v3();
	vec3_t _a_1_r_ = zeros_v3();
	vec3_t _a_2_r_p_ = zeros_v3();
	vec3_t _t_2_v_go_ = zeros_v3();
	vec3_t _v_p_ = zeros_v3();
	double _norm_r_p = 0.0;
	double _abs_norm_r_p_r_p = 0.0;

	// ## declaration for convergence test
	double _v_miss = 0.0;
	double _eps_v_d_v = 0.0;

	// ## declaration for corrector
	double _u_y_r_p = 0.0;
	vec3_t _u_y_r_p_u_y_ = zeros_v3();
	vec3_t _r_p_u_y_r_p_u_y_ = zeros_v3();
	vec3_t _u_x_ = zeros_v3();
	vec3_t _r_d_ = zeros_v3();
	vec3_t _u_z_ = zeros_v3();
	vec3_t _u_x_sin_gmm_d_ = zeros_v3();
	vec3_t _u_z_cos_gmm_d_ = zeros_v3();
	vec3_t _u_x_sin_gmm_d_u_z_cos_gmm_d_ = zeros_v3();
	vec3_t _v_d_ = zeros_v3();
	vec3_t _v_miss_ = zeros_v3();
	vec3_t _lmd_v_ = zeros_v3();
	vec3_t _v_d_v_ = zeros_v3();
	double _lmd_v_v_d_v = 0.0;
	vec3_t _v_p_v_ = zeros_v3();
	double _lmd_v_p_v = 0.0;
	double _f_v = 0.0;
	double _1_u_x_z_square = 0.0;
	double _s_eps = 0.0;
	vec3_t _u_y_sqrt_1_s_eps_square_ = zeros_v3();
	vec3_t _s_eps_u_z_ = zeros_v3();
	vec3_t _u_y_ = zeros_v3();

	// ## declaration for compute steering command variables
	vec3_t _lmd_ = zeros_v3();
	vec3_t _s_t_lmd_ = zeros_v3();
	vec3_t _r_go_s_t_lmd_ = zeros_v3();
	vec3_t _lmd_dot_ = zeros_v3();
	double _lmd_dot = 0.0;


	// # initialize and unpack state
	// ## initialize for every call
	_r = norm_v3(r_);
	
	if (init) {
		// ## initialize parameter for first call
		_i_corr_max = gtp.i_corr_max_init;
		_skip = 1;

		// ## initialize state for first call <ref1 p576>
		_lmd_ = nmlz_v3(v_); // [unitless] <ref1 p576>
		_lmd_dot = 1E-5; // [s^-1] <ref1 p576>
		_v_p_ = scl_v3(1.001, v_); // [m/s] <ref1 p576>
		_r_p_ = r_; // [m] <ref1 p576>
		_r_p = norm_v3(_r_p_); // [m] magnitude of R_p_
		_u_y_ = nmlz_v3(crs_v3_v3(v_, r_)); // [unitless] unit vector in opposite direction current as angular velocity vector; this should converge to be normal to orbital plane
		_a_thrust = pgts->a_thrust;
		_dlt_v_s_ = pgts->dlt_v_thrust_;
	}
	else {
		// ## initialize for subsequent calls after first call
		_i_corr_max = gtp.i_corr_min;
		_skip = 0;

		// ## unpack state for subsequent calls after first call
		_t_go = pgts->t_go;
		_k = pgts->k_time;
		_d_v_ = pgts->d_v_;
		_r_p_ = pgts->r_p_;
		_r_p = pgts->r_p;
		_v_p_ = pgts->v_p_;
		_r_d_ = pgts->r_d_;
		_v_d_ = pgts->v_d_;
		_u_y_ = pgts->u_y_;
		_lmd_ = pgts->lmd_;
		_lmd_dot_ = pgts->lmd_dot_;
		_lmd_dot = pgts->lmd_dot;
		_a_thrust = pgts->a_thrust;
		_dlt_v_s_ = pgts->dlt_v_thrust_;
	}


	// # update <ref1 p577>
	// ## D_V_=D_V_-DeltaV_S_ [m/s]
	_d_v_ = sub_v3_v3(_d_v_, _dlt_v_s_);

	for (i_corr = 0; i_corr <= _i_corr_max; i_corr++) {
		// skip at 1st loop of initialization
		if (!_skip) {
			// # time-to-go <ref1 p577>
			// ## D_V = norm(D_V_) <ref1 p577>
			_d_v = norm_v3(_d_v_);

			// ## L = D_V [m/s] <ref1 p570,571>
			_l = _d_v;

			// ## tau = V_ex/a_thrust [s] ratio of current mass to mass flow rate. function of time. <ref1 p570>
			_tau = gtp.v_ex/_a_thrust;

			// ## T_B = tau*(1-exp(-L/V_ex)) [s] burn time <ref1 p571>
			_t_b = _tau*(1.0 - exp(-1.0*(_l/gtp.v_ex)));

			// ## T_GO = T_B [s] time-to-go <ref1 p571>
			_t_go = _t_b;


			// # integrals <ref1 p577>
			// ## S = -L*(tau-T_B)+V_ex*T_B [m] <ref1 p570 eq32>
			_s = -1.0*_l*(_tau - _t_b) + gtp.v_ex*_t_b;

			// ## J = L*T_B-S [m] <ref1 p571 eq33>
			_j = _l*_t_b - _s;

			// ## Q = S*tau-V_ex*T_B^2/2 [m*s] <ref1 p571 eq34>
			_q = _s*_tau - 0.5*gtp.v_ex*_t_b*_t_b;

			// ## K = T_GO-S/L [s] <ref1 p571 eq38>
			_k = _t_go - _s/_l;

			// ## K_p = T_GO/2 [s] <ref1 p572 eq39>
			_k_p = 0.5*_t_go;

			// ## theta = lambda_dot*K_p [unitless] <ref1 p572 eq39>
			_tht = _lmd_dot*_k_p;

			// ## f_1 = sin(theta)/theta [unitless] <ref1 p572 eq39>
			// _small_f_1 = sin(_tht)/_tht;
			_f_1 = sin(_tht)/_tht;

			// ## f_2 = 3*(f_1-cos(theta))/theta^2 [unitless] <ref1 p572 eq39>
			_f_2 = 3.0*(_f_1 - cos(_tht))/(_tht*_tht);

			// ## delta = lambda_dot*(K-T_GO/2) [unitless] <ref1 p573 eq48>
			_dlt = _lmd_dot*(_k - 0.5*_t_go);

			// ## F_1 = f_1*cos(delta) [unitless] <ref1 p573 eq48>
			_cap_f_1 = _f_1*cos(_dlt);

			// ## F_2 = f_2*cos(delta) [unitless] <ref1 p573 eq48>
			_cap_f_2 = _f_2*cos(_dlt);

			// ## F_3 = F_1*(1-theta*delta/3) [unitless] <ref1 p573 eq48>
			_cap_f_3 = _cap_f_1*(1.0 - _tht*_dlt/3.0); 

			// ## Q_T = F_2*(Q-S*K) [m*s] <ref1 p573 eq48>
			_q_t = _cap_f_2*(_q - _s*_k);

			// ## S_T = F_3*S [m] <ref1 p573 eq48>
			_s_t = _cap_f_3*_s;

			// # range-to-go  <ref1 p577>
			// ## V_GO_ = F_1*D_V_ [m/s] velocity-to-go <ref1 p577>
			_v_go_ = scl_v3(_cap_f_1, _d_v_);

			// ## select earth centric gravitational constant
			_mu_l = MU_E; // if g_nav wasn't provided
			// _mu_l = (TBI); // if g_nav was provided (TO BE IMPLEMENT)

			// ## R_F1 = 3*dot(V_,R_)/R^2 [s^-1] <ref1 p575>
			_r_f1 = 3.0*dot_v3_v3(v_, r_)/(_r*_r);

			// ## R_F2 = 3*dot(V_p_,R_p_)/R_pm^2 [s^-1] <ref1 p575>
			_r_f2 = 3.0*dot_v3_v3(_v_p_, _r_p_)/(_r_p*_r_p);

			// ## theta_1 = T_GO*sqrt(mu_L/R^3)/2 [unitless] <ref1 p575>
			_tht_1 = 0.5*_t_go*sqrt(_mu_l/(_r*_r*_r));

			// ## theta_2 = T_GO*sqrt(mu_L/R_pm^3)/2 [unitless] <ref1 p575>
			_tht_2 = 0.5*_t_go*sqrt(_mu_l/(_r_p*_r_p*_r_p));

			// ## B_1 = -2*theta_1^2/T_GO [s^-1] <ref1 p575>
			_b_1 = -2.0*_tht_1*_tht_1/_t_go;

			// ## B_2 = -2*theta_2^2/T_GO [s^-1] <ref1 p575>
			_b_2 = -2.0*_tht_2*_tht_2/_t_go;

			// ## T_1 = tan(theta_1)/theta_1 [unitless] <ref1 p575>
			_t_1 = tan(_tht_1)/_tht_1;

			// ## T_2 = tan(theta_2)/theta_2 [unitless] <ref1 p575>
			_t_2 = tan(_tht_2)/_tht_2;

			// ## R_GO_ = R_D_/T_2 - R_/T_1 - T_GO/2*(V_ + V_D_ - V_GO_) [m] range-to-go vector <ref1 p576 eq69>
			_r_d_t_2_ = scl_v3((1.0/_t_2), _r_d_);
			_r_t_1_ = scl_v3((1.0/_t_1), r_);
			_v_v_d_v_go_ = add_v3_3(v_, _v_d_, scl_v3(-1.0, _v_go_));
			_t_go_2_v_v_d_v_go_ = scl_v3((0.5*_t_go), _v_v_d_v_go_);
			_r_go_ = add_v3_3(_r_d_t_2_, scl_v3(-1.0, _r_t_1_), scl_v3(-1.0, _t_go_2_v_v_d_v_go_));

			// ## lambda_ = uvec(V_GO_) [unitless] average thrust direction unit vector <ref1 p576 just below eq69-a>
			_lmd_ = nmlz_v3(_v_go_);

			// ## R_GO_ = R_GO_ + (S_T - dot(lambda_,R_GO_))*lambda_ [m] range-to-go vector, enforcing that lambda_ and lambda_dot_ are orthogonal to eachother <ref1 p576 eq69-a>
			_lmd_r_go = dot_v3_v3(_lmd_, _r_go_);
			_s_t_lmd_r_go = _s_t - _lmd_r_go;
			_s_t_lmd_r_go_lmd_ = scl_v3(_s_t_lmd_r_go, _lmd_);
			_r_go_ = add_v3_v3(_r_go_, _s_t_lmd_r_go_lmd_);

			// # predictor <ref1 p577>
			for (i_pred = 0; i_pred < gtp.i_pred_max; i_pred++) {
				// ## R_pm = norm(R_p_) [m] magnitude of R_p_
				_r_p = norm_v3(_r_p_);

				// ## theta_2 = T_GO*sqrt(mu_L/R_pm^3)/2 [unitless] <ref1 p575>
				_r_p_cube = _r_p*_r_p*_r_p;
				_tht_2 = 0.5*_t_go*sqrt(_mu_l/_r_p_cube);

				// ## T_2 = tan(theta_2)/theta_2 [unitless] <ref1 p575>
				_t_2 = tan(_tht_2)/_tht_2;

				// ## A_1 = T_2*(B_1+(1-1/T_1)*R_F1) [s^-1] <ref1 p575>
				_a_1 = _t_2*(_b_1 + (1.0 - 1.0/_t_1)*_r_f1);

				// ## A_2 = T_2*B_2-(T_2-1)*R_F2 [s^-1] <ref1 p575>
				_a_2 = _t_2*_b_2 - (_t_2 - 1.0)*_r_f2;

				// ## A_3 = 1/T_1+A_1*T_GO/2 [unitless] <ref1 p575>
				_a_3 = 1.0/_t_1 + 0.5*_a_1*_t_go;

				// ## A_4 = 1/T_2-A_2*T_GO/2 [unitless] <ref1 p575>
				_a_4 = 1.0/_t_2 - 0.5*_a_2*_t_go;

				// ## R_p_ = (A_3*R_+T_GO/2*((1+T_2/T_1)*V_ + (T_2-1)*V_GO_)+R_GO_)/A_4 [m] predicted position <ref1 p575 eq61>
				_a_3_r_ = scl_v3(_a_3, r_);
				_1_t_2_t_1_v_ = scl_v3((1.0 + _t_2/_t_1), v_);
				_t_2_1_v_go_ = scl_v3((_t_2 - 1.0), _v_go_);
				_1_t_2_xxx_v_go_ = add_v3_v3(_1_t_2_t_1_v_, _t_2_1_v_go_);
				_t_go_xxx_v_go_ = scl_v3((0.5*_t_go), _1_t_2_xxx_v_go_);
				_a_3_r_xxx_r_go_ = add_v3_3(_a_3_r_, _t_go_xxx_v_go_, _r_go_);
				_r_p_ = scl_v3((1.0/_a_4), _a_3_r_xxx_r_go_);

				// ## V_p_ = T_2/T_1*V_+A_1*R_+A_2*R_p_+T_2*V_GO_ [m/s] predicted velocity <ref1 p575 eq62>
				_t_2_t_1_v_ = scl_v3((_t_2/_t_1), v_);
				_a_1_r_ = scl_v3(_a_1, r_);
				_a_2_r_p_ = scl_v3(_a_2, _r_p_);
				_t_2_v_go_ = scl_v3(_t_2, _v_go_);
				_v_p_ = add_v3_4(_t_2_t_1_v_, _a_1_r_, _a_2_r_p_, _t_2_v_go_);
				
				_norm_r_p = norm_v3(_r_p_);
				_abs_norm_r_p_r_p = fabs(_norm_r_p - _r_p);
				if(gtp.eps_r >= _abs_norm_r_p_r_p) {
					break;
				}
			}
			if (gtp.i_pred_max <= i_pred) {
				return GUITRNS_EXIT_FAILURE_PEG_PRED_MAX;
			}


		}
		_skip = 0; // set to 0 after first loop


		// # corrector <ref1 p577>
		// ## U_x_ = uvec(R_p_ - dot(U_y_,R_p_)*U_y_) [unitless] remove U_y_ component from R_p_ and scale to length 1 <ref1 p576 eq63>
		_u_y_r_p = dot_v3_v3(_u_y_, _r_p_);
		_u_y_r_p_u_y_ = scl_v3(_u_y_r_p, _u_y_);
		_r_p_u_y_r_p_u_y_ = sub_v3_v3(_r_p_, _u_y_r_p_u_y_);
		_u_x_ = nmlz_v3(_r_p_u_y_r_p_u_y_);

		// ## R_D_ = R_D*U_x_ [m] <ref1 p576 eq63>
		_r_d_ = scl_v3(gtp.r_d, _u_x_);

		// ## U_z_ = cross(U_x_,U_y_) [unitless] unit vector down range <ref1 p576 eq64>
		_u_z_ = crs_v3_v3(_u_x_, _u_y_);

		// ## V_D_ = V_D*(U_x_*sind(gamma_D) + U_z_*cosd(gamma_D)) [m/s] desired velocity vector <ref1 p576 eq64>
		_u_x_sin_gmm_d_ = scl_v3(sin(gtp.gmm_d), _u_x_);
		_u_z_cos_gmm_d_ = scl_v3(cos(gtp.gmm_d), _u_z_);
		_u_x_sin_gmm_d_u_z_cos_gmm_d_ = add_v3_v3(_u_x_sin_gmm_d_, _u_z_cos_gmm_d_);
		_v_d_ = scl_v3(gtp.v_d, _u_x_sin_gmm_d_u_z_cos_gmm_d_);

		// ## V_miss_ = V_D_ - V_p_ [m/s] V_D_ error term <ref1 p576 eq65 (note: reference contains typo)>
		_v_miss_ = sub_v3_v3(_v_d_, _v_p_);

		// ## D_V_ = D_V_ + V_miss_ [m/s] correct D_V_ <ref1 p576 eq65>
		_d_v_ = add_v3_v3(_d_v_, _v_miss_);

		// ## lambda_V_ = uvec(D_V_) [unitless] <ref1 p576>
		_lmd_v_ = nmlz_v3(_d_v_);

		// ## F_V = (dot(lambda_V_,V_D_-V_)/dot(lambda_,V_p_-V_))-1 [unitless] <ref1 p576 eq66>
		_v_d_v_ = sub_v3_v3(_v_d_, v_);
		_lmd_v_v_d_v = dot_v3_v3(_lmd_v_, _v_d_v_);
		_v_p_v_ = sub_v3_v3(_v_p_, v_);
		_lmd_v_p_v = dot_v3_v3(_lmd_, _v_p_v_);
		_f_v = (_lmd_v_v_d_v/_lmd_v_p_v) - 1.0;

		if (fabs(_f_v) < gtp.eps) {
			// ## D_V_ = D_V*(1+F_V)*lambda_V_ [m/s] <ref1 p576 eq 65-a>
			_d_v_ = scl_v3((_d_v*(1.0 + _f_v)), _lmd_v_);
		}

		// ## S_epsilon = -D*(U_y_(3)+cosd(i_D))*U_z_(3)/(1-U_x_(3)^2) [unitless] <ref1 p577>
		_1_u_x_z_square = 1.0 - _u_x_.z*_u_x_.z;
		_s_eps = -gtp.d_damp*(_u_y_.z + cos(gtp.i_d))*_u_z_.z/_1_u_x_z_square;

		// ## U_y_ = U_y_*sqrt(1-S_epsilon^2)+S_epsilon*U_z_ [unitless] <ref1 p577>
		_u_y_sqrt_1_s_eps_square_ = scl_v3(sqrt(1.0 - _s_eps*_s_eps), _u_y_);
		_s_eps_u_z_ = scl_v3(_s_eps, _u_z_);
		_u_y_ = add_v3_v3(_u_y_sqrt_1_s_eps_square_, _s_eps_u_z_);


		// # convergence test <ref1 p576>
		// ## norm(V_miss_) <= epsilon_V*norm(D_V_)
		_v_miss = norm_v3(_v_miss_);
		_eps_v_d_v = gtp.eps_v*_d_v;
		if ((gtp.i_corr_min < i_corr) && (_eps_v_d_v >= _v_miss)) {
			break;
		}
	}


	// # warnings
	if (init && (_i_corr_max <= i_corr)) {
		return GUITRNS_EXIT_FAILURE_PEG_CORR_MAX;;
	}
	if (gtp.t_go_min > _t_go) {
		return GUITRNS_EXIT_FAILURE_PEG_T_GO_MIN;
	}


	// # compute steering command
	// ## K = J/L [s] <ref1 p570 eq24>
	_k = _j/_l;

	// ## lambda_ = uvec(V_GO_) [unitless] <ref1 p570 eq25>
	_lmd_ = nmlz_v3(_v_go_);

	// ## S_T = dot(lambda_,R_GO_) [m] <ref1 p570>
	_s_t = dot_v3_v3(_lmd_, _r_go_);

	// ## R_GO_ = R_GO_ + (S_T - dot(lambda_,R_GO_))*lambda_ [m] <ref1 p570 eq26>
	_lmd_r_go = dot_v3_v3(_lmd_, _r_go_);
	_s_t_lmd_r_go_lmd_ = scl_v3((_s_t - _lmd_r_go), _lmd_);
	_r_go_ = add_v3_v3(_r_go_, _s_t_lmd_r_go_lmd_);

	// ## lambda_dot_ = (R_GO_ - S_T*lambda_)/Q_T; [s^-1] <ref1 p570 eq27>
	_s_t_lmd_ = scl_v3(_s_t, _lmd_);
	_r_go_s_t_lmd_ = sub_v3_v3(_r_go_, _s_t_lmd_);
	_lmd_dot_ = scl_v3((1.0/_q_t), _r_go_s_t_lmd_);

	// ## lambda_dot = norm(lambda_dot_) [s^-1]  store magnitude of lambda_dot_ vector
	_lmd_dot = norm_v3(_lmd_dot_);


	// # pack state
	pgts->t_go = _t_go;
	pgts->k_time = _k;
	pgts->d_v_ = _d_v_;
	pgts->r_p = _r_p;
	pgts->r_p_ = _r_p_;
	pgts->v_p_ = _v_p_;
	pgts->r_d_ = _r_d_;
	pgts->v_d_ = _v_d_;
	pgts->u_y_  = _u_y_;
	pgts->lmd_ = _lmd_;
	pgts->lmd_dot_ = _lmd_dot_;
	pgts->lmd_dot = _lmd_dot;

	return EXIT_SUCCESS;
}

/// @brief 並進誘導 : 更新 : PEGコマンドON
/// @param [in,out] pgts pointer to guitrns state
void guitrns_update_peg_cmd_on(guitrns_state_t* pgts) {
	// # update time
	pgts->t = pgts->t + gtp.dt;
	pgts->t_c = pgts->t_c + gtp.dt;

	// # update mass
	pgts->mass = gtp.mass_0 + gtp.mass_dot*pgts->t;

	// # calc acceleration vector
	// ## Pfun=@(t)lambda_.*cos(lambda_dot.*(t-K))+lambda_dot_/lambda_dot.*sin(lambda_dot.*(t-K));% <ref1 p569 eq15>
	vec3_t _lmd_xxx_k_ = scl_v3((cos(pgts->lmd_dot*(pgts->t_c - pgts->k_time))), pgts->lmd_);
	vec3_t _lmd_dot_lmd_dot_ = scl_v3((1.0/pgts->lmd_dot), pgts->lmd_dot_);
	vec3_t _lmd_dot_xxx_k_ = scl_v3(sin(pgts->lmd_dot*(pgts->t_c - pgts->k_time)), _lmd_dot_lmd_dot_);
	pgts->p_ = add_v3_v3(_lmd_xxx_k_, _lmd_dot_xxx_k_);

	// ## F_T_=F_T*P_(t_c);% [N] thrust vector
	pgts->f_thrust_ = scl_v3(gtp.f_thrust, pgts->p_);

	// ## a_T_=F_T_/m(t);% [m/s^2] acceleration due to thrust
	pgts->a_thrust_ = scl_v3((1.0/pgts->mass), pgts->f_thrust_);

	// # update dlt_v_thrust
	// ## Deltav_T_=Deltav_T_+a_T_*dt_c;% accumulate change in speed due to thrust
	vec3_t _a_thrust_dt_ = scl_v3(gtp.dt, pgts->a_thrust_);
	pgts->dlt_v_thrust_ = add_v3_v3(pgts->dlt_v_thrust_, _a_thrust_dt_);
}


/// @brief 並進誘導 : 更新 : PEGコマンドOFF
/// @param [in,out] pgts pointer to guitrns state
void guitrns_update_peg_cmd_off(guitrns_state_t* pgts) {
	pgts->a_thrust_ = zeros_v3();
}

