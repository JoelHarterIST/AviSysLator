/// @file mdlrand_test.cpp
/// @brief This file contains the test functions for mdlrand.c

#include <gtest/gtest.h>
#include "mdlrand.h"
#include "gncmath.h"

#define AVILABLE_TEST_COUNT (10000000L)

/// @brief          標準偏差を計算する関数
/// @param data     データ配列
/// @return         標準偏差
double calculate_standard_deviation(const std::vector<double>& data) {
    if (data.size() <= 1) {
        return 0.0; // データが1つ以下の場合、標準偏差は0
    }

    double sum = 0.0;
    double mean = 0.0;
    // 平均値を計算
    for (int i = 0; i < data.size(); i++) {
        sum += data[i];
    }
    mean = sum / data.size();
 
    // 分散を計算
    double variance = 0.0;
    for (const auto& value : data) {
        variance += (value - mean) * (value - mean);
    }
    variance /= data.size();

    // 標準偏差を計算
    return std::sqrt(variance);
}

/// @brief 一様乱数テストケース(正常系)
TEST(mdlrand_gen_uniform_dbl_test, Normal) {
    // 乱数の範囲を指定
    double min = 0.0;
    double max = 1.0;

    for (int i = 0; i < 100000; ++i) {
        // 乱数を生成
        double random_value = mdlrand_gen_uniform_dbl();

        // 乱数が指定した範囲内にあるか確認
        EXPECT_GE(random_value, min);
        EXPECT_LT(random_value, max);
    }
}


/// @brief 正規乱数テストスイート
class mdlrand_gen_normal_dbl_test : public ::testing::TestWithParam<std::tuple<double, double>> {
    protected:
        void SetUp() override {

        }
};

/// @brief 正規乱数テストケース(正常系)
TEST_P(mdlrand_gen_normal_dbl_test, Normal) {
    // 乱数の範囲を指定
    double min = 0.0;
    double max = 0.0;
    double mean = std::get<0>(GetParam());
    double stddev = std::get<1>(GetParam());
    unsigned long err_cnt = 0;
    std::vector<double> data;
    data.resize(AVILABLE_TEST_COUNT);

    mdlrand_init(100);

    // 3シグマの範囲を計算
    min = mean - 3 * stddev;
    max = mean + 3 * stddev;


    for (int i = 0; i < AVILABLE_TEST_COUNT; ++i) {
        // 乱数を生成
        data[i] = mdlrand_gen_normal_dbl(mean, stddev);

        // 乱数が指定した範囲内にあるか確認
        if (data[i] < min || data[i] > max) {
            err_cnt++;
        }

    }
    double ret_stddev = calculate_standard_deviation(data);
    EXPECT_NEAR(ret_stddev, stddev, 0.001);

    // 3シグマの範囲に収まっているか確認
    EXPECT_LE(err_cnt, 0.003 * AVILABLE_TEST_COUNT);

    printf("stddev = %f\n", ret_stddev);
}

/// @brief 正規乱数テストのテストパターン
INSTANTIATE_TEST_SUITE_P(
    Normal,
    mdlrand_gen_normal_dbl_test,
    ::testing::Values(
        std::make_tuple( 0.0, 1.0),
        std::make_tuple(-1.0, 1.0),
        std::make_tuple( 1.0, 1.0),
        std::make_tuple( 0.0, 0.1),
        std::make_tuple( 0.0, 0.5),
        std::make_tuple( 0.0, 2.0),
        std::make_tuple( 0.0, 3.0)
    )
);

/// @brief ベクトル乱数テストスイート
class mdlrand_gen_normal_v3_test : public ::testing::TestWithParam<std::tuple<vec3_t, vec3_t>> {
    protected:
        void SetUp() override {

        }
};

/// @brief ベクトル乱数テストケース(正常系)
TEST_P(mdlrand_gen_normal_v3_test, Normal) {
    vec3_t min_ = {0.0, 0.0, 0.0};
    vec3_t max_ = {1.0, 1.0, 1.0};
    vec3_t v_ = std::get<0>(GetParam());
    vec3_t stddev_ = std::get<1>(GetParam());
    vec3_t ret_ = {0.0, 0.0, 0.0};
    unsigned long err_cnt_x = 0;
    unsigned long err_cnt_y = 0;
    unsigned long err_cnt_z = 0;
    std::vector<double> data_x;
    std::vector<double> data_y;
    std::vector<double> data_z;

    data_x.resize(AVILABLE_TEST_COUNT);
    data_y.resize(AVILABLE_TEST_COUNT);
    data_z.resize(AVILABLE_TEST_COUNT);

    // 3シグマの範囲を計算
    min_.x = v_.x - 3 * stddev_.x;
    max_.x = v_.x + 3 * stddev_.x;
    min_.y = v_.y - 3 * stddev_.y;
    max_.y = v_.y + 3 * stddev_.y;
    min_.z = v_.z - 3 * stddev_.z;
    max_.z = v_.z + 3 * stddev_.z;


    for (int i = 0; i < AVILABLE_TEST_COUNT; ++i) {
        // 乱数を生成
        ret_ = mdlrand_gen_normal_v3(v_, stddev_);
        data_x[i] = ret_.x;
        data_y[i] = ret_.y;
        data_z[i] = ret_.z;

        // 乱数が指定した範囲内にあるか確認
        if (ret_.x < min_.x || ret_.x > max_.x) {
            err_cnt_x++;
        }
        if (ret_.y < min_.y || ret_.y > max_.y) {
            err_cnt_y++;
        }   
        if (ret_.z < min_.z || ret_.z > max_.z) {
            err_cnt_z++;
        }
    }

    // 3シグマの範囲に収まっているか確認
    EXPECT_LE(err_cnt_x, 0.003 * AVILABLE_TEST_COUNT);
    EXPECT_LE(err_cnt_y, 0.003 * AVILABLE_TEST_COUNT);
    EXPECT_LE(err_cnt_z, 0.003 * AVILABLE_TEST_COUNT);

    double ret_stddev_x = calculate_standard_deviation(data_x);
    double ret_stddev_y = calculate_standard_deviation(data_y);
    double ret_stddev_z = calculate_standard_deviation(data_z);


    EXPECT_NEAR(ret_stddev_x, stddev_.x, 0.001);
    EXPECT_NEAR(ret_stddev_y, stddev_.y, 0.001);
    EXPECT_NEAR(ret_stddev_z, stddev_.z, 0.001);
    printf("stddev_x = %f\n", ret_stddev_x);
    printf("stddev_y = %f\n", ret_stddev_y);
    printf("stddev_z = %f\n", ret_stddev_z);
}

/// @brief ベクトル乱数テストのテストパターン
INSTANTIATE_TEST_SUITE_P(
    Normal,
    mdlrand_gen_normal_v3_test,
    ::testing::Values(
        std::make_tuple(vec3_t{ 0.0, 0.0, 0.0}, vec3_t{1.0, 0.0, 0.0}), 
        std::make_tuple(vec3_t{ 0.0, 0.0, 0.0}, vec3_t{0.0, 1.0, 0.0}),
        std::make_tuple(vec3_t{ 0.0, 0.0, 0.0}, vec3_t{0.0, 0.0, 1.0}),
        std::make_tuple(vec3_t{ 0.0, 0.0, 0.0}, vec3_t{1.0, 1.0, 1.0}),
        std::make_tuple(vec3_t{ 0.0, 0.0, 0.0}, vec3_t{1.0, 2.0, 3.0}),
        std::make_tuple(vec3_t{ 1.0, 0.0, 0.0}, vec3_t{1.0, 1.0, 1.0}),
        std::make_tuple(vec3_t{ 0.0, 1.0, 0.0}, vec3_t{1.0, 1.0, 1.0}),
        std::make_tuple(vec3_t{ 0.0, 0.0, 1.0}, vec3_t{1.0, 1.0, 1.0}),
        std::make_tuple(vec3_t{-1.0, 0.0, 0.0}, vec3_t{1.0, 1.0, 1.0}),
        std::make_tuple(vec3_t{ 0.0,-1.0, 0.0}, vec3_t{1.0, 1.0, 1.0}),
        std::make_tuple(vec3_t{ 0.0, 0.0,-1.0}, vec3_t{1.0, 1.0, 1.0}),
        std::make_tuple(vec3_t{-1.0,-2.0,-3.0}, vec3_t{1.0, 1.0, 1.0}),
        std::make_tuple(vec3_t{ 1.0, 2.0, 3.0}, vec3_t{1.0, 1.0, 1.0}),
        std::make_tuple(vec3_t{ 1.0, 2.0, 3.0}, vec3_t{1.0, 2.0, 3.0})
    )
);
