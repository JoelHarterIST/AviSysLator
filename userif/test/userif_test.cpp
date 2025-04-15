#include <gtest/gtest.h>
#include "userif.h"

// テストのユーティリティ //////////////////////////////////////////////////////////

class UserifTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }

};

// テスト ///////////////////////////////////////////////////////////////////////

// zeros_v3
TEST_F(UserifTest, Normal) {
	int32_t _ret =0;
	char* test_case_file = "test_case.yaml";

	_ret = userif_init(0.0, test_case_file);
	EXPECT_EQ(_ret, 0);
}
