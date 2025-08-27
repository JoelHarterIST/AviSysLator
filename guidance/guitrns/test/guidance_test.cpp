#include <gtest/gtest.h>
#include "userif.h"
#include "guidance.h"

// テストのユーティリティ //////////////////////////////////////////////////////////

class GuidanceTest : public ::testing::Test {
protected:
	void SetUp() override {
	}

	void TearDown() override {
	}

	void set_userif_params(char* test_case_file) {
		printf("case file : %s\n", test_case_file);
		// printf("mark variable address: %016lx\n", (ADDRESS_T)&mark_variable);
		printf("gp address: %016lx\n", (ADDRESS_T)&gp);
		//userif_init(0.0, test_case_file);
	}
};

// テスト ///////////////////////////////////////////////////////////////////////

// STOP
TEST_F(GuidanceTest, Normal) {
	int32_t i;
	int32_t _ret[2] = {0};
	guidance_state_t _gs[2];
	double _a_thrust = 0.0;
	vec3_t _r_ = zeros_v3();
	vec3_t _v_ = zeros_v3();
	vec3_t _dlt_v_thrust_  = zeros_v3();

	userif_init(0.0, "test/guidance_test_normal.yaml");
	userif_main_loop_start(0.0);
	userif_main(99,0.1);
	userif_main_loop_end();
	userif_finish();

	printf("EXIT_SUCCESS %d\n", EXIT_SUCCESS);
	printf("EXIT_FAILURE %d\n", EXIT_FAILURE);
	printf("EXIT_OK %d\n", EXIT_OK);
	printf("EXIT_NG %d\n", EXIT_NG);

	// # set guidance parameter
	// a = 6378137.0;% [m] semi-major axis <WGS-84>
	// r_D = Constants.a+230e3;  % [m] desired orbital radius
	gp.r_d = 6378137.0 + 230e3;
	// i_D = 42.2;      % [deg] desired orbital inclination
	gp.i_d = 42.2*DEG_RAD;
	// V_D = sqrt(Constants.mu/R_D);% [m/s] use default orbital speed for a circular orbit by default
	gp.v_d = sqrt(MU_E/gp.r_d);
	// s.orbitFlightPathAngle  (1,1)   double  = 0 % [deg] desired flight path angle at orbit insertion point; defaults to 0 (circular orbit)
	// gamma_D=s.orbitFlightPathAngle;
	gp.gmm_d = 0.0;
	// from loaded init file
	gp.i_sp = 344.32;
	// V_ex = g_0*I_sp;
	gp.v_ex = G_E*gp.i_sp;
	// from loaded init file
	gp.f_thrust = 141260;
	// from loaded init file
	gp.mass_0 = 12951.952;
	// m_dot = -F_T/g_0/I_sp;
	gp.mass_dot = -gp.f_thrust/G_E/gp.i_sp;
	// s.epsilon               (1,1)   double  = 5e-2 % convergence criterion for F_V in the corrector
	gp.eps = 5e-2;
	// s.epsilon_R             (1,1)   double  = 30   % [m] convergence criterion for stabilization of R_p_, approximately the same as suggested value of 100 ft <ref1 p577>
	gp.eps_r = 30;
	// s.epsilon_V             (1,1)   double  = 1e-2 % [unitless] convergence criterion for D_V <ref1 p576>
	gp.eps_v = 1e-2;
	// s.D                     (1,1)   double  = 0.5  % [unitless] damping factor for convergence of U_y_ <ref1 p577>
	gp.d_damp = 0.5;
	// s.i_corr_min            (1,1)   double  = 3    % minimum number of iterations within corrector.
	gp.i_corr_min = 2;
	// s.i_corr_max_initial    (1,1)   double  = 50   % maximum number of iterations within corrector
	gp.i_corr_max_init = 50;
	// s.i_corr_max_update     (1,1)   double  = 1    % maximum number of iterations within corrector for all calls after first call (ref1 suggests only 1 iteration is necessary)
	gp.i_corr_max_update = 1;
	// s.I_max                 (1,1)   double  = 10   % maximum number of iterations for R_p_ to converge. Source recommends I_max=5 but I have found I_max=10 to be better <ref1 p577>
	gp.i_pred_max = 10;
	// s.T_GO_min              (1,1)   double  = 4    % [s] minimum time to go; warn user if T_GO drops below this value
	gp.t_go_min = 4;


	// # set initial input
	// case,thrust,   Isp,     mass,   pos_ECI_X, pos_ECI_Y,  pos_ECI_Z,   vel_ECI_X,   vel_ECI_Y,  vel_ECI_Z
	//    0,141260,344.32,12951.952,-4020669.944,2639571.27,4372906.047,-2046.201518,-1830.379092,580.2079586
	_r_ = val_to_v3(-4020669.944,2639571.27,4372906.047);
	_v_ = val_to_v3(-2046.201518,-1830.379092,580.2079586);
	// a_T = F_T/m_0;% initialize
	gs.a_thrust = gp.f_thrust/gp.mass_0;
	gs.dlt_v_thrust_ = zeros_v3();

	// # test first call
	_ret[0] = guidance_core(1, _r_, _v_, &gs);
	_gs[0] = gs;

	// # set second input
	_r_ = val_to_v3(-4.02271734954116E+6, 2.6377356628922247E+6, 4.3734844375870349E+6);
	_v_ = val_to_v3(-2048.6122243180757, -1840.8413135134097, 576.57201001831561);
	gs.a_thrust = 10.941806051372708;
	gs.dlt_v_thrust_ = val_to_v3(-8.2456593185733027, -6.6340930734478, 2.7088771223107928);

	// # test second call
	_ret[1] = guidance_core(0, _r_, _v_, &gs);
	_gs[1] = gs;

	// # check initial result
	i = 0;
	EXPECT_EQ(EXIT_SUCCESS, _ret[i]);
	EXPECT_DOUBLE_EQ(_gs[i].d_v_.x, -2887.2982768334423);
	EXPECT_DOUBLE_EQ(_gs[i].d_v_.y, -4269.1350752792341);
	EXPECT_DOUBLE_EQ(_gs[i].d_v_.z, -50.798524962270818);
	EXPECT_DOUBLE_EQ(_gs[i].u_y_.x, -0.55308155874139386);
	EXPECT_DOUBLE_EQ(_gs[i].u_y_.y, 0.38301745396831982);
	EXPECT_DOUBLE_EQ(_gs[i].u_y_.z, -0.73986378431155553);
	EXPECT_DOUBLE_EQ(_gs[i].t_go, 242.50502737447252);
	EXPECT_DOUBLE_EQ(_gs[i].k_time, 151.01446247153797);
	EXPECT_DOUBLE_EQ(_gs[i].lmd_.x, -0.55995812582774374);
	EXPECT_DOUBLE_EQ(_gs[i].lmd_.y, -0.82845647858965688);
	EXPECT_DOUBLE_EQ(_gs[i].lmd_.z, -0.010332492550502146);
	EXPECT_DOUBLE_EQ(_gs[i].lmd_dot_.x, 0.0016220286153635006);
	EXPECT_DOUBLE_EQ(_gs[i].lmd_dot_.y, -0.001074435719754675);
	EXPECT_DOUBLE_EQ(_gs[i].lmd_dot_.z, -0.0017560981099505985);
	EXPECT_DOUBLE_EQ(_gs[i].lmd_dot, 0.0026209291323335787);
	EXPECT_DOUBLE_EQ(_gs[i].v_p_.x, -3473.8482258902968);
	EXPECT_DOUBLE_EQ(_gs[i].v_p_.y, -6887.5344685572436);
	EXPECT_DOUBLE_EQ(_gs[i].v_p_.z, -970.95974718018419);
	EXPECT_DOUBLE_EQ(_gs[i].r_p_.x, -4646336.9020856619);
	EXPECT_DOUBLE_EQ(_gs[i].r_p_.y, 1728924.7408641179);
	EXPECT_DOUBLE_EQ(_gs[i].r_p_.z, 4368387.4800136983);
	EXPECT_DOUBLE_EQ(_gs[i].r_p, 6607606.3993800366);
	EXPECT_DOUBLE_EQ(_gs[i].r_d_.x, -4646711.4283455778);
	EXPECT_DOUBLE_EQ(_gs[i].r_d_.y, 1729064.1187666946);
	EXPECT_DOUBLE_EQ(_gs[i].r_d_.z, 4368739.4961985489);
	EXPECT_DOUBLE_EQ(_gs[i].v_d_.x, -3470.6284387719861);
	EXPECT_DOUBLE_EQ(_gs[i].v_d_.y, -6880.1554185678015);
	EXPECT_DOUBLE_EQ(_gs[i].v_d_.z, -968.42097540504051);


	// # check second result
	i = 1;
	EXPECT_EQ(EXIT_SUCCESS, _ret[i]);
	EXPECT_DOUBLE_EQ(_gs[i].d_v_.x, -2949.4498472183486);
	EXPECT_DOUBLE_EQ(_gs[i].d_v_.y, -4345.4408542007577);
	EXPECT_DOUBLE_EQ(_gs[i].d_v_.z, -40.509288460398338);
	EXPECT_DOUBLE_EQ(_gs[i].u_y_.x, -0.55321881851867316);
	EXPECT_DOUBLE_EQ(_gs[i].u_y_.y, 0.38274557698545497);
	EXPECT_DOUBLE_EQ(_gs[i].u_y_.z, -0.73990185979957812);
	EXPECT_DOUBLE_EQ(_gs[i].t_go, 243.43080600667207);
	EXPECT_DOUBLE_EQ(_gs[i].k_time, 152.0591160525187);
	EXPECT_DOUBLE_EQ(_gs[i].lmd_.x, -0.56173418506512263);
	EXPECT_DOUBLE_EQ(_gs[i].lmd_.y, -0.82728142874755961);
	EXPECT_DOUBLE_EQ(_gs[i].lmd_.z, -0.0077551904308700553);
	EXPECT_DOUBLE_EQ(_gs[i].lmd_dot_.x, 0.0016228502387067992);
	EXPECT_DOUBLE_EQ(_gs[i].lmd_dot_.y, -0.001085523544429159);
	EXPECT_DOUBLE_EQ(_gs[i].lmd_dot_.z, -0.0017506968615740989);
	EXPECT_DOUBLE_EQ(_gs[i].lmd_dot, 0.0026223927554632543);
	EXPECT_DOUBLE_EQ(_gs[i].v_p_.x, -3468.9123427825448);
	EXPECT_DOUBLE_EQ(_gs[i].v_p_.y, -6880.1312541202569);
	EXPECT_DOUBLE_EQ(_gs[i].v_p_.z, -967.51549952976734);
	EXPECT_DOUBLE_EQ(_gs[i].r_p_.x, -4647162.6339121992);
	EXPECT_DOUBLE_EQ(_gs[i].r_p_.y, 1728326.3803280126);
	EXPECT_DOUBLE_EQ(_gs[i].r_p_.z, 4368695.6297938684);
	EXPECT_DOUBLE_EQ(_gs[i].r_p, 6608233.5492569199);
	EXPECT_DOUBLE_EQ(_gs[i].r_d_.x, -4647095.0190486647);
	EXPECT_DOUBLE_EQ(_gs[i].r_d_.y, 1728301.0427536459);
	EXPECT_DOUBLE_EQ(_gs[i].r_d_.z, 4368633.4248044612);
	EXPECT_DOUBLE_EQ(_gs[i].v_d_.x, -3468.5797493758905);
	EXPECT_DOUBLE_EQ(_gs[i].v_d_.y, -6881.3460475596048);
	EXPECT_DOUBLE_EQ(_gs[i].v_d_.z, -967.30069017886774);

}
