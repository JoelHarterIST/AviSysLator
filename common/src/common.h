/// @file common.h
/// @brief common definitions


#ifndef __COMMON_H__
#define __COMMON_H__


// # システムヘッダの取り込み
#include <stdio.h>
#include <stdint.h>
#include <string.h>


// # ユーザ作成ヘッダの取り込み
// ## 共通ヘッダ
// none

// ## 他モジュールヘッダ
// none

// ## 自モジュールヘッダ
// none


// # #defineマクロ
// ## ADDRESS TYPE
#if __SIZEOF_POINTER__ == 4
#define ADDRESS_T uint32_t
#else
#define ADDRESS_T uint64_t
#endif

// ## EXIT STATUS
#define	EXIT_SUCCESS	0	/* Successful exit status. 0 <= */
#define	EXIT_FAILURE	-1	/* Failing exit status.  0 >  */
#define	EXIT_OK	0	/* Successful exit status. 0 <= */
#define	EXIT_NG	-1	/* Failing exit status.  0 >  */

// ## SLOT TIME
#define DT_OF_SLOT_S (0.01) // [s] 0.5ms
// #define DT_OF_SLOT_S (0.0005) // [s] 0.5ms
#define DT_OF_SLOT_US (500) // [us] 500us, 0.5ms
#define NUM_OF_SLOT (20) // 10ms
#define CALL_OFF (0) // not call function 
#define CALL_ON (1) // call function 

// ## NAV TIME
#define NAV_TIME_MAX_US (1099511627775) // [us] 40bit uint max

// ## MISSION TIME
#define MISSION_TIME_MIN_US (-549755813888) // [us] 40bit int min
#define MISSION_TIME_MAX_US (549755813887) // [us] 40bit int max
#define MISSION_TIME_MIN_S (-549755.813888) // [s] 40bit int min

// ## MATH CONSTANTS
#define PI (3.14159265358979323846)
#define DEG_RAD (PI/180.0)
#define RAD_DEG (180.0/PI)

// ## NATURE CONSTANTS
#define MU_E (3.986004418E+14) // [m^3 s^-2]
#define G_E (9.80665) // [m/s^2]


// # #define関数マクロ
// none


// # typedef定義（intやcharといった基本型に対する型定義）
// none


// # enumタグ定義（typedefを同時に行う）
// none


// # struct/unionタグ定義（typedefを同時に行う）
// none


// # extern変数宣言
// extern double nav_time_s;
// extern double mission_time_s;


// # 関数プロトタイプ宣言
// none


// # インライン関数
// none


#endif

