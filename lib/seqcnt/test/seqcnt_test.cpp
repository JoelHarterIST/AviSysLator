/// @file   seqcnt_test.cpp
/// @brief  test functions for seqcnt.c

#include <gtest/gtest.h>
#include "seqcnt.h"


///////////////////////////////////////////////////////////////////////////////
// test_if_get, test_if_set
///////////////////////////////////////////////////////////////////////////////

#define TEST_STATE_STOP (0x00000000)
#define TEST_STATE_EXEC (0x00000001)

#define TEST_OPTION_ONE (0x00000010)
#define TEST_OPTION_TWO (0x00000011)

typedef struct {
	int32_t mode;
	int option;
	double param1;
	double param2;
} test_if_get_t;

typedef struct {
	int32_t mode;
	int option;
	double param1;
	double param2;
} test_if_set_t;


///////////////////////////////////////////////////////////////////////////////
// test utilities
///////////////////////////////////////////////////////////////////////////////

class SeqCtrlTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }

	void set_seqcnt_state(seqcnt_state_t* psqs, int32_t mode, int id, int step, int error) {
		psqs->mode = mode;
		psqs->id = id;
		psqs->step = step;
		psqs->error = error;
	}

	void set_test_if_get(int32_t mode, int _option, double _param1, double _param2) {
		test_if_get.mode = mode;
		test_if_get.option = _option;
		test_if_get.param1 = _param1;
		test_if_get.param2 = _param2;
	}

	void set_test_if_set(int mode, int _option, double _param1, double _param2) {
		test_if_set.mode = mode;
		test_if_set.option = _option;
		test_if_set.param1 = _param1;
		test_if_set.param2 = _param2;
	}

	void set_judge_data(int idx, var_type_t _type, judge_ope_t _ope,
		ADDRESS_T _ref, int _thrs_i, double _thrs_d, int _count, int _num, ADDRESS_T _next) {
		judge_data_base[idx].type = _type;
		judge_data_base[idx].ope = _ope;
		judge_data_base[idx].ref = _ref;
		switch (_type) {
			case INT:
				judge_data_base[idx].thrs = (uint64_t)_thrs_i;
				break;
			case DBL:
				judge_data_base[idx].thrs = *((uint64_t*)&_thrs_d);
				break;
			default:
				break;
		}
		judge_data_base[idx].count = _count;
		judge_data_base[idx].num = _num;
		judge_data_base[idx].next = _next;
	}

	void set_exec_data(int idx, var_type_t _type, ADDRESS_T _dest, int _src_i, double _src_d, ADDRESS_T _next) {
		exec_data_base[idx].type = _type;
		exec_data_base[idx].dest = _dest;
		switch (_type) {
			case INT:
				exec_data_base[idx].src = (uint64_t)_src_i;
				break;
			case DBL:
				exec_data_base[idx].src = *((uint64_t*)&_src_d);
				break;
			default:
				break;
		}
		exec_data_base[idx].next = _next;
	}

	void set_event_data(int idx, 
						judge_condition_t judge_condition,
						ADDRESS_T judge_data_address, 
						ADDRESS_T exec_data_address, 
						int next_step, 
						int next_seq, 
						ADDRESS_T next_event) {
		event_data_base[idx].judge_condition = judge_condition;
		event_data_base[idx].judge_data_address = judge_data_address;
		event_data_base[idx].exec_data_address = exec_data_address;
		event_data_base[idx].next_step = next_step;
		event_data_base[idx].next_seq = next_seq;
		event_data_base[idx].next_event = next_event;
	}

    int testint;
	int seq_id_data_base[SEQ_ID_MAX];
	event_data_t* seq_step_data_base[SEQ_STEP_MAX];
	event_data_t event_data_base[20];
	seqcnt_state_t seqcnt_state;
	seqcnt_state_t seqcnt_state_next;
	test_if_get_t test_if_get;
	test_if_set_t test_if_set;
	judge_data_t judge_data_base[20];
	exec_data_t exec_data_base[20];
};


///////////////////////////////////////////////////////////////////////////////
// test code
///////////////////////////////////////////////////////////////////////////////

/// \brief TEST test_func
///
/// てすとかんすう
TEST_F(SeqCtrlTest, TestFunc) {
	testint = 0;
	test_func(0, &testint);
    EXPECT_EQ(testint, 3);
}

/// \brief TEST seqcnt
///
/// てすと
TEST_F(SeqCtrlTest, SeqCtrlOnAlwaysTrueNoExec) {
	int i = 0;
	seq_id_data_base[0] = 0;
	seq_step_data_base[0] = &event_data_base[0];

	set_event_data(0, ALL_AND, 0, 0, 1, -1, 0);

	set_seqcnt_state(&seqcnt_state, SEQCNT_EXEC, 0, 0, 0);
	set_seqcnt_state(&seqcnt_state_next, SEQCNT_EXEC, 0, 0, 0);

	seqcnt(&seqcnt_state, 
				&seqcnt_state_next, 
				&seq_id_data_base[0], 
				&seq_step_data_base[0]);

	EXPECT_EQ(seqcnt_state.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state.id, 0);
	EXPECT_EQ(seqcnt_state.step, 0);
	EXPECT_EQ(seqcnt_state.error, 0);
	EXPECT_EQ(seqcnt_state_next.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state_next.id, 0);
	EXPECT_EQ(seqcnt_state_next.step, 1);
	EXPECT_EQ(seqcnt_state_next.error, 0);
}

/// \brief TEST seqcnt
///
/// てすと
TEST_F(SeqCtrlTest, SeqCtrlOnJudgeTrueAndExec) {
	int i = 0;
	seq_id_data_base[0] = 0;
	seq_step_data_base[0] = &event_data_base[0];

	set_seqcnt_state(&seqcnt_state, SEQCNT_EXEC, 0, 0, 0);
	set_seqcnt_state(&seqcnt_state_next, SEQCNT_EXEC, 0, 0, 0);
	set_test_if_get(TEST_STATE_STOP, TEST_OPTION_ONE, 123.456, -123.456);
	set_test_if_set(TEST_STATE_STOP, TEST_OPTION_ONE, 0.0, 0.0);

	set_event_data(0, ALL_AND, 
		(ADDRESS_T)&judge_data_base[0], (ADDRESS_T)&exec_data_base[0], 
		1, -1, 0);
	set_judge_data(0, INT, EQ, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_STOP, 0.0, 0, 1, 0);
	set_exec_data(0, INT, (ADDRESS_T)&test_if_set.mode, 
		TEST_STATE_EXEC, 0.0, 0);

	seqcnt(&seqcnt_state, 
				&seqcnt_state_next, 
				&seq_id_data_base[0], 
				&seq_step_data_base[0]);

	EXPECT_EQ(seqcnt_state.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state.id, 0);
	EXPECT_EQ(seqcnt_state.step, 0);
	EXPECT_EQ(seqcnt_state.error, 0);
	EXPECT_EQ(seqcnt_state_next.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state_next.id, 0);
	EXPECT_EQ(seqcnt_state_next.step, 1);
	EXPECT_EQ(seqcnt_state_next.error, 0);
	EXPECT_EQ(test_if_set.mode, TEST_STATE_EXEC);
	EXPECT_EQ(test_if_set.option, TEST_OPTION_ONE);
	EXPECT_DOUBLE_EQ(test_if_set.param1, 0.0);
	EXPECT_DOUBLE_EQ(test_if_set.param2, 0.0);
}

/// \brief TEST seqcnt
///
/// てすと
TEST_F(SeqCtrlTest, SeqCtrlChangeStateResetCount) {
	int i = 0;
	seq_id_data_base[0] = 0;
	seq_step_data_base[0] = &event_data_base[0];

	free_timer = 10;
	seq_timer = 20;
	step_timer = 30;
	set_event_data(0, ALL_AND, 0, 0, 1, -1, 0);

	set_seqcnt_state(&seqcnt_state, SEQCNT_STOP, 0, 0, 0);
	set_seqcnt_state(&seqcnt_state_next, SEQCNT_EXEC, 0, 0, 0);

	seqcnt(&seqcnt_state, 
					&seqcnt_state_next, 
					&seq_id_data_base[0], 
					&seq_step_data_base[0]);

	EXPECT_EQ(seqcnt_state.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state.id, 0);
	EXPECT_EQ(seqcnt_state.step, 0);
	EXPECT_EQ(seqcnt_state.error, 0);
	EXPECT_EQ(seqcnt_state_next.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state_next.id, 0);
	EXPECT_EQ(seqcnt_state_next.step, 1);
	EXPECT_EQ(seqcnt_state_next.error, 0);
	EXPECT_EQ(free_timer, 1);
	EXPECT_EQ(seq_timer, 1);
	EXPECT_EQ(step_timer, 1);
}

/// \brief TEST seqcnt
///
/// てすと
TEST_F(SeqCtrlTest, SeqCtrlChangeIdResetCount) {
	int i = 0;
	seq_id_data_base[0] = 0;
	seq_step_data_base[0] = &event_data_base[0];

	free_timer = 10;
	seq_timer = 20;
	step_timer = 30;
	set_event_data(0, ALL_AND, 0, 0, -1, 2, 0);

	set_seqcnt_state(&seqcnt_state, SEQCNT_EXEC, 0, 0, 0);
	set_seqcnt_state(&seqcnt_state_next, SEQCNT_EXEC, 1, 0, 0);

	seqcnt(&seqcnt_state, 
				&seqcnt_state_next, 
				&seq_id_data_base[0], 
				&seq_step_data_base[0]);

	EXPECT_EQ(seqcnt_state.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state.id, 1);
	EXPECT_EQ(seqcnt_state.step, 0);
	EXPECT_EQ(seqcnt_state.error, 0);
	EXPECT_EQ(seqcnt_state_next.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state_next.id, 2);
	EXPECT_EQ(seqcnt_state_next.step, 0);
	EXPECT_EQ(seqcnt_state_next.error, 0);
	EXPECT_EQ(free_timer, 11);
	EXPECT_EQ(seq_timer, 1);
	EXPECT_EQ(step_timer, 1);
}

/// \brief TEST seqcnt
///
/// てすと
TEST_F(SeqCtrlTest, SeqCtrlChangeStepResetCount) {
	int i = 0;
	seq_id_data_base[0] = 0;
	seq_step_data_base[0] = &event_data_base[0];
	seq_step_data_base[1] = &event_data_base[1];

	free_timer = 10;
	seq_timer = 20;
	step_timer = 30;
	set_event_data(0, ALL_AND, 0, 0, 1, -1, 0);
	set_event_data(1, ALL_AND, 0, 0, 2, -1, 0);

	set_seqcnt_state(&seqcnt_state, SEQCNT_EXEC, 0, 0, 0);
	set_seqcnt_state(&seqcnt_state_next, SEQCNT_EXEC, 0, 1, 0);

	seqcnt(&seqcnt_state, 
				&seqcnt_state_next, 
				&seq_id_data_base[0], 
				&seq_step_data_base[0]);

	EXPECT_EQ(seqcnt_state.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state.id, 0);
	EXPECT_EQ(seqcnt_state.step, 1);
	EXPECT_EQ(seqcnt_state.error, 0);
	EXPECT_EQ(seqcnt_state_next.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state_next.id, 0);
	EXPECT_EQ(seqcnt_state_next.step, 2);
	EXPECT_EQ(seqcnt_state_next.error, 0);
	EXPECT_EQ(free_timer, 11);
	EXPECT_EQ(seq_timer, 21);
	EXPECT_EQ(step_timer, 1);
}

/// \brief TEST seqcnt
///
/// てすと
TEST_F(SeqCtrlTest, SeqCtrlOn1stEventTrue) {
	int i = 0;
	seq_id_data_base[0] = 0;
	seq_step_data_base[0] = &event_data_base[0];

	set_seqcnt_state(&seqcnt_state, SEQCNT_EXEC, 0, 0, 0);
	set_seqcnt_state(&seqcnt_state_next, SEQCNT_EXEC, 0, 0, 0);
	set_test_if_get(TEST_STATE_STOP, TEST_OPTION_ONE, 123.456, -123.456);
	set_test_if_set(TEST_STATE_STOP, TEST_OPTION_ONE, 0.0, 0.0);

	set_event_data(0, ALL_AND, 
		(ADDRESS_T)&judge_data_base[0], (ADDRESS_T)&exec_data_base[0], 
		1, -1, (ADDRESS_T)&event_data_base[1]);
	set_event_data(1, ALL_AND, 
		(ADDRESS_T)&judge_data_base[1], (ADDRESS_T)&exec_data_base[1], 
		2, -1, 0);
	set_judge_data(0, INT, EQ, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_STOP, 0.0, 0, 1, 0);
	set_judge_data(1, INT, EQ, (ADDRESS_T)&test_if_get.option,
		TEST_OPTION_TWO, 0.0, 0, 1, 0);
	set_exec_data(0, INT, (ADDRESS_T)&test_if_set.mode, 
		TEST_STATE_EXEC, 0.0, 0);
	set_exec_data(1, INT, (ADDRESS_T)&test_if_set.option, 
		TEST_OPTION_TWO, 0.0, 0);

	seqcnt(&seqcnt_state, 
				&seqcnt_state_next, 
				&seq_id_data_base[0], 
				&seq_step_data_base[0]);

	EXPECT_EQ(seqcnt_state.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state.id, 0);
	EXPECT_EQ(seqcnt_state.step, 0);
	EXPECT_EQ(seqcnt_state.error, 0);
	EXPECT_EQ(seqcnt_state_next.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state_next.id, 0);
	EXPECT_EQ(seqcnt_state_next.step, 1);
	EXPECT_EQ(seqcnt_state_next.error, 0);
	EXPECT_EQ(test_if_set.mode, TEST_STATE_EXEC);
	EXPECT_EQ(test_if_set.option, TEST_OPTION_ONE);
	EXPECT_DOUBLE_EQ(test_if_set.param1, 0.0);
	EXPECT_DOUBLE_EQ(test_if_set.param2, 0.0);
}

/// \brief TEST seqcnt
///
/// てすと
TEST_F(SeqCtrlTest, SeqCtrlOnToOff1stEventError) {
	int i = 0;
	seq_id_data_base[0] = 0;
	seq_step_data_base[0] = &event_data_base[0];

	set_seqcnt_state(&seqcnt_state, SEQCNT_EXEC, 0, 0, 0);
	set_seqcnt_state(&seqcnt_state_next, SEQCNT_EXEC, 0, 0, 0);
	set_test_if_get(TEST_STATE_STOP, TEST_OPTION_ONE, 123.456, -123.456);
	set_test_if_set(TEST_STATE_STOP, TEST_OPTION_ONE, 0.0, 0.0);

	set_event_data(0, ALL_AND, 
		(ADDRESS_T)&judge_data_base[0], (ADDRESS_T)&exec_data_base[0], 
		1, -1, (ADDRESS_T)&event_data_base[1]);
	set_event_data(1, ALL_AND, 
		(ADDRESS_T)&judge_data_base[1], (ADDRESS_T)&exec_data_base[1], 
		2, -1, 0);
	set_judge_data(0, (var_type_t)-1, EQ, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_STOP, 0.0, 0, 1, 0);
	set_judge_data(1, INT, EQ, (ADDRESS_T)&test_if_get.option,
		TEST_OPTION_TWO, 0.0, 0, 1, 0);
	set_exec_data(0, INT, (ADDRESS_T)&test_if_set.mode, 
		TEST_STATE_EXEC, 0.0, 0);
	set_exec_data(1, INT, (ADDRESS_T)&test_if_set.option, 
		TEST_OPTION_TWO, 0.0, 0);

	seqcnt(&seqcnt_state, 
				&seqcnt_state_next, 
				&seq_id_data_base[0], 
				&seq_step_data_base[0]);

	EXPECT_EQ(seqcnt_state.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state.id, 0);
	EXPECT_EQ(seqcnt_state.step, 0);
	EXPECT_EQ(seqcnt_state.error, 0);
	EXPECT_EQ(seqcnt_state_next.mode, SEQCNT_STOP);
	EXPECT_EQ(seqcnt_state_next.id, 0);
	EXPECT_EQ(seqcnt_state_next.step, 0);
	EXPECT_EQ(seqcnt_state_next.error, 1);
	EXPECT_EQ(test_if_set.mode, TEST_STATE_STOP);
	EXPECT_EQ(test_if_set.option, TEST_OPTION_ONE);
	EXPECT_DOUBLE_EQ(test_if_set.param1, 0.0);
	EXPECT_DOUBLE_EQ(test_if_set.param2, 0.0);
}

/// \brief TEST seqcnt
///
/// てすと
TEST_F(SeqCtrlTest, SeqCtrlOn2ndEventTrue) {
	int i = 0;
	seq_id_data_base[0] = 0;
	seq_step_data_base[0] = &event_data_base[0];

	set_seqcnt_state(&seqcnt_state, SEQCNT_EXEC, 0, 0, 0);
	set_seqcnt_state(&seqcnt_state_next, SEQCNT_EXEC, 0, 0, 0);
	set_test_if_get(TEST_STATE_STOP, TEST_OPTION_ONE, 123.456, -123.456);
	set_test_if_set(TEST_STATE_STOP, TEST_OPTION_ONE, 0.0, 0.0);

	set_event_data(0, ALL_AND, 
		(ADDRESS_T)&judge_data_base[0], (ADDRESS_T)&exec_data_base[0], 
		1, -1, (ADDRESS_T)&event_data_base[1]);
	set_event_data(1, ALL_AND, 
		(ADDRESS_T)&judge_data_base[1], (ADDRESS_T)&exec_data_base[1], 
		2, -1, 0);
	set_judge_data(0, INT, EQ, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_EXEC, 0.0, 0, 1, 0);
	set_judge_data(1, INT, EQ, (ADDRESS_T)&test_if_get.option,
		TEST_OPTION_ONE, 0.0, 0, 1, 0);
	set_exec_data(0, INT, (ADDRESS_T)&test_if_set.mode, 
		TEST_STATE_EXEC, 0.0, 0);
	set_exec_data(1, INT, (ADDRESS_T)&test_if_set.option, 
		TEST_OPTION_TWO, 0.0, 0);

	seqcnt(&seqcnt_state, 
				&seqcnt_state_next, 
				&seq_id_data_base[0], 
				&seq_step_data_base[0]);

	EXPECT_EQ(seqcnt_state.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state.id, 0);
	EXPECT_EQ(seqcnt_state.step, 0);
	EXPECT_EQ(seqcnt_state.error, 0);
	EXPECT_EQ(seqcnt_state_next.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state_next.id, 0);
	EXPECT_EQ(seqcnt_state_next.step, 2);
	EXPECT_EQ(seqcnt_state_next.error, 0);
	EXPECT_EQ(test_if_set.mode, TEST_STATE_STOP);
	EXPECT_EQ(test_if_set.option, TEST_OPTION_TWO);
	EXPECT_DOUBLE_EQ(test_if_set.param1, 0.0);
	EXPECT_DOUBLE_EQ(test_if_set.param2, 0.0);
}

/// \brief TEST seqcnt
///
/// てすと
TEST_F(SeqCtrlTest, SeqCtrlOnAllEventFalse) {
	int i = 0;
	seq_id_data_base[0] = 0;
	seq_step_data_base[0] = &event_data_base[0];

	set_seqcnt_state(&seqcnt_state, SEQCNT_EXEC, 0, 0, 0);
	set_seqcnt_state(&seqcnt_state_next, SEQCNT_EXEC, 0, 0, 0);
	set_test_if_get(TEST_STATE_STOP, TEST_OPTION_ONE, 123.456, -123.456);
	set_test_if_set(TEST_STATE_STOP, TEST_OPTION_ONE, 0.0, 0.0);

	set_event_data(0, ALL_AND, 
		(ADDRESS_T)&judge_data_base[0], (ADDRESS_T)&exec_data_base[0], 
		1, -1, (ADDRESS_T)&event_data_base[1]);
	set_event_data(1, ALL_AND, 
		(ADDRESS_T)&judge_data_base[1], (ADDRESS_T)&exec_data_base[1], 
		2, -1, 0);
	set_judge_data(0, INT, EQ, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_EXEC, 0.0, 0, 1, 0);
	set_judge_data(1, INT, EQ, (ADDRESS_T)&test_if_get.option,
		TEST_OPTION_TWO, 0.0, 0, 1, 0);
	set_exec_data(0, INT, (ADDRESS_T)&test_if_set.mode, 
		TEST_STATE_EXEC, 0.0, 0);
	set_exec_data(1, INT, (ADDRESS_T)&test_if_set.option, 
		TEST_OPTION_TWO, 0.0, 0);

	seqcnt(&seqcnt_state, 
				&seqcnt_state_next, 
				&seq_id_data_base[0], 
				&seq_step_data_base[0]);

	EXPECT_EQ(seqcnt_state.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state.id, 0);
	EXPECT_EQ(seqcnt_state.step, 0);
	EXPECT_EQ(seqcnt_state.error, 0);
	EXPECT_EQ(seqcnt_state_next.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state_next.id, 0);
	EXPECT_EQ(seqcnt_state_next.step, 0);
	EXPECT_EQ(seqcnt_state_next.error, 0);
	EXPECT_EQ(test_if_set.mode, TEST_STATE_STOP);
	EXPECT_EQ(test_if_set.option, TEST_OPTION_ONE);
	EXPECT_DOUBLE_EQ(test_if_set.param1, 0.0);
	EXPECT_DOUBLE_EQ(test_if_set.param2, 0.0);
}

/// \brief TEST seqcnt
///
/// てすと
TEST_F(SeqCtrlTest, SeqCtrlOnExceedEventMax) {
	int i = 0;
	seq_id_data_base[0] = 0;
	seq_step_data_base[0] = &event_data_base[0];

	set_seqcnt_state(&seqcnt_state, SEQCNT_EXEC, 0, 0, 0);
	set_seqcnt_state(&seqcnt_state_next, SEQCNT_EXEC, 0, 0, 0);
	set_test_if_get(TEST_STATE_STOP, TEST_OPTION_ONE, 123.456, -123.456);
	set_test_if_set(TEST_STATE_STOP, TEST_OPTION_ONE, 0.0, 0.0);

	for (i=0; i<EVENT_MAX; i++) {
		set_event_data(i, ALL_AND, 
			(ADDRESS_T)&judge_data_base[0], 0,
			1, -1, (ADDRESS_T)&event_data_base[i+1]);
	}
	set_judge_data(0, INT, EQ, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_EXEC, 0.0, 0, 1, 0);

	seqcnt(&seqcnt_state, 
				&seqcnt_state_next, 
				&seq_id_data_base[0], 
				&seq_step_data_base[0]);

	EXPECT_EQ(seqcnt_state.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state.id, 0);
	EXPECT_EQ(seqcnt_state.step, 0);
	EXPECT_EQ(seqcnt_state.error, 0);
	EXPECT_EQ(seqcnt_state_next.mode, SEQCNT_STOP);
	EXPECT_EQ(seqcnt_state_next.id, 0);
	EXPECT_EQ(seqcnt_state_next.step, 0);
	EXPECT_EQ(seqcnt_state_next.error, 1);
	EXPECT_EQ(test_if_set.mode, TEST_STATE_STOP);
	EXPECT_EQ(test_if_set.option, TEST_OPTION_ONE);
	EXPECT_DOUBLE_EQ(test_if_set.param1, 0.0);
	EXPECT_DOUBLE_EQ(test_if_set.param2, 0.0);
}

/// \brief TEST seqcnt
///
/// てすと
TEST_F(SeqCtrlTest, SeqCtrlOnChangeSeqId) {
	int i = 0;
	seq_id_data_base[0] = 0;
	seq_step_data_base[0] = &event_data_base[0];

	set_event_data(0, ALL_AND, 0, 0, -1, 1, 0);

	set_seqcnt_state(&seqcnt_state, SEQCNT_EXEC, 0, 0, 0);
	set_seqcnt_state(&seqcnt_state_next, SEQCNT_EXEC, 0, 0, 0);

	seqcnt(&seqcnt_state, 
				&seqcnt_state_next, 
				&seq_id_data_base[0], 
				&seq_step_data_base[0]);

	EXPECT_EQ(seqcnt_state.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state.id, 0);
	EXPECT_EQ(seqcnt_state.step, 0);
	EXPECT_EQ(seqcnt_state.error, 0);
	EXPECT_EQ(seqcnt_state_next.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state_next.id, 1);
	EXPECT_EQ(seqcnt_state_next.step, 0);
	EXPECT_EQ(seqcnt_state_next.error, 0);
}

/// \brief TEST seqcnt
///
/// てすと
TEST_F(SeqCtrlTest, SeqCtrlOnToOnResetCountFalse) {
	int i = 0;
	seq_id_data_base[0] = 0;
	seq_step_data_base[0] = &event_data_base[0];

	set_event_data(0, ALL_AND, 0, 0, 1, -1, 0);

	set_seqcnt_state(&seqcnt_state, SEQCNT_EXEC, 0, 0, 0);
	set_seqcnt_state(&seqcnt_state_next, SEQCNT_EXEC, 0, 0, 0);

	seqcnt(&seqcnt_state, 
				&seqcnt_state_next, 
				&seq_id_data_base[0], 
				&seq_step_data_base[0]);

	EXPECT_EQ(seqcnt_state.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state.id, 0);
	EXPECT_EQ(seqcnt_state.step, 0);
	EXPECT_EQ(seqcnt_state.error, 0);
	EXPECT_EQ(seqcnt_state_next.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state_next.id, 0);
	EXPECT_EQ(seqcnt_state_next.step, 1);
	EXPECT_EQ(seqcnt_state_next.error, 0);
}

/// \brief TEST seqcnt
///
/// てすと
TEST_F(SeqCtrlTest, SeqCtrlOnToOffNoNextStepAndId) {
	int i = 0;
	seq_id_data_base[0] = 0;
	seq_step_data_base[0] = &event_data_base[0];

	set_event_data(0, ALL_AND, 0, 0, -1, -1, 0);

	set_seqcnt_state(&seqcnt_state, SEQCNT_EXEC, 0, 0, 0);
	set_seqcnt_state(&seqcnt_state_next, SEQCNT_EXEC, 0, 0, 0);

	seqcnt(&seqcnt_state, 
				&seqcnt_state_next, 
				&seq_id_data_base[0], 
				&seq_step_data_base[0]);

	EXPECT_EQ(seqcnt_state.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state.id, 0);
	EXPECT_EQ(seqcnt_state.step, 0);
	EXPECT_EQ(seqcnt_state.error, 0);
	EXPECT_EQ(seqcnt_state_next.mode, SEQCNT_STOP);
	EXPECT_EQ(seqcnt_state_next.id, 0);
	EXPECT_EQ(seqcnt_state_next.step, 0);
	EXPECT_EQ(seqcnt_state_next.error, 0);
}

/// \brief TEST seqcnt
///
/// てすと
TEST_F(SeqCtrlTest, SeqCtrlOnToOffJudgeError) {
	int i = 0;
	seq_id_data_base[0] = 0;
	seq_step_data_base[0] = &event_data_base[0];

	set_seqcnt_state(&seqcnt_state, SEQCNT_EXEC, 0, 0, 0);
	set_seqcnt_state(&seqcnt_state_next, SEQCNT_EXEC, 0, 0, 0);
	set_test_if_get(TEST_STATE_STOP, TEST_OPTION_ONE, 123.456, -123.456);
	set_test_if_set(TEST_STATE_STOP, TEST_OPTION_ONE, 0.0, 0.0);

	set_event_data(0, ALL_AND, 
		(ADDRESS_T)&judge_data_base[0], (ADDRESS_T)&exec_data_base[0], 
		1, -1, 0);
	set_judge_data(0, (var_type_t)-1, EQ, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_STOP, 0.0, 0, 1, 0);
	set_exec_data(0, INT, (ADDRESS_T)&test_if_set.mode, 
		TEST_STATE_EXEC, 0.0, 0);

	seqcnt(&seqcnt_state, 
				&seqcnt_state_next, 
				&seq_id_data_base[0], 
				&seq_step_data_base[0]);

	EXPECT_EQ(seqcnt_state.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state.id, 0);
	EXPECT_EQ(seqcnt_state.step, 0);
	EXPECT_EQ(seqcnt_state.error, 0);
	EXPECT_EQ(seqcnt_state_next.mode, SEQCNT_STOP);
	EXPECT_EQ(seqcnt_state_next.id, 0);
	EXPECT_EQ(seqcnt_state_next.step, 0);
	EXPECT_EQ(seqcnt_state_next.error, 1);
	EXPECT_EQ(test_if_set.mode, TEST_STATE_STOP);
	EXPECT_EQ(test_if_set.option, TEST_OPTION_ONE);
	EXPECT_DOUBLE_EQ(test_if_set.param1, 0.0);
	EXPECT_DOUBLE_EQ(test_if_set.param2, 0.0);
}

/// \brief TEST seqcnt
///
/// てすと
TEST_F(SeqCtrlTest, SeqCtrlOnToOffExecError) {
	int i = 0;
	seq_id_data_base[0] = 0;
	seq_step_data_base[0] = &event_data_base[0];

	set_seqcnt_state(&seqcnt_state, SEQCNT_EXEC, 0, 0, 0);
	set_seqcnt_state(&seqcnt_state_next, SEQCNT_EXEC, 0, 0, 0);
	set_test_if_get(TEST_STATE_STOP, TEST_OPTION_ONE, 123.456, -123.456);
	set_test_if_set(TEST_STATE_STOP, TEST_OPTION_ONE, 0.0, 0.0);

	set_event_data(0, ALL_AND, 
		(ADDRESS_T)&judge_data_base[0], (ADDRESS_T)&exec_data_base[0], 
		1, -1, 0);
	set_judge_data(0, INT, EQ, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_STOP, 0.0, 0, 1, 0);
	set_exec_data(0, (var_type_t)-1, (ADDRESS_T)&test_if_set.mode, 
		TEST_STATE_EXEC, 0.0, 0);

	seqcnt(&seqcnt_state, 
				&seqcnt_state_next, 
				&seq_id_data_base[0], 
				&seq_step_data_base[0]);

	EXPECT_EQ(seqcnt_state.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state.id, 0);
	EXPECT_EQ(seqcnt_state.step, 0);
	EXPECT_EQ(seqcnt_state.error, 0);
	EXPECT_EQ(seqcnt_state_next.mode, SEQCNT_STOP);
	EXPECT_EQ(seqcnt_state_next.id, 0);
	EXPECT_EQ(seqcnt_state_next.step, 0);
	EXPECT_EQ(seqcnt_state_next.error, 1);
	EXPECT_EQ(test_if_set.mode, TEST_STATE_STOP);
	EXPECT_EQ(test_if_set.option, TEST_OPTION_ONE);
	EXPECT_DOUBLE_EQ(test_if_set.param1, 0.0);
	EXPECT_DOUBLE_EQ(test_if_set.param2, 0.0);
}

/// \brief TEST seqcnt
///
/// てすと
TEST_F(SeqCtrlTest, SeqCtrlOnToOffFreeTimerError) {
	int i = 0;
	seq_id_data_base[0] = 0;
	seq_step_data_base[0] = &event_data_base[0];

	free_timer = INT32_MAX;
	seq_timer = 1;
	step_timer = 2;
	set_seqcnt_state(&seqcnt_state, SEQCNT_EXEC, 0, 0, 0);
	set_seqcnt_state(&seqcnt_state_next, SEQCNT_EXEC, 0, 0, 0);
	set_test_if_get(TEST_STATE_STOP, TEST_OPTION_ONE, 123.456, -123.456);
	set_test_if_set(TEST_STATE_STOP, TEST_OPTION_ONE, 0.0, 0.0);

	set_event_data(0, ALL_AND, 
		(ADDRESS_T)&judge_data_base[0], (ADDRESS_T)&exec_data_base[0], 
		1, -1, 0);
	set_judge_data(0, INT, EQ, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_EXEC, 0.0, 0, 1, 0);
	set_exec_data(0, INT, (ADDRESS_T)&test_if_set.mode, 
		TEST_STATE_EXEC, 0.0, 0);

	seqcnt(&seqcnt_state, 
				&seqcnt_state_next, 
				&seq_id_data_base[0], 
				&seq_step_data_base[0]);

	EXPECT_EQ(seqcnt_state.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state.id, 0);
	EXPECT_EQ(seqcnt_state.step, 0);
	EXPECT_EQ(seqcnt_state.error, 0);
	EXPECT_EQ(seqcnt_state_next.mode, SEQCNT_STOP);
	EXPECT_EQ(seqcnt_state_next.id, 0);
	EXPECT_EQ(seqcnt_state_next.step, 0);
	EXPECT_EQ(seqcnt_state_next.error, 1);
	EXPECT_EQ(test_if_set.mode, TEST_STATE_STOP);
	EXPECT_EQ(test_if_set.option, TEST_OPTION_ONE);
	EXPECT_DOUBLE_EQ(test_if_set.param1, 0.0);
	EXPECT_DOUBLE_EQ(test_if_set.param2, 0.0);
	EXPECT_EQ(free_timer, INT32_MAX);
	EXPECT_EQ(seq_timer, 1);
	EXPECT_EQ(step_timer, 2);
}

/// \brief TEST seqcnt
///
/// てすと
TEST_F(SeqCtrlTest, SeqCtrlOnToOffSeqTimerError) {
	int i = 0;
	seq_id_data_base[0] = 0;
	seq_step_data_base[0] = &event_data_base[0];

	free_timer = 0;
	seq_timer = INT32_MAX;
	step_timer = 2;
	set_seqcnt_state(&seqcnt_state, SEQCNT_EXEC, 0, 0, 0);
	set_seqcnt_state(&seqcnt_state_next, SEQCNT_EXEC, 0, 0, 0);
	set_test_if_get(TEST_STATE_STOP, TEST_OPTION_ONE, 123.456, -123.456);
	set_test_if_set(TEST_STATE_STOP, TEST_OPTION_ONE, 0.0, 0.0);

	set_event_data(0, ALL_AND, 
		(ADDRESS_T)&judge_data_base[0], (ADDRESS_T)&exec_data_base[0], 
		1, -1, 0);
	set_judge_data(0, INT, EQ, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_EXEC, 0.0, 0, 1, 0);
	set_exec_data(0, INT, (ADDRESS_T)&test_if_set.mode, 
		TEST_STATE_EXEC, 0.0, 0);

	seqcnt(&seqcnt_state, 
				&seqcnt_state_next, 
				&seq_id_data_base[0], 
				&seq_step_data_base[0]);

	EXPECT_EQ(seqcnt_state.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state.id, 0);
	EXPECT_EQ(seqcnt_state.step, 0);
	EXPECT_EQ(seqcnt_state.error, 0);
	EXPECT_EQ(seqcnt_state_next.mode, SEQCNT_STOP);
	EXPECT_EQ(seqcnt_state_next.id, 0);
	EXPECT_EQ(seqcnt_state_next.step, 0);
	EXPECT_EQ(seqcnt_state_next.error, 1);
	EXPECT_EQ(test_if_set.mode, TEST_STATE_STOP);
	EXPECT_EQ(test_if_set.option, TEST_OPTION_ONE);
	EXPECT_DOUBLE_EQ(test_if_set.param1, 0.0);
	EXPECT_DOUBLE_EQ(test_if_set.param2, 0.0);
	EXPECT_EQ(free_timer, 0);
	EXPECT_EQ(seq_timer, INT32_MAX);
	EXPECT_EQ(step_timer, 2);
}

/// \brief TEST seqcnt
///
/// てすと
TEST_F(SeqCtrlTest, SeqCtrlOnToOffStepTimerError) {
	int i = 0;
	seq_id_data_base[0] = 0;
	seq_step_data_base[0] = &event_data_base[0];

	free_timer = 0;
	seq_timer = 1;
	step_timer = INT32_MAX;
	set_seqcnt_state(&seqcnt_state, SEQCNT_EXEC, 0, 0, 0);
	set_seqcnt_state(&seqcnt_state_next, SEQCNT_EXEC, 0, 0, 0);
	set_test_if_get(TEST_STATE_STOP, TEST_OPTION_ONE, 123.456, -123.456);
	set_test_if_set(TEST_STATE_STOP, TEST_OPTION_ONE, 0.0, 0.0);

	set_event_data(0, ALL_AND, 
		(ADDRESS_T)&judge_data_base[0], (ADDRESS_T)&exec_data_base[0], 
		1, -1, 0);
	set_judge_data(0, INT, EQ, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_EXEC, 0.0, 0, 1, 0);
	set_exec_data(0, INT, (ADDRESS_T)&test_if_set.mode, 
		TEST_STATE_EXEC, 0.0, 0);

	seqcnt(&seqcnt_state, 
				&seqcnt_state_next, 
				&seq_id_data_base[0], 
				&seq_step_data_base[0]);

	EXPECT_EQ(seqcnt_state.mode, SEQCNT_EXEC);
	EXPECT_EQ(seqcnt_state.id, 0);
	EXPECT_EQ(seqcnt_state.step, 0);
	EXPECT_EQ(seqcnt_state.error, 0);
	EXPECT_EQ(seqcnt_state_next.mode, SEQCNT_STOP);
	EXPECT_EQ(seqcnt_state_next.id, 0);
	EXPECT_EQ(seqcnt_state_next.step, 0);
	EXPECT_EQ(seqcnt_state_next.error, 1);
	EXPECT_EQ(test_if_set.mode, TEST_STATE_STOP);
	EXPECT_EQ(test_if_set.option, TEST_OPTION_ONE);
	EXPECT_DOUBLE_EQ(test_if_set.param1, 0.0);
	EXPECT_DOUBLE_EQ(test_if_set.param2, 0.0);
	EXPECT_EQ(free_timer, 0);
	EXPECT_EQ(seq_timer, 1);
	EXPECT_EQ(step_timer, INT32_MAX);
}

/// \brief TEST seqcnt
///
/// てすと
TEST_F(SeqCtrlTest, SeqCtrlOff) {
	set_seqcnt_state(&seqcnt_state, SEQCNT_STOP, 0, 0, 0);
	set_seqcnt_state(&seqcnt_state_next, SEQCNT_STOP, 0, 0, 0);

	seqcnt(&seqcnt_state, 
				&seqcnt_state_next, 
				&seq_id_data_base[0], 
				&seq_step_data_base[0]);

	EXPECT_EQ(seqcnt_state.mode, SEQCNT_STOP);
	EXPECT_EQ(seqcnt_state.id, 0);
	EXPECT_EQ(seqcnt_state.step, 0);
	EXPECT_EQ(seqcnt_state.error, 0);
	EXPECT_EQ(seqcnt_state_next.mode, SEQCNT_STOP);
	EXPECT_EQ(seqcnt_state_next.id, 0);
	EXPECT_EQ(seqcnt_state_next.step, 0);
	EXPECT_EQ(seqcnt_state_next.error, 0);
}

/// \brief TEST event_judge
///
/// いべんと
TEST_F(SeqCtrlTest, EventJudgeIsNormalAllAndOK_EQ) {

	int _ret = 0;
	bool _rslt = false;

	set_test_if_get(TEST_STATE_STOP, TEST_OPTION_ONE, 123.456, -123.456);
	set_judge_data(0, INT, EQ, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_STOP, 0.0, 0, 1, (ADDRESS_T)&judge_data_base[1]);
	set_judge_data(1, INT, EQ, (ADDRESS_T)&test_if_get.option,
		TEST_OPTION_ONE, 0.0, 0, 1, (ADDRESS_T)&judge_data_base[2]);
	set_judge_data(2, DBL, EQ, (ADDRESS_T)&test_if_get.param1,
		0, 123.456, 0, 1, (ADDRESS_T)&judge_data_base[3]);
	set_judge_data(3, DBL, EQ, (ADDRESS_T)&test_if_get.param2,
		0, -123.456, 0, 1, 0);

	_ret = event_judge((ADDRESS_T)&judge_data_base[0], ALL_AND, false, &_rslt);

	EXPECT_EQ(_ret, 0);
	EXPECT_TRUE(_rslt);
}

/// \brief TEST event_judge
///
/// いべんと
TEST_F(SeqCtrlTest, EventJudgeIsNormalAllOrNG_EQ) {

	int _ret = 0;
	bool _rslt = false;

	set_test_if_get(TEST_STATE_STOP, TEST_OPTION_ONE, 123.456, -123.456);
	set_judge_data(0, INT, EQ, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_EXEC, 0.0, 0, 1, (ADDRESS_T)&judge_data_base[1]);
	set_judge_data(1, INT, EQ, (ADDRESS_T)&test_if_get.option,
		TEST_OPTION_TWO, 0.0, 0, 1, (ADDRESS_T)&judge_data_base[2]);
	set_judge_data(2, DBL, EQ, (ADDRESS_T)&test_if_get.param1,
		0, 123.457, 0, 1, (ADDRESS_T)&judge_data_base[3]);
	set_judge_data(3, DBL, EQ, (ADDRESS_T)&test_if_get.param2,
		0, -123.457, 0, 1, 0);

	_ret = event_judge((ADDRESS_T)&judge_data_base[0], ALL_OR, false, &_rslt);

	EXPECT_EQ(_ret, 0);
	EXPECT_FALSE(_rslt);
}

/// \brief TEST event_judge
///
/// いべんと
TEST_F(SeqCtrlTest, EventJudgeIsNormalAllAndOK_NEQ) {

	int _ret = 0;
	bool _rslt = false;

	set_test_if_get(TEST_STATE_STOP, TEST_OPTION_ONE, 123.456, -123.456);
	set_judge_data(0, INT, NEQ, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_EXEC, 0.0, 0, 1, (ADDRESS_T)&judge_data_base[1]);
	set_judge_data(1, INT, NEQ, (ADDRESS_T)&test_if_get.option,
		TEST_OPTION_TWO, 0.0, 0, 1, (ADDRESS_T)&judge_data_base[2]);
	set_judge_data(2, DBL, NEQ, (ADDRESS_T)&test_if_get.param1,
		0, 123.457, 0, 1, (ADDRESS_T)&judge_data_base[3]);
	set_judge_data(3, DBL, NEQ, (ADDRESS_T)&test_if_get.param2,
		0, -123.457, 0, 1, 0);

	_ret = event_judge((ADDRESS_T)&judge_data_base[0], ALL_AND, false, &_rslt);

	EXPECT_EQ(_ret, 0);
	EXPECT_TRUE(_rslt);
}

/// \brief TEST event_judge
///
/// いべんと
TEST_F(SeqCtrlTest, EventJudgeIsNormalAllOrNG_NEQ) {

	int _ret = 0;
	bool _rslt = false;

	set_test_if_get(TEST_STATE_STOP, TEST_OPTION_ONE, 123.456, -123.456);
	set_judge_data(0, INT, NEQ, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_STOP, 0.0, 0, 1, (ADDRESS_T)&judge_data_base[1]);
	set_judge_data(1, INT, NEQ, (ADDRESS_T)&test_if_get.option,
		TEST_OPTION_ONE, 0.0, 0, 1, (ADDRESS_T)&judge_data_base[2]);
	set_judge_data(2, DBL, NEQ, (ADDRESS_T)&test_if_get.param1,
		0, 123.456, 0, 1, (ADDRESS_T)&judge_data_base[3]);
	set_judge_data(3, DBL, NEQ, (ADDRESS_T)&test_if_get.param2,
		0, -123.456, 0, 1, 0);

	_ret = event_judge((ADDRESS_T)&judge_data_base[0], ALL_OR, false, &_rslt);

	EXPECT_EQ(_ret, 0);
	EXPECT_FALSE(_rslt);
}



/// \brief TEST event_judge
///
/// いべんと
TEST_F(SeqCtrlTest, EventJudgeIsNormalAllAndOK_LT) {

	int _ret = 0;
	bool _rslt = false;

	set_test_if_get(TEST_STATE_STOP, TEST_OPTION_ONE, 123.456, -123.456);
	set_judge_data(0, INT, LT, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_EXEC, 0.0, 0, 1, (ADDRESS_T)&judge_data_base[1]);
	set_judge_data(1, INT, LT, (ADDRESS_T)&test_if_get.option,
		TEST_OPTION_TWO, 0.0, 0, 1, (ADDRESS_T)&judge_data_base[2]);
	set_judge_data(2, DBL, LT, (ADDRESS_T)&test_if_get.param1,
		0, 123.457, 0, 1, (ADDRESS_T)&judge_data_base[3]);
	set_judge_data(3, DBL, LT, (ADDRESS_T)&test_if_get.param2,
		0, -123.455, 0, 1, 0);

	_ret = event_judge((ADDRESS_T)&judge_data_base[0], ALL_AND, false, &_rslt);

	EXPECT_EQ(_ret, 0);
	EXPECT_TRUE(_rslt);
}

/// \brief TEST event_judge
///
/// いべんと
TEST_F(SeqCtrlTest, EventJudgeIsNormalAllOrNG_LT) {

	int _ret = 0;
	bool _rslt = false;

	set_test_if_get(TEST_STATE_EXEC, TEST_OPTION_TWO, 123.456, -123.456);
	set_judge_data(0, INT, LT, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_EXEC, 0.0, 0, 1, (ADDRESS_T)&judge_data_base[1]);
	set_judge_data(1, INT, LT, (ADDRESS_T)&test_if_get.option,
		TEST_OPTION_TWO, 0.0, 0, 1, (ADDRESS_T)&judge_data_base[2]);
	set_judge_data(2, DBL, LT, (ADDRESS_T)&test_if_get.param1,
		0, 123.456, 0, 1, (ADDRESS_T)&judge_data_base[3]);
	set_judge_data(3, DBL, LT, (ADDRESS_T)&test_if_get.param2,
		0, -123.456, 0, 1, 0);

	_ret = event_judge((ADDRESS_T)&judge_data_base[0], ALL_OR, false, &_rslt);

	EXPECT_EQ(_ret, 0);
	EXPECT_FALSE(_rslt);
}

/// \brief TEST event_judge
///
/// いべんと
TEST_F(SeqCtrlTest, EventJudgeIsNormalAllAndOK_LTE) {

	int _ret = 0;
	bool _rslt = false;

	set_test_if_get(TEST_STATE_STOP, TEST_OPTION_TWO, 123.456, -123.456);
	set_judge_data(0, INT, LTE, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_EXEC, 0.0, 0, 1, (ADDRESS_T)&judge_data_base[1]);
	set_judge_data(1, INT, LTE, (ADDRESS_T)&test_if_get.option,
		TEST_OPTION_TWO, 0.0, 0, 1, (ADDRESS_T)&judge_data_base[2]);
	set_judge_data(2, DBL, LTE, (ADDRESS_T)&test_if_get.param1,
		0, 123.457, 0, 1, (ADDRESS_T)&judge_data_base[3]);
	set_judge_data(3, DBL, LTE, (ADDRESS_T)&test_if_get.param2,
		0, -123.456, 0, 1, 0);

	_ret = event_judge((ADDRESS_T)&judge_data_base[0], ALL_AND, false, &_rslt);

	EXPECT_EQ(_ret, 0);
	EXPECT_TRUE(_rslt);
}

/// \brief TEST event_judge
///
/// いべんと
TEST_F(SeqCtrlTest, EventJudgeIsNormalAllOrNG_LTE) {

	int _ret = 0;
	bool _rslt = false;

	set_test_if_get(TEST_STATE_EXEC, TEST_OPTION_TWO, 123.456, -123.456);
	set_judge_data(0, INT, LTE, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_STOP, 0.0, 0, 1, (ADDRESS_T)&judge_data_base[1]);
	set_judge_data(1, INT, LTE, (ADDRESS_T)&test_if_get.option,
		TEST_OPTION_ONE, 0.0, 0, 1, (ADDRESS_T)&judge_data_base[2]);
	set_judge_data(2, DBL, LTE, (ADDRESS_T)&test_if_get.param1,
		0, 123.455, 0, 1, (ADDRESS_T)&judge_data_base[3]);
	set_judge_data(3, DBL, LTE, (ADDRESS_T)&test_if_get.param2,
		0, -123.457, 0, 1, 0);

	_ret = event_judge((ADDRESS_T)&judge_data_base[0], ALL_OR, false, &_rslt);

	EXPECT_EQ(_ret, 0);
	EXPECT_FALSE(_rslt);
}

/// \brief TEST event_judge
///
/// いべんと
TEST_F(SeqCtrlTest, EventJudgeIsNormalAllAndOK_GT) {

	int _ret = 0;
	bool _rslt = false;

	set_test_if_get(TEST_STATE_EXEC, TEST_OPTION_TWO, 123.456, -123.456);
	set_judge_data(0, INT, GT, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_STOP, 0.0, 0, 1, (ADDRESS_T)&judge_data_base[1]);
	set_judge_data(1, INT, GT, (ADDRESS_T)&test_if_get.option,
		TEST_OPTION_ONE, 0.0, 0, 1, (ADDRESS_T)&judge_data_base[2]);
	set_judge_data(2, DBL, GT, (ADDRESS_T)&test_if_get.param1,
		0, 123.455, 0, 1, (ADDRESS_T)&judge_data_base[3]);
	set_judge_data(3, DBL, GT, (ADDRESS_T)&test_if_get.param2,
		0, -123.457, 0, 1, 0);

	_ret = event_judge((ADDRESS_T)&judge_data_base[0], ALL_AND, false, &_rslt);

	EXPECT_EQ(_ret, 0);
	EXPECT_TRUE(_rslt);
}

/// \brief TEST event_judge
///
/// いべんと
TEST_F(SeqCtrlTest, EventJudgeIsNormalAllOrNG_GT) {

	int _ret = 0;
	bool _rslt = false;

	set_test_if_get(TEST_STATE_STOP, TEST_OPTION_ONE, 123.456, -123.456);
	set_judge_data(0, INT, GT, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_STOP, 0.0, 0, 1, (ADDRESS_T)&judge_data_base[1]);
	set_judge_data(1, INT, GT, (ADDRESS_T)&test_if_get.option,
		TEST_OPTION_ONE, 0.0, 0, 1, (ADDRESS_T)&judge_data_base[2]);
	set_judge_data(2, DBL, GT, (ADDRESS_T)&test_if_get.param1,
		0, 123.457, 0, 1, (ADDRESS_T)&judge_data_base[3]);
	set_judge_data(3, DBL, GT, (ADDRESS_T)&test_if_get.param2,
		0, -123.456, 0, 1, 0);

	_ret = event_judge((ADDRESS_T)&judge_data_base[0], ALL_OR, false, &_rslt);

	EXPECT_EQ(_ret, 0);
	EXPECT_FALSE(_rslt);
}

/// \brief TEST event_judge
///
/// いべんと
TEST_F(SeqCtrlTest, EventJudgeIsNormalAllAndOK_GTE) {

	int _ret = 0;
	bool _rslt = false;

	set_test_if_get(TEST_STATE_EXEC, TEST_OPTION_ONE, 123.456, -123.456);
	set_judge_data(0, INT, GTE, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_STOP, 0.0, 0, 1, (ADDRESS_T)&judge_data_base[1]);
	set_judge_data(1, INT, GTE, (ADDRESS_T)&test_if_get.option,
		TEST_OPTION_ONE, 0.0, 0, 1, (ADDRESS_T)&judge_data_base[2]);
	set_judge_data(2, DBL, GTE, (ADDRESS_T)&test_if_get.param1,
		0, 123.455, 0, 1, (ADDRESS_T)&judge_data_base[3]);
	set_judge_data(3, DBL, GTE, (ADDRESS_T)&test_if_get.param2,
		0, -123.456, 0, 1, 0);

	_ret = event_judge((ADDRESS_T)&judge_data_base[0], ALL_AND, false, &_rslt);

	EXPECT_EQ(_ret, 0);
	EXPECT_TRUE(_rslt);
}

/// \brief TEST event_judge
///
/// いべんと
TEST_F(SeqCtrlTest, EventJudgeIsNormalAllOrNG_GTE) {

	int _ret = 0;
	bool _rslt = false;

	set_test_if_get(TEST_STATE_STOP, TEST_OPTION_ONE, 123.456, -123.456);
	set_judge_data(0, INT, GTE, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_EXEC, 0.0, 0, 1, (ADDRESS_T)&judge_data_base[1]);
	set_judge_data(1, INT, GTE, (ADDRESS_T)&test_if_get.option,
		TEST_OPTION_TWO, 0.0, 0, 1, (ADDRESS_T)&judge_data_base[2]);
	set_judge_data(2, DBL, GTE, (ADDRESS_T)&test_if_get.param1,
		0, 123.457, 0, 1, (ADDRESS_T)&judge_data_base[3]);
	set_judge_data(3, DBL, GTE, (ADDRESS_T)&test_if_get.param2,
		0, -123.455, 0, 1, 0);

	_ret = event_judge((ADDRESS_T)&judge_data_base[0], ALL_OR, false, &_rslt);

	EXPECT_EQ(_ret, 0);
	EXPECT_FALSE(_rslt);
}

/// \brief TEST event_judge
///
/// いべんと
TEST_F(SeqCtrlTest, EventJudgeIsNormalAllOrOK) {

	int _ret = 0;
	bool _rslt = false;

	set_test_if_get(TEST_STATE_STOP, TEST_OPTION_ONE, 123.456, -123.456);
	set_judge_data(0, DBL, EQ, (ADDRESS_T)&test_if_get.param1, 
		0, 123.457, 0, 1, (ADDRESS_T)&judge_data_base[1]);
	set_judge_data(1, INT, EQ, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_STOP, 0.0, 0, 1, 0);

	_ret = event_judge((ADDRESS_T)&judge_data_base[0], ALL_OR, false, &_rslt);

	EXPECT_EQ(_ret, 0);
	EXPECT_TRUE(_rslt);
}

/// \brief TEST event_judge
///
/// いべんと
TEST_F(SeqCtrlTest, EventJudgeIsNormalAllAndNG) {

	int _ret = 0;
	bool _rslt = false;

	set_test_if_get(TEST_STATE_STOP, TEST_OPTION_ONE, 123.456, -123.456);
	set_judge_data(0, DBL, EQ, (ADDRESS_T)&test_if_get.param1,
		0, 123.457, 0, 1, (ADDRESS_T)&judge_data_base[1]);
	set_judge_data(1, INT, EQ, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_STOP, 0.0, 0, 1, 0);

	_ret = event_judge((ADDRESS_T)&judge_data_base[0], ALL_AND, false, &_rslt);

	EXPECT_EQ(_ret, 0);
	EXPECT_FALSE(_rslt);
}

/// \brief TEST event_judge
///
/// いべんと
TEST_F(SeqCtrlTest, EventJudgeIsNormalCount) {

	int i = 0;
	int _ret[3] = {0};
	bool _rslt[3] = {false};

	set_test_if_get(TEST_STATE_STOP, TEST_OPTION_ONE, 123.456, -123.456);
	set_judge_data(0, DBL, EQ, (ADDRESS_T)&test_if_get.param1, 
		0, 123.456, 0, 2, (ADDRESS_T)&judge_data_base[1]);
	set_judge_data(1, INT, EQ, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_STOP, 0.0, 0, 3, 0);

	for (i=0; i<3; i++) {
		_ret[i] = event_judge((ADDRESS_T)&judge_data_base[0], ALL_AND, false, &_rslt[i]);
	}

	EXPECT_EQ(_ret[0], 0);
	EXPECT_EQ(_ret[1], 0);
	EXPECT_EQ(_ret[2], 0);
	EXPECT_FALSE(_rslt[0]);
	EXPECT_FALSE(_rslt[1]);
	EXPECT_TRUE(_rslt[2]);
}

/// \brief TEST event_judge
///
/// いべんと
TEST_F(SeqCtrlTest, EventJudgeIsNormalResetCount) {

	int i = 0;
	int _ret[3] = {0};
	bool _rslt[3] = {false};
	bool _rst[3] = {true, false, false};

	set_test_if_get(TEST_STATE_STOP, TEST_OPTION_ONE, 123.456, -123.456);
	set_judge_data(0, DBL, EQ, (ADDRESS_T)&test_if_get.param1, 
		0, 123.456, 0, 1, (ADDRESS_T)&judge_data_base[1]);
	set_judge_data(1, INT, EQ, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_STOP, 0.0, 1, 3, 0);

	for (i=0; i<3; i++) {
		_ret[i] = event_judge((ADDRESS_T)&judge_data_base[0], ALL_AND, _rst[i], &_rslt[i]);
	}

	EXPECT_EQ(_ret[0], 0);
	EXPECT_EQ(_ret[1], 0);
	EXPECT_EQ(_ret[2], 0);
	EXPECT_FALSE(_rslt[0]);
	EXPECT_FALSE(_rslt[1]);
	EXPECT_TRUE(_rslt[2]);
}

/// \brief TEST event_judge
///
/// いべんと
TEST_F(SeqCtrlTest, EventJudgeIsErrorType) {

	int _ret = 0;
	bool _rslt = false;

	set_test_if_get(TEST_STATE_STOP, TEST_OPTION_ONE, 123.456, -123.456);
	set_judge_data(0, (var_type_t)-1, EQ, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_EXEC, 0.0, 0, 1, 0);

	_ret = event_judge((ADDRESS_T)&judge_data_base[0], ALL_OR, false, &_rslt);

	EXPECT_EQ(_ret, 1);
	EXPECT_FALSE(_rslt);
}

/// \brief TEST event_judge
///
/// いべんと
TEST_F(SeqCtrlTest, EventJudgeIsErrorIntOpe) {

	int _ret = 0;
	bool _rslt = false;

	set_test_if_get(TEST_STATE_STOP, TEST_OPTION_ONE, 123.456, -123.456);
	set_judge_data(0, INT, (judge_ope_t)-1, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_STOP, 0.0, 0, 1, 0);

	_ret = event_judge((ADDRESS_T)&judge_data_base[0], ALL_OR, false, &_rslt);

	EXPECT_EQ(_ret, 1);
	EXPECT_FALSE(_rslt);
}

/// \brief TEST event_judge
///
/// いべんと
TEST_F(SeqCtrlTest, EventJudgeIsErrorDblOpe) {

	int _ret = 0;
	bool _rslt = false;

	set_test_if_get(TEST_STATE_STOP, TEST_OPTION_ONE, 123.456, -123.456);
	set_judge_data(0, DBL, (judge_ope_t)-1, (ADDRESS_T)&test_if_get.mode,
		TEST_STATE_STOP, 0.0, 0, 1, 0);

	_ret = event_judge((ADDRESS_T)&judge_data_base[0], ALL_OR, false, &_rslt);

	EXPECT_EQ(_ret, 1);
	EXPECT_FALSE(_rslt);
}

/// \brief TEST event_judge
///
/// いべんと
TEST_F(SeqCtrlTest, EventJudgeExceedMax) {

	int i;
	int _ret = 0;
	bool _rslt = false;

	set_test_if_get(TEST_STATE_STOP, TEST_OPTION_ONE, 123.456, -123.456);
	for (i=0; i<JUDGE_MAX; i++) {
		set_judge_data(i, DBL, EQ, (ADDRESS_T)&test_if_get.mode,
			TEST_STATE_STOP, 0.0, 0, 1, (ADDRESS_T)&judge_data_base[i+1]);
	}

	_ret = event_judge((ADDRESS_T)&judge_data_base[0], ALL_OR, false, &_rslt);

	EXPECT_EQ(_ret, 1);
	EXPECT_FALSE(_rslt);
}

/// \brief TEST event_exec
///
/// いべんと
TEST_F(SeqCtrlTest, EventExecIsNormal) {

	int _ret = 0;

	set_test_if_set(TEST_STATE_STOP, TEST_OPTION_ONE, 0.0, 0.0);
	set_exec_data(0, INT, (ADDRESS_T)&test_if_set.mode, TEST_STATE_EXEC, 0.0, (ADDRESS_T)&exec_data_base[1]);
	set_exec_data(1, INT, (ADDRESS_T)&test_if_set.option, TEST_OPTION_TWO, 0.0, (ADDRESS_T)&exec_data_base[2]);
	set_exec_data(2, DBL, (ADDRESS_T)&test_if_set.param1, 0, 123.456, (ADDRESS_T)&exec_data_base[3]);
	set_exec_data(3, DBL, (ADDRESS_T)&test_if_set.param2, 0, -123.456, 0);

	_ret = event_exec((ADDRESS_T)&exec_data_base[0]);

	EXPECT_EQ(_ret, 0);
	EXPECT_EQ(test_if_set.mode, TEST_STATE_EXEC);
	EXPECT_EQ(test_if_set.option, TEST_OPTION_TWO);
	EXPECT_DOUBLE_EQ(test_if_set.param1, 123.456);
	EXPECT_DOUBLE_EQ(test_if_set.param2, -123.456);
}

/// \brief TEST event_exec
///
/// いべんと
TEST_F(SeqCtrlTest, EventExecIsWrongType) {
	
	int _ret = 0;
	set_test_if_set(TEST_STATE_STOP, TEST_OPTION_ONE, 0.0, 0.0);
	set_exec_data(0, (var_type_t)-1, (ADDRESS_T)&test_if_set.mode, TEST_STATE_EXEC, 0.0, 0);

	_ret = event_exec((ADDRESS_T)&exec_data_base[0]);

	EXPECT_EQ(_ret, 1);
	EXPECT_EQ(test_if_set.mode, TEST_STATE_STOP);
	EXPECT_EQ(test_if_set.option, TEST_OPTION_ONE);
	EXPECT_DOUBLE_EQ(test_if_set.param1, 0.0);
	EXPECT_DOUBLE_EQ(test_if_set.param2, 0.0);
}

/// \brief TEST event_exec
///
/// いべんと
TEST_F(SeqCtrlTest, EventExecExceedMax) {
	
	int i;
	int _ret = 0;

	set_test_if_set(TEST_STATE_STOP, TEST_OPTION_ONE, 0.0, 0.0);
	for (i=0; i<EXEC_MAX; i++) {
		set_exec_data(i, INT, (ADDRESS_T)&test_if_set.mode, TEST_STATE_EXEC, 0.0, (ADDRESS_T)&exec_data_base[i+1]);
	}

	_ret = event_exec((ADDRESS_T)&exec_data_base[0]);

	EXPECT_EQ(_ret, 1);
}
