/// @file guitrns_ext.h
/// @brief externals of guidance translation


#ifndef __GUITRNS_EXT_H__
#define __GUITRNS_EXT_H__


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
#define GUITRNS_MODE_OFF (0)
#define GUITRNS_MODE_STAGE_1_INIT (1)
#define GUITRNS_MODE_STAGE_1 (2)
#define GUITRNS_MODE_STAGE_2_INIT (3)
#define GUITRNS_MODE_STAGE_2 (4)


// # #define関数マクロ
// none


// # typedef定義（intやcharといった基本型に対する型定義）
// ## input
typedef struct {
	int32_t mode;
	vec3_t pos_;
	vec3_t vel_;
} guitrns_input_t;

// ## output
typedef struct {
	int32_t mode;
	vec3_t a_thrust_;
} guitrns_output_t;


// # enumタグ定義（typedefを同時に行う）
// none


// # struct/unionタグ定義（typedefを同時に行う）
// none


// # extern変数宣言
extern guitrns_input_t gti;
extern guitrns_output_t gto;


// # 関数プロトタイプ宣言
// none


// # インライン関数
// none


#endif

