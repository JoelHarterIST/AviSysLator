/// @file mdlutil.c
/// @brief This file contains the utility functions for model. 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "mdlutil.h"



/// @brief  update index
/// @param [in] idx   ring buffer index          
/// @param [in] size  ring buffer size
/// @return       updated index
uint32_t update_index(uint32_t idx, uint32_t size) 
{
    if (idx >= size) {
        return 0;
    }

    if (size <= 1) {
        return 0;
    }
    idx = (idx + 1) % size;
    return idx;
}

///  @brief ディレイ処理：int32型
///   
///  ディレイ処理をする。バッファはリングバッファでFIFO。
///  バッファのインデックスの更新は本関数で実施する。
///   
///  @param array delayの情報を格納する配列のアドレス
///  @param size delayの情報を格納する配列の数
///  @param ind 更新する配列のインデックス
///  @param src_adr 反映元のアドレス
///  @param dst_adr 反映先のアドレス
uint32_t mdlutil_delay_i32(int32_t* array, uint32_t size, uint32_t ind, int32_t* src_adr, int32_t* dst_adr)
{
    uint32_t i = 0;
 	int32_t _tmp = 0;

    // 無効なアドレスは処理をしない。
    if (array == NULL || src_adr == NULL || dst_adr == NULL) {
        printf("error: array or adr is NULL\n");
        return ind;
    }
    if (src_adr == dst_adr) {
        // printf("error: src_adr and dst_adr are same\n");
        return ind;
    }

    // サイズが０の場合はそのままコピーする。
    if (size <= 1) {
        *dst_adr = *src_adr;
        return ind;
    }
 
    // 無効なインデックスは処理をしない。
    if (size <= ind) {
        // printf("error: size is %d, ind is %d\n", size, ind);
        return ind;
    }

    _tmp = *src_adr;
    *dst_adr = array[ind];
    array[ind] = _tmp;
    ind = update_index(ind, size); 

    return ind;
}

///  @brief ディレイ処理：double型
///   
///  ディレイ処理をする。バッファはリングバッファでFIFO。
///  バッファのインデックスの更新は本関数で実施する。
///   
///  @param array delayの情報を格納する配列のアドレス
///  @param size delayの情報を格納する配列の数
///  @param ind 更新する配列のインデックス
///  @param src_adr 反映元のアドレス
///  @param dst_adr 反映先のアドレス
uint32_t mdlutil_delay_dbl(double* array, uint32_t size, uint32_t ind, double* src_adr, double* dst_adr)
{
    uint32_t i = 0;
 	double _tmp = 0;

    // 無効なアドレスは処理をしない。
    if (array == NULL || src_adr == NULL || dst_adr == NULL) {
        printf("error: array or adr is NULL\n");
        return ind;
    }
    if (src_adr == dst_adr) {
        // printf("error: src_adr and dst_adr are same\n");
        return ind;
    }

    // サイズが０の場合はそのままコピーする。
    if (size <= 1) {
        *dst_adr = *src_adr;
        return ind;
    }
 
    // 無効なインデックスは処理をしない。
    if (size <= ind) {
        // printf("error: size is %d, ind is %d\n", size, ind);
        return ind;
    }

    _tmp = *src_adr;
    *dst_adr = array[ind];
    array[ind] = _tmp;
    ind = update_index(ind, size); 

    return ind;
}

///  @brief ディレイ処理：vec3型
///   
///  ディレイ処理をする。バッファはリングバッファでFIFO。
///  バッファのインデックスの更新は本関数で実施する。
///   
///  @param array delayの情報を格納する配列のアドレス
///  @param size delayの情報を格納する配列の数
///  @param ind 更新する配列のインデックス
///  @param src_adr 反映元のアドレス
///  @param dst_adr 反映先のアドレス
uint32_t mdlutil_delay_v3(vec3_t* array, uint32_t size, uint32_t ind, vec3_t* src_adr, vec3_t* dst_adr)
{
    uint32_t i = 0;
 	vec3_t _tmp = {0.0, 0.0, 0.0};

    // 無効なアドレスは処理をしない。
    if (array == NULL || src_adr == NULL || dst_adr == NULL) {
        printf("error: array or adr is NULL\n");
        return ind;
    }
    if (src_adr == dst_adr) {
        // printf("error: src_adr and dst_adr are same\n");
        return ind;
    }

    // サイズが０の場合はそのままコピーする。
    if (size <= 1) {
        *dst_adr = *src_adr;
        return ind;
    }
 
    // 無効なインデックスは処理をしない。
    if (size <= ind) {
        // printf("error: size is %d, ind is %d\n", size, ind);
        return ind;
    }

    _tmp = *src_adr;
    *dst_adr = array[ind];
    array[ind] = _tmp;
    ind = update_index(ind, size); 

    return ind;
}
