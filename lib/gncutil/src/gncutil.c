/// @file gncutil.c
/// @brief This file contains the utility functions for GNC. 
/// @note 命名規約として下記の例外規約を適用する(計算式の途中で簡潔に記述するため)。
///       - 変換関数は「(変換前)_to_(変換後)_(補足説明)」とする。
///       - 一般的な汎用ライブラリで使用されており、一般的に認識されている関数名称があればそれを使用する。
///         - lobyte, hibyte, makeword は「minwindef.h」から流用


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "gncutil.h"
#include "gncmath.h"


/// @brief      物理値生値変換
/// @param phy  物理量
/// @param lsb  LSB値
/// @note       物理量をLSB値で割った値を変換する
/// @return     生値
int16_t phy_to_raw_i16(double phy, double lsb) 
{
    int16_t v = 0;
    // convert double to uint32_t
    v = (int16_t)(phy * (1/lsb));

    return v;
}

/// @brief      物理値生値変換
/// @param phy  生値
/// @param lsb  LSB値
/// @note       生値をLSB値で掛けた値を変換する
/// @return     物理値
double raw_to_phy_i16(int16_t raw, double lsb) 
{
    double v = 0;
    // convert double to uint32_t
    v = (double)(raw * lsb);

    return v;
}

/// @brief    下位8ビットを取得
/// @param    16ビットの値 
/// @return   下位8ビット
uint8_t lobyte(uint16_t value) 
{
    return (uint8_t)(value & 0xFF); // 下位8ビットをマスク
}

/// @brief      上位8ビットを取得
/// @param      16ビットの値 
/// @return     上位8ビット
uint8_t hibyte(uint16_t value) 
{
    return (uint8_t)((value >> 8) & 0xFF); // 上位8ビットをマスク
}


/// @brief      2バイトの値を作成
/// @param lowByte  下位バイト
/// @param highByte 上位バイト
/// @return     2バイトの値
uint16_t makeword(uint8_t lowByte, uint8_t highByte) 
{
    return (uint16_t)(lowByte | (highByte << 8));
}

