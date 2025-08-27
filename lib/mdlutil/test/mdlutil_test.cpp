/// @file mdlutil_test.cpp
/// @brief This file contains the test functions for mdlutil.c
#include <gtest/gtest.h>
#include "mdlutil.h"
#include "gncmath.h"


/// @brief ディレイのテストケース
#define BUF_OFS_SIZE (32UL)
#define DELAY_OFS_SIZE (16UL)

// gyro.cで定義されているローカル関数
extern uint32_t update_index(uint32_t idx, uint32_t size); 

/// @brief ディレイテストケース(異常系)
TEST(mdlutil_delay_dbl_test, Error) {
    uint32_t delay_idx = 0;
    double delay_buf[BUF_OFS_SIZE] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0,
                                       1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0,
                                       2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0,
                                       3.1, 3.2};
    double input = 0.1, output = 0.0;

    // ディレイバッファがNULLの場合
    delay_idx = mdlutil_delay_dbl(NULL, BUF_OFS_SIZE, 15, &input, &output);
    EXPECT_FLOAT_EQ(delay_idx, 15);
    delay_idx = 0;

    // 入力データのアドレスがNULLの場合
    delay_idx = mdlutil_delay_dbl(delay_buf, BUF_OFS_SIZE, 15, NULL, &output);
    EXPECT_FLOAT_EQ(delay_idx, 15);
    delay_idx = 0;

    // 出力データのアドレスがNULLの場合
    delay_idx = mdlutil_delay_dbl(delay_buf, BUF_OFS_SIZE, 15, &input, NULL);
    EXPECT_FLOAT_EQ(delay_idx, 15);
    delay_idx = 0;

    // 入力データと出力データのアドレスが同じ場合
    delay_idx = mdlutil_delay_dbl(delay_buf, BUF_OFS_SIZE, 15, &input, &input);
    EXPECT_FLOAT_EQ(delay_idx, 15);
    delay_idx = 0;

    // ディレイバッファのサイズより、インデックスが大きい場合
    delay_idx = mdlutil_delay_dbl(delay_buf, BUF_OFS_SIZE, 32, &input, &output);
    EXPECT_FLOAT_EQ(delay_idx, 32);
    delay_idx = 0;
}

/// @brief ディレイテストケース(異常系)
TEST(mdlutil_delay_i32_test, Error) {
    uint32_t delay_idx = 0;
    int32_t delay_buf[BUF_OFS_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                                       11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                                       21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                                       31, 32};
    int32_t input = 0.1, output = 0.0;

    // ディレイバッファがNULLの場合
    delay_idx = mdlutil_delay_i32(NULL, BUF_OFS_SIZE, 15, &input, &output);
    EXPECT_FLOAT_EQ(delay_idx, 15);
    delay_idx = 0;

    // 入力データのアドレスがNULLの場合
    delay_idx = mdlutil_delay_i32(delay_buf, BUF_OFS_SIZE, 15, NULL, &output);
    EXPECT_FLOAT_EQ(delay_idx, 15);
    delay_idx = 0;

    // 出力データのアドレスがNULLの場合
    delay_idx = mdlutil_delay_i32(delay_buf, BUF_OFS_SIZE, 15, &input, NULL);
    EXPECT_FLOAT_EQ(delay_idx, 15);
    delay_idx = 0;

    // 入力データと出力データのアドレスが同じ場合
    delay_idx = mdlutil_delay_i32(delay_buf, BUF_OFS_SIZE, 15, &input, &input);
    EXPECT_FLOAT_EQ(delay_idx, 15);
    delay_idx = 0;

    // ディレイバッファのサイズより、インデックスが大きい場合
    delay_idx = mdlutil_delay_i32(delay_buf, BUF_OFS_SIZE, 32, &input, &output);
    EXPECT_FLOAT_EQ(delay_idx, 32);
    delay_idx = 0;
}

/// @brief ディレイテストケース(異常系)
TEST(mdlutil_delay_vec_test, Error) {
    uint32_t delay_idx = 0;
    vec3_t delay_buf[BUF_OFS_SIZE] = {0};
    vec3_t input = {0}, output = {};

    // ディレイバッファがNULLの場合
    delay_idx = mdlutil_delay_v3(NULL, BUF_OFS_SIZE, 15, &input, &output);
    EXPECT_FLOAT_EQ(delay_idx, 15);
    delay_idx = 0;

    // 入力データのアドレスがNULLの場合
    delay_idx = mdlutil_delay_v3(delay_buf, BUF_OFS_SIZE, 15, NULL, &output);
    EXPECT_FLOAT_EQ(delay_idx, 15);
    delay_idx = 0;

    // 出力データのアドレスがNULLの場合
    delay_idx = mdlutil_delay_v3(delay_buf, BUF_OFS_SIZE, 15, &input, NULL);
    EXPECT_FLOAT_EQ(delay_idx, 15);
    delay_idx = 0;

    // 入力データと出力データのアドレスが同じ場合
    delay_idx = mdlutil_delay_v3(delay_buf, BUF_OFS_SIZE, 15, &input, &input);
    EXPECT_FLOAT_EQ(delay_idx, 15);
    delay_idx = 0;

    // ディレイバッファのサイズより、インデックスが大きい場合
    delay_idx = mdlutil_delay_v3(delay_buf, BUF_OFS_SIZE, 32, &input, &output);
    EXPECT_FLOAT_EQ(delay_idx, 32);
    delay_idx = 0;
}


/// @brief ディレイテストケース（正常系：ディレイあり、型double）
TEST(mdlutil_delay_dbl_test, NormalEnanle) {
    uint32_t delay_idx = 0, w_idx = 0, r_idx = 0;
    double delay_buf[BUF_OFS_SIZE] = {0};
    double exp_array[BUF_OFS_SIZE] = {0};
    double input = -1, output = 0.0;

    for (int i = 0; i < 100; i++) {
        input += 0.1;
        // printf("input = %f\n", input);
        delay_idx = mdlutil_delay_dbl(delay_buf, DELAY_OFS_SIZE, delay_idx, &input, &output);

        exp_array[w_idx] = input;
        w_idx = (w_idx + 1) % BUF_OFS_SIZE;
        if (i < DELAY_OFS_SIZE) {
            EXPECT_FLOAT_EQ(output, 0.0);
        }
        else {
            EXPECT_FLOAT_EQ(output, exp_array[r_idx]);
            r_idx = (r_idx + 1) % BUF_OFS_SIZE;
        }
    }
}

/// @brief ディレイテストケース（正常系：ディレイあり、型int32）
TEST(mdlutil_delay_i32_test, NormalEnable) {
    uint32_t delay_idx = 0, w_idx = 0, r_idx = 0;
    int32_t delay_buf[BUF_OFS_SIZE] = {0};
    int32_t exp_array[BUF_OFS_SIZE] = {0};
    int32_t input = -10, output = 0.0;

    for (int i = 0; i < 100; i++) {
        input += 1;
        // printf("input = %d\n", input);
        delay_idx = mdlutil_delay_i32(delay_buf, DELAY_OFS_SIZE, delay_idx, &input, &output);

        exp_array[w_idx] = input;
        w_idx = (w_idx + 1) % BUF_OFS_SIZE;
        if (i < DELAY_OFS_SIZE) {
            EXPECT_EQ(output, 0.0);
        }
        else {
            EXPECT_EQ(output, exp_array[r_idx]);
            r_idx = (r_idx + 1) % BUF_OFS_SIZE;
        }
    }
}

/// @brief ディレイテストケース（正常系：ディレイあり、型vector）
TEST(mdlutil_delay_vec_test, NormalEnable) {
    uint32_t delay_idx = 0, w_idx = 0, r_idx = 0;
    vec3_t delay_buf[BUF_OFS_SIZE] = {0};
    vec3_t exp_array[BUF_OFS_SIZE] = {0};
    vec3_t input = {-1, -2, -3}, output = {0};

    for (int i = 0; i < 100; i++) {
        input.x += 0.1;
        input.y += 0.1;
        input.z += 0.1;
        // printf("input = %d\n", input);
        delay_idx = mdlutil_delay_v3(delay_buf, DELAY_OFS_SIZE, delay_idx, &input, &output);

        exp_array[w_idx] = input;
        w_idx = (w_idx + 1) % BUF_OFS_SIZE;
        if (i < DELAY_OFS_SIZE) {
            EXPECT_FLOAT_EQ(output.x, 0.0);
            EXPECT_FLOAT_EQ(output.y, 0.0);
            EXPECT_FLOAT_EQ(output.z, 0.0);
        }
        else {
            EXPECT_FLOAT_EQ(output.x, exp_array[r_idx].x);
            EXPECT_FLOAT_EQ(output.y, exp_array[r_idx].y);
            EXPECT_FLOAT_EQ(output.z, exp_array[r_idx].z);
            r_idx = (r_idx + 1) % BUF_OFS_SIZE;
        }
    }
}

/// @brief ディレイテストケース（正常系：ディレイなし、型double）
TEST(mdlutil_delay_dbl_test, NormalDisable) {
    uint32_t delay_idx = 0, w_idx = 0, r_idx = 0;
    double delay_buf[BUF_OFS_SIZE] = {0};
    double exp_array[BUF_OFS_SIZE] = {0};
    double input = -1, output = 0.0;

    for (int i = 0; i < 100; i++) {
        input += 0.1;
        // printf("input = %f\n", input);
        delay_idx = mdlutil_delay_dbl(delay_buf, 1, delay_idx, &input, &output);

        exp_array[w_idx] = input;
        w_idx = (w_idx + 1) % BUF_OFS_SIZE;
        EXPECT_FLOAT_EQ(output, exp_array[r_idx]);
        r_idx = (r_idx + 1) % BUF_OFS_SIZE;
    }
}

/// @brief ディレイテストケース（正常系：ディレイなし、型int32）
TEST(mdlutil_delay_i32_test, NormalDisable) {
    uint32_t delay_idx = 0, w_idx = 0, r_idx = 0;
    int32_t delay_buf[BUF_OFS_SIZE] = {0};
    int32_t exp_array[BUF_OFS_SIZE] = {0};
    int32_t input = -10, output = 0.0;

    for (int i = 0; i < 100; i++) {
        input += 1;
        // printf("input = %d\n", input);
        delay_idx = mdlutil_delay_i32(delay_buf, 1, delay_idx, &input, &output);

        exp_array[w_idx] = input;
        w_idx = (w_idx + 1) % BUF_OFS_SIZE;

        EXPECT_EQ(output, exp_array[r_idx]);
        r_idx = (r_idx + 1) % BUF_OFS_SIZE;
    }
}

/// @brief ディレイテストケース（正常系：ディレイなし、型vector）
TEST(mdlutil_delay_vec_test, NormalDisable) {
    uint32_t delay_idx = 0, w_idx = 0, r_idx = 0;
    vec3_t delay_buf[BUF_OFS_SIZE] = {0};
    vec3_t exp_array[BUF_OFS_SIZE] = {0};
    vec3_t input = {-1, -2, -3}, output = {0};

    for (int i = 0; i < 100; i++) {
        input.x += 0.1;
        input.y += 0.1;
        input.z += 0.1;
        // printf("input = %d\n", input);
        delay_idx = mdlutil_delay_v3(delay_buf, 1, delay_idx, &input, &output);

        exp_array[w_idx] = input;
        w_idx = (w_idx + 1) % BUF_OFS_SIZE;
        EXPECT_FLOAT_EQ(output.x, exp_array[r_idx].x);
        EXPECT_FLOAT_EQ(output.y, exp_array[r_idx].y);
        EXPECT_FLOAT_EQ(output.z, exp_array[r_idx].z);
        r_idx = (r_idx + 1) % BUF_OFS_SIZE;
    }
}

/// @brief  インデックス更新テストケース（正常系）
TEST(update_index_test, Normal) {

    EXPECT_EQ(update_index(0, 1), 0);
    EXPECT_EQ(update_index(1, 1), 0);

    EXPECT_EQ(update_index(0, 2), 1);
    EXPECT_EQ(update_index(1, 2), 0);

    EXPECT_EQ(update_index(0, 16), 1);
    EXPECT_EQ(update_index(1, 16), 2);
    EXPECT_EQ(update_index(2, 16), 3);
    EXPECT_EQ(update_index(3, 16), 4);
    EXPECT_EQ(update_index(4, 16), 5);
    EXPECT_EQ(update_index(5, 16), 6);
    EXPECT_EQ(update_index(6, 16), 7);
    EXPECT_EQ(update_index(7, 16), 8);
    EXPECT_EQ(update_index(8, 16), 9);
    EXPECT_EQ(update_index(9, 16), 10);
    EXPECT_EQ(update_index(10, 16), 11);
    EXPECT_EQ(update_index(11, 16), 12);
    EXPECT_EQ(update_index(12, 16), 13);
    EXPECT_EQ(update_index(13, 16), 14);
    EXPECT_EQ(update_index(14, 16), 15);
    EXPECT_EQ(update_index(15, 16), 0);
}

/// @brief  インデックス更新テストケース（異常系）
TEST(update_index_test, Error) {
    EXPECT_EQ(update_index(0, 0), 0);
    EXPECT_EQ(update_index(1, 0), 0);

    EXPECT_EQ(update_index(15, 15), 0);
    EXPECT_EQ(update_index(16, 15), 0);
}
