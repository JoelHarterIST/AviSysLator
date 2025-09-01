/// @file main.c
/// @brief main functions of avisyslator_gnc


// # システムヘッダの取り込み
// none


// # ユーザ作成ヘッダの取り込み
// ## 共通ヘッダ
#include "common.h"

// ## 他モジュールヘッダ
#include "guitrns.h"
#include "ptrns.h"
#include "userif.h"

// ## 自モジュールヘッダ
#include "main_int.h"


// # 自ファイル内でのみ使用する#defineマクロ
// none


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
// ## param
main_param_t mp = {
	.num_of_loop = 0
};

// ## state
main_state_t ms = {
	.sim_slot = 0,
	.sim_time = 0.0
};

// ## framework timers
uint64_t nav_time = 0; // [us]
int64_t mission_time = MISSION_TIME_MIN_US; // [us]
double nav_time_s = 0.0; // [s]
double mission_time_s = MISSION_TIME_MIN_S; // [s]


// # 関数定義
// ## 内部関数プロトタイプ宣言
void update_time_slot(void);

// ## 外部公開関数定義
/// @brief avisyslator main function
int main(int argc, char* argv[]) {
	// # declaration
	int i = 0;
	int _ret = 0;

	enum {
		ARG_PROG_NAME,
		ARG_PATH_IN,
		ARG__COUNT,
	};

	// # handle args
	if (argc != ARG__COUNT) {
		fprintf(stderr, "Usage:\n");
		fprintf(stderr, "  %s <INPUT>\n", argv[ARG_PROG_NAME]);
		return EXIT_FAILURE;
	}


	// # initialize
	// ## user interface init
	_ret = userif_init(ms.sim_time, argv[ARG_PATH_IN]);
	if (EXIT_SUCCESS != _ret) return EXIT_FAILURE;

	// ## framework timer update after userif_init
	nav_time = (uint64_t)(nav_time_s * 1e6); // convert to microseconds
	mission_time = (int64_t)(mission_time_s * 1e6); // convert to microseconds


	// # simulation loop
	userif_main_loop_start(ms.sim_time);
	for (i=0; i < mp.num_of_loop; i++) {
		// ## sensor
		// none

		// ## sensor interface
		// none

		// ## navigation
		// none

		// ## guidance
		guitrns_main(ms.sim_slot);

		// ## control
		// none

		// ## actuator interface
		// none

		// ## sequence
		// none

		// ## actuator
		// none

		// ## plant
		ptrns_main(ms.sim_slot);

		// ## update time and slot
		update_time_slot();

		// ## user interface (log)
		userif_main(i, ms.sim_time);
	}
	userif_main_loop_end();


	// # finalize
	// ## user interface (log) finish
	userif_finish();

	return EXIT_SUCCESS;
}

// ## 内部関数定義
/// @brief 時間とスロットの更新
void update_time_slot(void) {
	// # update simulation time
	// ## update sim_time
	ms.sim_time += DT_OF_SLOT_S;

	// ## update sim_slot
	ms.sim_slot++;
	if (NUM_OF_SLOT <= ms.sim_slot) {
		ms.sim_slot = 0;
	}

	// # update framework time
	// ## update nav_time, nav_time_s
	if (NAV_TIME_MAX_US > nav_time) {
		nav_time += DT_OF_SLOT_US;
	}
	nav_time_s = (double)nav_time*1e-6; // convert to seconds

	// ## update mission_time, mission_time_s
	if ((MISSION_TIME_MIN_US < mission_time) && (mission_time < MISSION_TIME_MAX_US)) {
		mission_time += DT_OF_SLOT_US;
	}
	mission_time_s = (double)mission_time*1e-6; // convert to seconds

}

