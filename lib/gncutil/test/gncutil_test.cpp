/// @file gncutil_test.cpp
/// @brief This file contains the test functions for gncutil.c
#include <gtest/gtest.h>
#include "gncutil.h"
#include "gncmath.h"


/// @brief 生値物理値変換テストケース（正常系）
TEST(raw_to_phy_i16_test, Normal) {
    EXPECT_FLOAT_EQ(raw_to_phy_i16(0x0000, 0.1), 0.0);
    EXPECT_FLOAT_EQ(raw_to_phy_i16(0x0001, 0.1), 0.1);
    EXPECT_FLOAT_EQ(raw_to_phy_i16(0x7FFF, 0.1), 3276.7);
    EXPECT_FLOAT_EQ(raw_to_phy_i16(0x8000, 0.1), -3276.8);
    EXPECT_FLOAT_EQ(raw_to_phy_i16(0xFFFF, 0.1), -0.1);
}

/// @brief 物理値生値変換テストケース（正常系）
TEST(phy_to_raw_i16_test, Normal) {
    EXPECT_EQ(phy_to_raw_i16(0.00, 0.10), (int16_t)0x0000);
    EXPECT_EQ(phy_to_raw_i16(0.10, 0.10), (int16_t)0x0001);
    EXPECT_EQ(phy_to_raw_i16(3276.7, 0.1), (int16_t)0x7FFF);
    EXPECT_EQ(phy_to_raw_i16(-3276.8, 0.1), (int16_t)0x8000);
    EXPECT_EQ(phy_to_raw_i16(-0.1, 0.1), (int16_t)0xFFFF);
}

/// @brief  int16整数値作成テストケース（正常系）
TEST(makeword_test, Normal) {
    EXPECT_EQ(makeword(0x00, 0x00), (uint16_t)0x0000);
    EXPECT_EQ(makeword(0x01, 0x00), (uint16_t)0x0001);
    EXPECT_EQ(makeword(0x00, 0x01), (uint16_t)0x0100);
    EXPECT_EQ(makeword(0x01, 0x01), (uint16_t)0x0101);
    EXPECT_EQ(makeword(0xFF, 0xFF), (uint16_t)0xFFFF);
}

/// @brief  lobyte抽出テストケース（正常系）
TEST(lobyte_test, Normal) {
    EXPECT_EQ(lobyte(0x0000), (uint8_t)0x00);   
    EXPECT_EQ(lobyte(0x0100), (uint8_t)0x00);
    EXPECT_EQ(lobyte(0x0001), (uint8_t)0x01);
    EXPECT_EQ(lobyte(0x0101), (uint8_t)0x01);
    EXPECT_EQ(lobyte(0xFFFF), (uint8_t)0xFF);
    EXPECT_EQ(lobyte(0xFF00), (uint8_t)0x00);
    EXPECT_EQ(lobyte(0x00FF), (uint8_t)0xFF);
    EXPECT_EQ(lobyte(0x1234), (uint8_t)0x34);
}

/// @brief  hibyte抽出テストケース（正常系）
TEST(hibyte_test, Normal) {
    EXPECT_EQ(hibyte(0x0000), (uint8_t)0x00);   
    EXPECT_EQ(hibyte(0x0100), (uint8_t)0x01);
    EXPECT_EQ(hibyte(0x0001), (uint8_t)0x00);
    EXPECT_EQ(hibyte(0x0101), (uint8_t)0x01);
    EXPECT_EQ(hibyte(0xFFFF), (uint8_t)0xFF);
    EXPECT_EQ(hibyte(0xFF00), (uint8_t)0xFF);
    EXPECT_EQ(hibyte(0x00FF), (uint8_t)0x00);
    EXPECT_EQ(hibyte(0x1234), (uint8_t)0x12);
}
