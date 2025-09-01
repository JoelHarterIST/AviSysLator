/// @file main_int.h
/// @brief internals of main


#ifndef __MAIN_INT_H__
#define __MAIN_INT_H__


// # システムヘッダの取り込み
// none


// # ユーザ作成ヘッダの取り込み
// ## 共通ヘッダ
#include "common.h"

// ## 他モジュールヘッダ
// none

// ## 自モジュールヘッダ
// none


// # #defineマクロ
// none


// # #define関数マクロ
// none


// # typedef定義（intやcharといった基本型に対する型定義）
// ## param
typedef struct {
	int32_t num_of_loop;
} main_param_t;

// ## state
typedef struct {
	int32_t sim_slot;
	double sim_time;
} main_state_t;


// # enumタグ定義（typedefを同時に行う）
// none


// # struct/unionタグ定義（typedefを同時に行う）
// none


// # extern変数宣言
extern main_param_t mp;
extern main_state_t ms;
extern double nav_time_s; // [s]
extern double mission_time_s; // [s]


// # 関数プロトタイプ宣言
// none


// # インライン関数
// none


#endif

