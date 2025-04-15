#include <gtest/gtest.h>
#include "gncmath.h"

// テストのユーティリティ //////////////////////////////////////////////////////////

class LibmathTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }

};

// テスト ///////////////////////////////////////////////////////////////////////

// zeros_v3
TEST_F(LibmathTest, Normal) {
	vec3_t _v3 = zeros_v3();

	EXPECT_DOUBLE_EQ(_v3.x, 0.0);
	EXPECT_DOUBLE_EQ(_v3.y, 0.0);
	EXPECT_DOUBLE_EQ(_v3.z, 0.0);
}
