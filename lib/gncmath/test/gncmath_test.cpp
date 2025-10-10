#include <gtest/gtest.h>
#include "gncmath.h"

extern quat_t update_q(vec3_t v, quat_t q);

/// @brief  ベクトルの比較
/// @param v : 比較対象ベクトル
/// @param x : 期待値x
/// @param y : 期待値y
/// @param z : 期待値z
inline void EXCECT_VECTOR(vec3_t v, double x, double y, double z) 
{
    EXPECT_NEAR((v.x), (x), 1.0e-8); 
    EXPECT_NEAR((v.y), (y), 1.0e-8); 
    EXPECT_NEAR((v.z), (z), 1.0e-8);
}

/// @brief マトリックスの比較
/// @param m : 比較対象マトリックス
/// @param ax : 期待値ax
/// @param ay : 期待値ay
/// @param az : 期待値az
/// @param bx : 期待値bx
/// @param by : 期待値by
/// @param bz : 期待値bz
/// @param cx : 期待値cx
/// @param cy : 期待値cy
/// @param cz : 期待値cz
void EXCECT_MATRIX(mat33_t m, double ax, double ay, double az,
    double bx, double by, double bz,
    double cx, double cy, double cz) 
{
    EXPECT_NEAR((m.a.x), (ax), 1.0e-8); 
    EXPECT_NEAR((m.a.y), (ay), 1.0e-8); 
    EXPECT_NEAR((m.a.z), (az), 1.0e-8);
    EXPECT_NEAR((m.b.x), (bx), 1.0e-8); 
    EXPECT_NEAR((m.b.y), (by), 1.0e-8); 
    EXPECT_NEAR((m.b.z), (bz), 1.0e-8);
    EXPECT_NEAR((m.c.x), (cx), 1.0e-8); 
    EXPECT_NEAR((m.c.y), (cy), 1.0e-8); 
    EXPECT_NEAR((m.c.z), (cz), 1.0e-8);
}

/// @brief クォータニオンの比較
void EXCECT_QUAT(quat_t q, double s, double x, double y, double z) 
{
    EXPECT_NEAR((q.s), (s), 1.0e-8); 
    EXPECT_NEAR((q.x), (x), 1.0e-8); 
    EXPECT_NEAR((q.y), (y), 1.0e-8); 
    EXPECT_NEAR((q.z), (z), 1.0e-8);
}

/// @brief 角度をラジアンに変換
double deg2rad(double deg) 
{
    return deg * M_PI * (1/ 180.0);
}


// テストのユーティリティ //////////////////////////////////////////////////////////

class LibmathTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }

};

// テスト ///////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// ベクトルテスト ///////////////////////////////////////////////////////////////
/// @brief zeros_v3のテスト：正常系
TEST(zeros_v3_test, Normal) {
	vec3_t _v3 = zeros_v3();

	EXPECT_DOUBLE_EQ(_v3.x, 0.0);
	EXPECT_DOUBLE_EQ(_v3.y, 0.0);
	EXPECT_DOUBLE_EQ(_v3.z, 0.0);
}

/// @brief ones_v3のテスト：正常系
TEST(ones_v3_test, Normal) {
	vec3_t _v3 = ones_v3();

	EXPECT_DOUBLE_EQ(_v3.x, 1.0);
	EXPECT_DOUBLE_EQ(_v3.y, 1.0);
	EXPECT_DOUBLE_EQ(_v3.z, 1.0);
}

/// @brief val_to_v3のテスト：正常系
TEST(val_to_v3_test, Normal) {
    vec3_t _v3 = val_to_v3(1.0, 2.0, 3.0);

    EXPECT_DOUBLE_EQ(_v3.x, 1.0);
    EXPECT_DOUBLE_EQ(_v3.y, 2.0);
    EXPECT_DOUBLE_EQ(_v3.z, 3.0);
}

/// @brief array_to_v3のテスト：正常系
TEST(array_to_v3_test, Normal) {
    double _a[3] = {1.0, 2.0, 3.0};
    vec3_t _v3 = array_to_v3(_a);

    EXPECT_DOUBLE_EQ(_v3.x, 1.0);
    EXPECT_DOUBLE_EQ(_v3.y, 2.0);
    EXPECT_DOUBLE_EQ(_v3.z, 3.0);
}

/// @brief v3_to_arrayのテスト：正常系
TEST(v3_to_array_test, Normal) {
    double _a[3];
    vec3_t _v3 = val_to_v3(1.0, 2.0, 3.0);
    v3_to_array(_v3, _a);

    EXPECT_DOUBLE_EQ(_a[0], 1.0);
    EXPECT_DOUBLE_EQ(_a[1], 2.0);
    EXPECT_DOUBLE_EQ(_a[2], 3.0);

    print_v3(_v3, (char*)"v3");
}

/// @brief norm_v3のテスト：正常系
TEST(norm_v3_test, Normal) {
	EXPECT_DOUBLE_EQ(norm_v3(vec3_t{1.0,-2.0,4.0}), 4.58257569495584);
	EXPECT_DOUBLE_EQ(norm_v3(vec3_t{1.5,-2.5,4.5}), 5.361902647381804);
    EXPECT_DOUBLE_EQ(norm_v3(vec3_t{1.0,2.0,3.0}), 3.741657386773941);
    EXPECT_DOUBLE_EQ(norm_v3(vec3_t{1.0,2.0,4.0}), 4.58257569495584);
    EXPECT_DOUBLE_EQ(norm_v3(vec3_t{1.0,2.0,5.0}), 5.477225575051661);
    
}

/// @brief norm_v3のテスト：異常系
TEST(norm_v3_test, Error) {
    EXPECT_DOUBLE_EQ(norm_v3(vec3_t{0.00,0.00,0.00}), 0.00);
}

/// @brief nmlz_v3のテスト：正常系
TEST(nmlz_v3_test, Normal) {
    EXCECT_VECTOR(nmlz_v3(vec3_t{0.00,0.00,0.00}), 0.00, 0.00, 0.00);
	EXCECT_VECTOR(nmlz_v3(vec3_t{1.0,-2.0,4.0}), 0.2182178902359924, -0.4364357804719848, 0.8728715609439696);
	EXCECT_VECTOR(nmlz_v3(vec3_t{1.5,-2.5,4.5}), 0.27975144247209416, -0.4662524041201569, 0.8392543274162825);
}

/// @brief add_v3_v3のテスト：正常系
TEST(add_v3_v3_test, Normal) {
    EXCECT_VECTOR(add_v3_v3(vec3_t{1.0,-2.0,4.0}, vec3_t{1.5,-2.5,4.5}), 2.5, -4.5, 8.5);
    EXCECT_VECTOR(add_v3_v3(vec3_t{2.0,-2.0,4.0}, vec3_t{1.5,-2.5,4.5}), 3.5, -4.5, 8.5);
    EXCECT_VECTOR(add_v3_v3(vec3_t{1.0,-3.0,4.0}, vec3_t{1.5,-2.5,4.5}), 2.5, -5.5, 8.5);
    EXCECT_VECTOR(add_v3_v3(vec3_t{1.0,-2.0,5.0}, vec3_t{1.5,-2.5,4.5}), 2.5, -4.5, 9.5);
}

/// @brief add_v3_3のテスト：正常系
TEST(add_v3_3_test, Normal) {
    EXCECT_VECTOR(add_v3_3(vec3_t{1.0,-2.0,4.0}, vec3_t{1.5,-2.5,4.5}, vec3_t{1.0,-2.0,4.0}), 3.5, -6.5, 12.5);
    EXCECT_VECTOR(add_v3_3(vec3_t{2.0,-2.0,4.0}, vec3_t{1.5,-2.5,4.5}, vec3_t{1.0,-2.0,4.0}), 4.5, -6.5, 12.5);
    EXCECT_VECTOR(add_v3_3(vec3_t{1.0,-3.0,4.0}, vec3_t{1.5,-2.5,4.5}, vec3_t{1.0,-2.0,4.0}), 3.5, -7.5, 12.5);
    EXCECT_VECTOR(add_v3_3(vec3_t{1.0,-2.0,5.0}, vec3_t{1.5,-2.5,4.5}, vec3_t{1.0,-2.0,4.0}), 3.5, -6.5, 13.5);
}

/// @brief add_v3_4のテスト：正常系
TEST(add_v3_4_test, Normal) {
    EXCECT_VECTOR(add_v3_4(vec3_t{1.0,-2.0,4.0}, vec3_t{1.5,-2.5,4.5}, vec3_t{1.0,-2.0,4.0}, vec3_t{1.0,-2.0,4.0}), 4.5, -8.5, 16.5);
    EXCECT_VECTOR(add_v3_4(vec3_t{2.0,-2.0,4.0}, vec3_t{1.5,-2.5,4.5}, vec3_t{1.0,-2.0,4.0}, vec3_t{1.0,-2.0,4.0}), 5.5, -8.5, 16.5);
    EXCECT_VECTOR(add_v3_4(vec3_t{1.0,-3.0,4.0}, vec3_t{1.5,-2.5,4.5}, vec3_t{1.0,-2.0,4.0}, vec3_t{1.0,-2.0,4.0}), 4.5, -9.5, 16.5);
    EXCECT_VECTOR(add_v3_4(vec3_t{1.0,-2.0,5.0}, vec3_t{1.5,-2.5,4.5}, vec3_t{1.0,-2.0,4.0}, vec3_t{1.0,-2.0,4.0}), 4.5, -8.5, 17.5);
}

/// @brief sub_v3_v3のテスト：正常系
TEST(sub_v3_v3_test, Normal) {
    EXCECT_VECTOR(sub_v3_v3(vec3_t{1.0,-2.0,4.0}, vec3_t{1.5,-2.5,4.5}), -0.5, 0.5, -0.5);
    EXCECT_VECTOR(sub_v3_v3(vec3_t{2.0,-2.0,4.0}, vec3_t{1.5,-2.5,4.5}),  0.5, 0.5, -0.5);
    EXCECT_VECTOR(sub_v3_v3(vec3_t{1.0,-3.0,4.0}, vec3_t{1.5,-2.5,4.5}), -0.5, -0.5, -0.5);
    EXCECT_VECTOR(sub_v3_v3(vec3_t{1.0,-2.0,5.0}, vec3_t{1.5,-2.5,4.5}), -0.5, 0.5, 0.5);
}

/// @brief scl_v3のテスト：正常系
TEST(scl_v3_test, Normal) {
    EXCECT_VECTOR(scl_v3(2.0, vec3_t{1.0,-2.0,4.0}), 2.0, -4.0, 8.0);
    EXCECT_VECTOR(scl_v3(1.5, vec3_t{1.5,-2.5,4.5}), 2.25, -3.75, 6.75);
    EXCECT_VECTOR(scl_v3(1.0, vec3_t{1.0,-2.0,4.0}), 1.0, -2.0, 4.0);
}

/// @brief hdm_v3_v3のテスト：正常系
TEST(hdm_v3_v3_test, Normal) {
    EXCECT_VECTOR(hdm_v3_v3(vec3_t{0.0, 0.0,0.0}, vec3_t{1.5,-2.5,4.5}), 0.0, 0.0, 0.0);
    EXCECT_VECTOR(hdm_v3_v3(vec3_t{1.0,-2.0,4.0}, vec3_t{1.5,-2.5,4.5}), 1.5, 5.0, 18.0);
    EXCECT_VECTOR(hdm_v3_v3(vec3_t{2.0,-2.0,4.0}, vec3_t{1.5,-2.5,4.5}), 3.0, 5.0, 18.0);
    EXCECT_VECTOR(hdm_v3_v3(vec3_t{1.0,-3.0,4.0}, vec3_t{1.5,-2.5,4.5}), 1.5, 7.5, 18.0);
    EXCECT_VECTOR(hdm_v3_v3(vec3_t{1.0,-2.0,5.0}, vec3_t{1.5,-2.5,4.5}), 1.5, 5.0, 22.5);
}

//// @brief dot_v3_v3のテスト：正常系
TEST(dot_v3_v3_test, Normal) {
    EXPECT_DOUBLE_EQ(dot_v3_v3(vec3_t{0.0, 0.0,0.0}, vec3_t{1.5,-2.5,4.5}), 0.0);
    EXPECT_DOUBLE_EQ(dot_v3_v3(vec3_t{1.0,-2.0,4.0}, vec3_t{1.5,-2.5,4.5}), 24.5);
    EXPECT_DOUBLE_EQ(dot_v3_v3(vec3_t{2.0,-2.0,4.0}, vec3_t{1.5,-2.5,4.5}), 26.0);
    EXPECT_DOUBLE_EQ(dot_v3_v3(vec3_t{1.0,-3.0,4.0}, vec3_t{1.5,-2.5,4.5}), 27.0);
    EXPECT_DOUBLE_EQ(dot_v3_v3(vec3_t{1.0,-2.0,5.0}, vec3_t{1.5,-2.5,4.5}), 29.0);
}

TEST(crs_v3_v3_test, Normal) {
    EXCECT_VECTOR(crs_v3_v3(vec3_t{0.0, 0.0,0.0}, vec3_t{1.5,-2.5,4.5}), 0.0, 0.0, 0.0);
    EXCECT_VECTOR(crs_v3_v3(vec3_t{1.0,-2.0,4.0}, vec3_t{1.5,-2.5,4.5}), 1.0, 1.5, 0.5);
    EXCECT_VECTOR(crs_v3_v3(vec3_t{2.0,-2.0,4.0}, vec3_t{1.5,-2.5,4.5}), 1.0, -3.0, -2.0);
    EXCECT_VECTOR(crs_v3_v3(vec3_t{1.0,-3.0,4.0}, vec3_t{1.5,-2.5,4.5}), -3.5, 1.5, 2.0);
    EXCECT_VECTOR(crs_v3_v3(vec3_t{1.0,-2.0,5.0}, vec3_t{1.5,-2.5,4.5}), 3.5, 3.0, 0.5);
}

////////////////////////////////////////////////////////////////////////////////
// マトリックステスト ///////////////////////////////////////////////////////////
/// @brief dot_m33_v3のテスト：正常系
TEST(dot_m33_v3_test, Normal) {
    mat33_t m;
    EXCECT_VECTOR(dot_m33_v3(m, vec3_t{0.0,0.0,0.0}), 0.0, 0.0, 0.0);

    // x軸で30deg回転
    m = {
            {1.0, 0.0, 0.0},
            {0.0, 0.8660254037844387, -0.49999999999999994},
            {0.0, 0.49999999999999994, 0.8660254037844387}};
    EXCECT_VECTOR(dot_m33_v3(m, vec3_t{1.5,-2.5,4.5}), 1.5, 0.08493649053890292566, 5.147114317029974);

    // y軸で30deg回転
    m = {
        {0.8660254037844387, 0.0, 0.49999999999999994},
        {0.0, 1.0, 0.0},
        {-0.49999999999999994, 0.0, 0.8660254037844387}};
    EXCECT_VECTOR(dot_m33_v3(m, vec3_t{1.5,-2.5,4.5}), -0.9509618943233418, -2.5, 4.647114317029974);


    // z軸で30deg回転
    m = {
        {0.8660254037844387, -0.49999999999999994, 0.0},
        {0.49999999999999994, 0.8660254037844387, 0.0},
        {0.0, 0.0, 1.0}};
    EXCECT_VECTOR(dot_m33_v3(m, vec3_t{1.5,-2.5,4.5}), 0.04903810567665828346, -2.915063509461097, 4.5);
}

/// @brief  zeros_m33のテスト：正常系
TEST(zeros_m33_test, Normal) {
    EXCECT_MATRIX(zeros_m33(), 
                0, 0, 0,
                0, 0, 0,
                0, 0, 0);

}

/// @brief  ones_m33のテスト：正常系
TEST(ones_m33_test, Normal) {
    EXCECT_MATRIX(ones_m33(), 
                1.0, 1.0, 1.0,
                1.0, 1.0, 1.0,
                1.0, 1.0, 1.0);
}

/// @brief  eye_m33のテスト：正常系
TEST(eye_m33_test, Normal) {
    EXCECT_MATRIX(eye_m33(), 
                1.0, 0.0, 0.0,
                0.0, 1.0, 0.0,
                0.0, 0.0, 1.0);
    print_m33(ones_m33(), (char*)"eye_m33");            
}

/// @brief  eye_m33のテスト：正常系
TEST(euler321_to_dcm_test, Normal) {
    mat33_t m;
    m = euler321_to_dcm(vec3_t{deg2rad(30) , deg2rad(45), deg2rad(60)});
    EXCECT_MATRIX(m, 
        0.35355339059327384, -0.5732233047033631, 0.7391989197401165,
        0.6123724356957946, 0.7391989197401166, 0.2803300858899106,
        -0.7071067811865475, 0.35355339059327373, 0.6123724356957946);

    m = euler321_to_dcm(vec3_t{deg2rad(45) , deg2rad(60), deg2rad(30)});
    EXCECT_MATRIX(m, 
        0.43301270189221946, 0.17677669529663687, 0.8838834764831843,
        0.25, 0.9185586535436917, -0.30618621784789724,
        -0.8660254037844386, 0.3535533905932738, 0.35355339059327384);
    
    m = euler321_to_dcm(vec3_t{deg2rad(60) , deg2rad(30), deg2rad(45)});
    EXCECT_MATRIX(m, 
        0.6123724356957946, -0.04736717274537657, 0.7891491309924313,
        0.6123724356957945, 0.659739608441171, -0.4355957403991577,
        -0.49999999999999994, 0.75, 0.43301270189221946);
}

/// @brief  eye_m33のテスト：正常系
TEST(euler321_to_dcm_test, GimbalLock) {
    mat33_t m;
    m = euler321_to_dcm(vec3_t{deg2rad(90) , deg2rad(45), deg2rad(60)});
    EXCECT_MATRIX(m, 
         0.35355339059327384, 0.35355339059327373, 0.8660254037844386,
         0.6123724356957946,  0.6123724356957945, -0.5000000000000001,
        -0.7071067811865475,  0.7071067811865476,  0.0);
 
    m = euler321_to_dcm(vec3_t{deg2rad(30) , deg2rad(90), deg2rad(60)});
    EXCECT_MATRIX(m, 
         0.0, -0.50,              0.8660254037844387,
         0.0, 0.8660254037844387, 0.50,
        -1.0, 0.0,                0.0);
    
    m = euler321_to_dcm(vec3_t{deg2rad(30) , deg2rad(45), deg2rad(90)});
    EXCECT_MATRIX(m, 
        0.0,                -0.8660254037844387,  0.50,
        0.7071067811865476,  0.35355339059327373, 0.6123724356957945,
        -0.7071067811865475, 0.35355339059327373, 0.6123724356957946);
}        



/// @brief  dcm_to_euler321のテスト：正常系
TEST(dcm_to_euler321_test, Normal) {
    mat33_t m;
    vec3_t v;
    int __max2 = 0;    
    m = euler321_to_dcm(vec3_t{deg2rad(30) , deg2rad(45), deg2rad(60)});

    v = dcm_to_euler321(m, vec3_t{0.0, 0.0, 0.0});
    EXCECT_VECTOR(v, 
        deg2rad(30) , deg2rad(45), deg2rad(60));    
}

/// @brief  dcm_to_euler321のテスト：GimbalLock
TEST(dcm_to_euler321_test, GimbalLock) {
    mat33_t m;
    vec3_t v;    
    m = euler321_to_dcm(vec3_t{deg2rad(90) , deg2rad(0), deg2rad(0)});
    m = {{1, 0, 0},
        {0, 0, -1},
        {0, 1, 0}};    
    v = dcm_to_euler321(m, vec3_t{0.0, 0.0, 0.0});
    // print_m33(m, (char*)"dcm");
    EXCECT_VECTOR(v, 
        deg2rad(90) , deg2rad(0), deg2rad(0)); 
        
    m = euler321_to_dcm(vec3_t{deg2rad(0) , deg2rad(90), deg2rad(0)});
    m = {{0, 0, 1},
        {0, 1, 0},
        {-1, 0, 0}};    
    v = dcm_to_euler321(m, vec3_t{0.0, 0.0, 0.0});
    // print_m33(m, (char*)"dcm");
    EXCECT_VECTOR(v, 
        deg2rad(0) , deg2rad(90), deg2rad(0));    

    m = euler321_to_dcm(vec3_t{deg2rad(0) , deg2rad(0), deg2rad(90)});
    m = {{0, -1, 0},
        {1, 0, 0},
        {0, 0, 1}};
    v = dcm_to_euler321(m, vec3_t{0.0, 0.0, 0.0});
    // print_m33(m, (char*)"dcm");
    EXCECT_VECTOR(v, 
        deg2rad(0) , deg2rad(0), deg2rad(90));         

}

/// @brief  dcm_to_euler321のテスト：異常系
TEST(dcm_to_euler321_test, Error) {
    mat33_t m;
    vec3_t v;    
    m = zeros_m33();
    v = dcm_to_euler321(m, vec3_t{-1, -1, -1});
    EXCECT_VECTOR(v, 
        -1 , -1, -1);         
}


/// @brief  trns_m33のテスト：正常系
TEST(trns_m33_test, Normal) {
    mat33_t m, m0;
    m0 = euler321_to_dcm(vec3_t{deg2rad(30) , deg2rad(45), deg2rad(60)});

    m = trns_m33(m0);
    EXCECT_MATRIX(m, 
        0.35355339059327384188, 0.61237243569579458136, -0.70710678118654746172,
        -0.57322330470336313457, 0.73919891974011664448, 0.35355339059327373086,
        0.73919891974011653346, 0.28033008588991065180, 0.61237243569579458136);    
}

/// @brief  det_m33のテスト：正常系
TEST(det_m33_test, Normal) {
    mat33_t m;
    m = euler321_to_dcm(vec3_t{deg2rad(30) , deg2rad(45), deg2rad(60)});

    EXPECT_DOUBLE_EQ(det_m33(m), 1.0);
}

/// @brief  inv_m33のテスト：正常系
TEST(inv_m33_test, Normal) {
    mat33_t m;
    m = euler321_to_dcm(vec3_t{deg2rad(30) , deg2rad(45), deg2rad(60)});

    EXCECT_MATRIX(inv_m33(m),
        0.35355339059327384188, 0.61237243569579447033, -0.70710678118654746172,
        -0.57322330470336313457, 0.73919891974011653346, 0.35355339059327367535,
        0.73919891974011675551, 0.28033008588991054078, 0.61237243569579458136);
}

/// @brief  inv_m33のテスト：異常系
TEST(inv_m33_test, Error) {
    mat33_t m;
    m = zeros_m33();
    EXCECT_MATRIX(inv_m33(m),
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0);
}


/// @brief  mult_m33_m33のテスト：正常系
TEST(dot_m33_test, Normal) {
    mat33_t m, r_x, r_y, r_z;
    r_x = euler321_to_dcm(vec3_t{deg2rad(30) , 0.0, 0.0});
    r_y = euler321_to_dcm(vec3_t{0.0 , deg2rad(45), 0.0});
    r_z = euler321_to_dcm(vec3_t{0.0 , 0.0, deg2rad(60)});
    
    m = mult_m33_m33(r_y, r_z);
    EXCECT_MATRIX(m,
        0.35355339059327384188, -0.86602540378443859659, 0.35355339059327378637,
        0.61237243569579458136, 0.50000000000000011102, 0.61237243569579447033,
        -0.70710678118654746172, 0.00000000000000000000, 0.70710678118654757274);

    m = mult_m33_m33(r_x, m);
    EXCECT_MATRIX(m,
        0.35355339059327384188, -0.57322330470336313457, 0.73919891974011653346,
        0.61237243569579458136, 0.73919891974011664448, 0.28033008588991065180,
        -0.70710678118654746172, 0.35355339059327373086, 0.61237243569579458136);

}

////////////////////////////////////////////////////////////////////////////////
// クォーターニオンテスト ////////////////////////////////////////////////////////

/// @brief  ones_qのテスト：正常系
TEST(ones_q_test, Normal) {
    quat_t q = eye_q();

    EXCECT_QUAT(q, 1.0, 0.0, 0.0, 0.0);
    print_q(q, (char*)"q");
}

/// @brief  val_to_qのテスト：正常系
TEST(val_to_q_test, Normal) {
    quat_t q = val_to_q(1.0, 2.0, 3.0, 4.0);

    EXCECT_QUAT(q, 1.0, 2.0, 3.0, 4.0);
}

/// @brief  array_to_qのテスト：正常系
TEST(array_to_q_test, Normal) {
    double a[4] = {1.0, 2.0, 3.0, 4.0};
    quat_t q = array_to_q(a);

    EXCECT_QUAT(q, 1.0, 2.0, 3.0, 4.0);
}

/// @brief  q_to_arrayのテスト：正常系
TEST(q_to_array_test, Normal) {
    double a[4];
    quat_t q = val_to_q(1.0, 2.0, 3.0, 4.0);
    q_to_array(q, a);

    EXPECT_DOUBLE_EQ(a[0], 1.0);
    EXPECT_DOUBLE_EQ(a[1], 2.0);
    EXPECT_DOUBLE_EQ(a[2], 3.0);
    EXPECT_DOUBLE_EQ(a[3], 4.0);
}

/// @brief  norm_qのテスト：正常系
TEST(norm_q_test, Normal) { 
    quat_t q;
    EXPECT_DOUBLE_EQ(norm_q(eye_q()), 1.0);
    q = val_to_q(1.0, 2.0, 3.0, 4.0);
    EXPECT_DOUBLE_EQ(norm_q(q), 5.477225575051661);
    q = val_to_q(1.5, -2.5, 4.5, -3.5);
    EXPECT_DOUBLE_EQ(norm_q(q), 6.4031242374328485);
}

/// @brief  nmlz_qのテスト：異常系
TEST(norm_q_test, Error) { 
    quat_t q = {0.0, 0.0, 0.0, 0.0};
    EXPECT_DOUBLE_EQ(norm_q(q), 0.0);
}

/// @brief  nmlz_qのテスト：正常系
TEST(nmlz_q_test, Normal) { 
    quat_t q;
    q = nmlz_q(eye_q());
    EXCECT_QUAT(q, 1.0, 0.0, 0.0, 0.0);
    q = val_to_q(1.0, 2.0, 3.0, 4.0);
    q = nmlz_q(q);
    EXCECT_QUAT(q, 0.182574185835055, 0.365148371670111, 0.547722557505166, 0.730296743340221);
    q = val_to_q(1.5, -2.5, 4.5, -3.5);
    q = nmlz_q(q);
    EXCECT_QUAT(q, 0.234260642832909, -0.390434404721515, 0.702781928498727, -0.546608166610121);
}

/// @brief  nmlz_qのテスト：異常系
TEST(nmlz_q_test, Error) {
    quat_t q = {0.0, 0.0, 0.0, 0.0};
    q = nmlz_q(q);
    EXCECT_QUAT(q, 0.0, 0.0, 0.0, 0.0);
} 

/// @brief  euler321_to_qのテスト：正常系
TEST(euler321_to_q_test, Normal) { 
    quat_t q;
    q = euler321_to_q(vec3_t{deg2rad(30) , deg2rad(45), deg2rad(60)});
    EXCECT_QUAT(q, 
        0.8223631719059994, 0.022260026714733816, 0.43967973954090955, 0.3604234056503559);

    q = euler321_to_q(vec3_t{deg2rad(-45) , deg2rad(60), deg2rad(-30)});
    EXCECT_QUAT(q, 
        0.8223631719059994, -0.20056212114657512, 0.5319756951821668, -0.022260026714733844);
}

/// @brief  q_to_euler321のテスト：正常系
TEST(q_to_euler321_test, Normal) { 
    quat_t q;
    q = euler321_to_q(vec3_t{deg2rad(30) , deg2rad(45), deg2rad(60)});
    vec3_t v = q_to_euler321(q);
    EXCECT_VECTOR(v, 
        0.5235987755982989, 0.7853981633974483, 1.0471975511965976);

    q = euler321_to_q(vec3_t{deg2rad(-45) , deg2rad(60), deg2rad(-30)});
    v = q_to_euler321(q);
    EXCECT_VECTOR(v, 
        -0.7853981633974485, 1.0471975511965974, -0.5235987755982989);    
}

/// @brief  dcm_to_qのテスト：正常系
TEST(dcm_to_q_test, Normal) { 
    mat33_t m;
    m = euler321_to_dcm(vec3_t{deg2rad(30) , deg2rad(45), deg2rad(60)});
    quat_t q = dcm_to_q(m);
    EXCECT_QUAT(q, 
        0.8223631719059994, 0.022260026714733816, 0.43967973954090955, 0.3604234056503559);

    m = euler321_to_dcm(vec3_t{deg2rad(-45) , deg2rad(60), deg2rad(-30)});
    q = dcm_to_q(m);
    EXCECT_QUAT(q, 
        0.8223631719059994, -0.20056212114657512, 0.5319756951821668, -0.022260026714733844);
}

/// @brief  dcm_to_qのテスト：GimbalLock
TEST(dcm_to_q_test, GimbalLock) { 
    mat33_t m;
    // m = euler321_to_dcm(vec3_t{deg2rad(90) , deg2rad(0), deg2rad(0)});
    m = {{1, 0, 0},
        {0, 0, -1},
        {0, 1, 0}};    
    quat_t q = dcm_to_q(m);
    EXCECT_QUAT(q, 
        0.7071067811865476, 0.7071067811865475, 0.0, 0.0);

    // m = euler321_to_dcm(vec3_t{deg2rad(0) , deg2rad(90), deg2rad(0)});
    m = {{0, 0, 1},
        {0, 1, 0},
        {-1, 0, 0}};    
    q = dcm_to_q(m);
    EXCECT_QUAT(q, 
        0.7071067811865476, 0.0, 0.7071067811865475, 0.0);

    // // m = euler321_to_dcm(vec3_t{deg2rad(0) , deg2rad(0), deg2rad(90)});
    m = {{0, -1, 0},
        {1, 0, 0},
        {0, 0, 1}};   
    q = dcm_to_q(m);
    EXCECT_QUAT(q, 
        0.7071067811865476, 0.0, 0.0, 0.7071067811865475);
}

/// @brief  dcm_to_qのテスト：対角成分が負の値
TEST(dcm_to_q_test, TraceNegaviveNum) { 
    mat33_t m;
    m = euler321_to_dcm(vec3_t{deg2rad(150) , deg2rad(0), deg2rad(0)});
    print_m33(m, (char*)"dcm");
    quat_t q = dcm_to_q(m);
    EXCECT_QUAT(q, 
        0.25881904510252074, 0.9659258262890683, 0.0, 0.0);

    m = euler321_to_dcm(vec3_t{deg2rad(0) , deg2rad(150), deg2rad(0)});
    print_m33(m, (char*)"dcm");
    q = dcm_to_q(m);
    EXCECT_QUAT(q, 
        0.25881904510252074, 0.0, 0.9659258262890683, 0.0);

    m = euler321_to_dcm(vec3_t{deg2rad(0) , deg2rad(0), deg2rad(150)});
    print_m33(m, (char*)"dcm");
    q = dcm_to_q(m);
    EXCECT_QUAT(q, 
        0.25881904510252074, 0.0, 0.0, 0.9659258262890683);

    m = euler321_to_dcm(vec3_t{deg2rad(150) , deg2rad(45), deg2rad(60)});
    print_m33(m, (char*)"dcm");
    q = dcm_to_q(m);
    EXCECT_QUAT(q, 
        0.3919038373291199, 0.7233174113647118, 0.5319756951821668, -0.20056212114657512);

    m = euler321_to_dcm(vec3_t{deg2rad(45) , deg2rad(150), deg2rad(60)});
    print_m33(m, (char*)"dcm");
    q = dcm_to_q(m);
    EXCECT_QUAT(q, 
        0.3919038373291199, -0.3604234056503559, 0.8223631719059994, -0.20056212114657512);

    m = euler321_to_dcm(vec3_t{deg2rad(45) , deg2rad(60), deg2rad(150)});
    print_m33(m, (char*)"dcm");
    q = dcm_to_q(m);
    EXCECT_QUAT(q, 
        0.39190383732911993, -0.3604234056503559, 0.43967973954090955, 0.7233174113647118);


    m = {vec3_t{ 1.0, 0.0, 0.0},
         vec3_t{ 0.0, -0.86602540378443870, -0.5},
         vec3_t{ 0.0, 0.5,                 -0.86602540378443870}};
    print_m33(m, (char*)"dcm");
    q = dcm_to_q(m);
    EXCECT_QUAT(q, 
        0.25881904510252074, 0.9659258262890683, 0.0, 0.0);

}


// @brief  q_to_dcmのテスト：正常系
TEST(q_to_dcm_test, Normal) { 
    quat_t q;
    q = euler321_to_q(vec3_t{deg2rad(30) , deg2rad(45), deg2rad(60)});
    mat33_t m = q_to_dcm(q);
    EXCECT_MATRIX(m, 
        0.35355339059327384, -0.5732233047033631, 0.7391989197401165,
        0.6123724356957946, 0.7391989197401166, 0.2803300858899106,
        -0.7071067811865475, 0.35355339059327373, 0.6123724356957946);

    q = euler321_to_q(vec3_t{deg2rad(-45) , deg2rad(60), deg2rad(-30)});
    m = q_to_dcm(q);
    EXCECT_MATRIX(m, 
        0.4330127018922194, -0.1767766952966369, 0.8838834764831844,
        -0.2500000000000001, 0.9185586535436918, 0.30618621784789735,
        -0.8660254037844386, -0.35355339059327395, 0.35355339059327384);    
}

/// @brief  q_to_dcmのテスト：GimbalLock
TEST(q_to_dcm_test, GimbalLock) { 
    quat_t q;
    mat33_t m;

    q = euler321_to_q(vec3_t{deg2rad(90) , deg2rad(30), deg2rad(60)});
    m = q_to_dcm(q);
    EXCECT_MATRIX(m, 
        0.43301270189221946, 0.250,                0.8660254037844386,
        0.750,               0.43301270189221935, -0.50,
        -0.50,               0.8660254037844387,   0.0);
   
    q = euler321_to_q(vec3_t{deg2rad(30) , deg2rad(90), deg2rad(60)});
    m = q_to_dcm(q);
    EXCECT_MATRIX(m, 
         0.0, -0.5,                0.8660254037844388,
         0.0,  0.8660254037844388, 0.5,
        -1.0,  0.0,                0.0);

    q = euler321_to_q(vec3_t{deg2rad(30) , deg2rad(60), deg2rad(90)});
    m = q_to_dcm(q);
    EXCECT_MATRIX(m, 
         0.0,                -0.8660254037844384, 0.50,
         0.5,                 0.4330127018922194, 0.7499999999999998,
        -0.8660254037844385,  0.250,              0.4330127018922193);
}

/// @brief  qproのテスト：正常系
TEST(qpro_test, Normal) { 
    quat_t q1, q2, q3;
    q2 = euler321_to_q(vec3_t{deg2rad(-45) , deg2rad(60), deg2rad(-30)});

    q1 = euler321_to_q(vec3_t{deg2rad(30) , deg2rad(45), deg2rad(60)});
    q3 = qpro(q1, q2);
    EXCECT_QUAT(q3, 
        0.4548698042205856, -0.3481528504633321, 0.7272618713398046, 0.3781180032977912);

    q1 = euler321_to_q(vec3_t{deg2rad(45) , deg2rad(60), deg2rad(30)});
    q3 = qpro(q1, q2);
    EXCECT_QUAT(q3, 
        0.4340037194709008, -0.023683586372688303, 0.8749544401338115, 0.2133883476483185);

    q1 = euler321_to_q(vec3_t{deg2rad(60) , deg2rad(30), deg2rad(45)});
    q3 = qpro(q1, q2);
    EXCECT_QUAT(q3, 
        0.5445496711223732, 0.016046069276784397, 0.7275623730145923, 0.4169666326154349);
}

/// @brief  qerrのテスト：Nomral
TEST(qerr_test, Normal) { 
    quat_t q1, q2, q3;
    vec3_t v;
    q1 = euler321_to_q(vec3_t{deg2rad(30) , deg2rad(45), deg2rad(60)});
    q2 = euler321_to_q(vec3_t{deg2rad(-45) , deg2rad(60), deg2rad(-30)});
    q3 = qerr(q1, q2);
    EXCECT_QUAT(q3, 
        0.897692568794007, 0.01828304624274646, 0.1476925687940069, -0.41472965564947284);

}

/// @brief  dq_dtのテスト：正常系
TEST(dq_dt_test, Normal) {
    quat_t q1, q2;
    vec3_t w0, w;

    // 初期姿勢：defalt
    q1 = eye_q();
    w = vec3_t{deg2rad(90) , deg2rad(0), deg2rad(0)};
    q2 = calc_dq_dt(w, q1);
    EXCECT_QUAT(q2, 0.0, 0.7853981633974483, 0.0, 0.0);

    w = vec3_t{deg2rad(0) , deg2rad(90), deg2rad(0)};
    q2 = calc_dq_dt(w, q1);
    EXCECT_QUAT(q2, 0.0, 0.0, 0.7853981633974483, 0.0);

    w = vec3_t{deg2rad(0) , deg2rad(0), deg2rad(90)};
    q2 = calc_dq_dt(w, q1);
    EXCECT_QUAT(q2, 0.0, 0.0, 0.0, 0.7853981633974483);

    w = vec3_t{deg2rad(30) , deg2rad(45), deg2rad(60)};
    q2 = calc_dq_dt(w, q1);
    EXCECT_QUAT(q2, 0.0, 0.2617993877991494, 0.39269908169872414, 0.5235987755982988);

    // 初期姿勢：任意
    q1 = euler321_to_q(vec3_t{deg2rad(30) , deg2rad(45), deg2rad(60)});
    w = vec3_t{deg2rad(1) , deg2rad(2), deg2rad(3)};
    q2 = calc_dq_dt(w, q1);
    EXCECT_QUAT(q2, -0.017303977182951742, 0.012396686032552024, 0.016915466425197742, 0.01808099869845975);
} 


