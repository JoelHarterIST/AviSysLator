/// @file mdlrand.c
/// @brief This file contains the random number generator functions for model.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdint.h>
#include "mt19937ar.h"
#include "mdlrand.h"



typedef struct random_gen {
  int has_gauss; // 前回の計算結果の有無
  double gauss;  // 前回の計算結果
} random_gen_t;

random_gen_t g_random_gen;


//
//  @brief ボックス＝ミュラー法による正規分布乱数生成
//   
//  ボックス＝ミュラー法を使用した正規分乱数生成(https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform)
//   
//  @param mean 平均値
//  @param stddev 標準偏差
//  @return 正規分布乱数
//  
double box_muller_normal(double mean, double stddev) 
{
    double x = 0;
    double y = 0;
    double z  = 0;
    double xpi  = 0;
    double g2rad = 0;

    if (g_random_gen.has_gauss != 0)
    {
        z = g_random_gen.gauss;
        g_random_gen.has_gauss = 0;
    }
    else
    {
        x = mdlrand_gen_uniform_dbl();
        y = mdlrand_gen_uniform_dbl();
        g2rad = -2.0 * log(x);
        xpi = 2.0 * M_PI * y;
        z = sqrt(g2rad) * cos(xpi);
        g_random_gen.gauss = sqrt(g2rad) * sin(xpi);
        g_random_gen.has_gauss = 1;
    }
    // printf("stddev:%f, meain:%f. z:%f\n", stddev, mean, z);
    return z * stddev + mean;
}


//  @brief 乱数発生器の初期化
//   
//  指定されたSeedで乱数発生器の初期化する
//  @param seed 乱数発生器の初期化に使用するSeed
//  
void mdlrand_init(unsigned long seed)
{
    init_genrand(seed);
    memset(&g_random_gen, sizeof(g_random_gen), 0);
}

// 
//
//  @brief 一様分布乱数生成
//   
//  [0,0xffffffff]で乱数を返す
// 
//  @return 0<= x < 0xffffffffのランダムな整数値
//  
uint32_t mdlrand_gen_uniform_u32()
{
    uint32_t ret = 0;
    ret = (uint32_t)genrand_int32();
    return ret;
}

//
//  @brief 一様分布乱数生成
//   
//  [0,1)で乱数を返す
// 
//  @return 0<= x < 1.0のランダムな浮動小数点
//  
double mdlrand_gen_uniform_dbl()
{
    double ret = 0;
    ret = genrand_res53();
    return ret;
}

//
//  @brief 正規分布乱数生成
//   
//  [0,1)で正規分布乱数を返す
// 
//  @param mean 平均値
//  @param stddev 標準偏差
//  @return 0<= x < 1.0のランダムな正規分布乱数を返す
// 
double mdlrand_gen_normal_dbl(double mean, double stddev)
{
    double ret = 0;
    ret = box_muller_normal(mean, stddev);
    return ret;
}


/// @brief  random number generator for vector
/// @param v            vector
/// @param stddev       standard deviation
/// @return     vector with noise
vec3_t mdlrand_gen_normal_v3(vec3_t v_, vec3_t stddev_) 
{
    vec3_t _ans;

    // printf("stddev_.x:%f, stddev_.y:%f, stddev_.z:%f\n", stddev_.x, stddev_.y, stddev_.z);

    _ans.x = mdlrand_gen_normal_dbl(v_.x, stddev_.x);
    _ans.y = mdlrand_gen_normal_dbl(v_.y, stddev_.y);
    _ans.z = mdlrand_gen_normal_dbl(v_.z, stddev_.z);
    return _ans;
}