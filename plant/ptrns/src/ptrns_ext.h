/// @file ptrns_ext.h
/// @brief externals of plant translation


#ifndef __PTRNS_EXT_H__
#define __PTRNS_EXT_H__


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
#define PTRNS_MODE_OFF (0)
#define PTRNS_MODE_INIT (1)
#define PTRNS_MODE_ON (2)


// # #define関数マクロ
// none


// # typedef定義（intやcharといった基本型に対する型定義）
// ## input
typedef struct {
	int32_t mode;
	vec3_t acl_;
} ptrns_input_t;

// ## output
typedef struct {
	int32_t mode;
	vec3_t pos_;
	vec3_t vel_;
} ptrns_output_t;


// # enumタグ定義（typedefを同時に行う）
// none


// # struct/unionタグ定義（typedefを同時に行う）
// none


// # extern変数宣言
extern ptrns_input_t pti;
extern ptrns_output_t pto;


// # 関数プロトタイプ宣言
// none


// # インライン関数
// none


#endif

