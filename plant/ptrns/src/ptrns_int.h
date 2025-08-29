/// @file ptrns_int.h
/// @brief internals of plant translation


#ifndef __PTRNS_INT_H__
#define __PTRNS_INT_H__


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
// ## param
typedef struct {
	double isp;
} ptrns_param_t;

// ## state
typedef struct {
    int32_t mode;
	vec3_t pos_;
	vec3_t vel_;
	vec3_t acl_;
} ptrns_state_t;


// # enumタグ定義（typedefを同時に行う）
// none


// # struct/unionタグ定義（typedefを同時に行う）
// none


// # extern変数宣言
extern ptrns_param_t ptp;
extern ptrns_state_t pts;


// # 関数プロトタイプ宣言
// none


// # インライン関数
// none


#endif

