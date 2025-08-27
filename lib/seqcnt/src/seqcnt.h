/// @file seqcnt.h
/// @brief functions and structures of sequence control

#ifndef __SEQCNT_H__
#define __SEQCNT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "seqcnt_ext.h"

#if __SIZEOF_POINTER__ == 4
#define ADDRESS_T uint32_t
#else
#define ADDRESS_T uint64_t
#endif

///////////////////////////////////////////////////////////////////////////////
// seq_data, event_data
///////////////////////////////////////////////////////////////////////////////

#define EVENT_MAX (10)


///////////////////////////////////////////////////////////////////////////////
// judge_data, exec_data
///////////////////////////////////////////////////////////////////////////////

#define JUDGE_MAX (10)
#define EXEC_MAX (10)

/// \brief イベント判定/実行処理 変数型指定ENUM
///
/// - INT : int
/// - DBL : double
typedef enum {
	INT = 0,
	DBL
} var_type_t;

/// \brief イベント判定処理 比較演算子指定ENUM
///
/// - EQ : =
/// - NEQ : !=
/// - LT : <
/// - LTE : <=
/// - GT : >
/// - GTE : >=
typedef enum {
	EQ = 0,
	NEQ,
	LT,
	LTE,
	GT,
	GTE
} judge_ope_t;

/// \brief イベント判定処理情報
///
/// 
typedef struct {
	var_type_t type;
	judge_ope_t ope;
	ADDRESS_T ref;
	uint64_t thrs;
	int count;
	int num;
	ADDRESS_T next; 
} judge_data_t;

/// \brief イベント実行処理情報
///
/// 
typedef struct {
	var_type_t type;
	ADDRESS_T dest;
	uint64_t src;
	ADDRESS_T next; 
} exec_data_t;


///////////////////////////////////////////////////////////////////////////////
// seq_id_data, seq_step_data, event_data
///////////////////////////////////////////////////////////////////////////////

#define SEQ_ID_MAX (10)
#define SEQ_STEP_MAX (100)
#define EVENT_MAX (10)

/// \brief イベント判定処理 ALL_AND / ALL_OR指定ENUM
///
/// - ALL_AND
/// - ALL_OR
typedef enum {
	ALL_AND = 0,
	ALL_OR
} judge_condition_t;

/// \brief イベント情報
///
/// 
typedef struct {
	judge_condition_t judge_condition;
	ADDRESS_T judge_data_address;
	ADDRESS_T exec_data_address;
	int next_step;
	int next_seq;
	ADDRESS_T next_event;
} event_data_t;


///////////////////////////////////////////////////////////////////////////////
// extern
///////////////////////////////////////////////////////////////////////////////

extern int seq_step; // temporaly

#define TIMER_MAX (INT32_MAX)

extern int seq_timer;
extern int step_timer;
extern int free_timer;


///////////////////////////////////////////////////////////////////////////////
// function prototype
///////////////////////////////////////////////////////////////////////////////

void seqcnt(seqcnt_state_t* seqcnt_state, 
				seqcnt_state_t* seqcnt_state_next,  
				const int seq_id_data_base[SEQ_ID_MAX], 
				event_data_t* seq_step_data_base[SEQ_STEP_MAX]);

void test_func(int input, int* output);

int event_judge(ADDRESS_T judge_data_address, 
				judge_condition_t judge_condition, 
				bool reset_count, 
				bool* judge_result);

int check_count(int num, int* ctr);

int event_exec(ADDRESS_T exec_data_address);


#ifdef __cplusplus
} // extern "C"
#endif

#endif