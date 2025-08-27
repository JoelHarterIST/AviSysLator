// autogenarated sequence definition header file
// generated 2025-06-03 22:02:00.502869

// define
#define SEQ_0_EVENT_BASE (0)
#define SEQ_0_JUDGE_BASE (0)
#define SEQ_0_EXEC_BASE (0)

// extern
extern int seq_id_data_base[SEQ_ID_MAX];
extern event_data_t* seq_step_data_base[SEQ_STEP_MAX];
extern event_data_t event_data_base[20];
extern judge_data_t judge_data_base[20];
extern exec_data_t exec_data_base[20];

// seq_id definition
int seq_id_data_base[SEQ_ID_MAX] = {
	0,	// SEQ 0
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0};

// seq_step definition
event_data_t* seq_step_data_base[SEQ_STEP_MAX] = {
	&event_data_base[SEQ_0_EVENT_BASE+0],	// SEQ 0 : STEP0
	&event_data_base[SEQ_0_EVENT_BASE+1],	// SEQ 0 : STEP1
	&event_data_base[SEQ_0_EVENT_BASE+2],	// SEQ 0 : STEP2
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0};

// event definition
event_data_t event_data_base[20] = {
	{ALL_AND, (ADDRESS_T)&judge_data_base[SEQ_0_JUDGE_BASE+0], (ADDRESS_T)&exec_data_base[SEQ_0_EXEC_BASE+0], 1, -1, 0},	// SEQ 0 : START_SEQ
	{ALL_AND, (ADDRESS_T)&judge_data_base[SEQ_0_JUDGE_BASE+1], (ADDRESS_T)&exec_data_base[SEQ_0_EXEC_BASE+2], 2, -1, 0},	// SEQ 0 : MOVE_STEP2
	{ALL_AND, (ADDRESS_T)&judge_data_base[SEQ_0_JUDGE_BASE+2], (ADDRESS_T)&exec_data_base[SEQ_0_EXEC_BASE+4], -1, -1, 0},	// SEQ 0 : STOP_SEQ
	{0, 0, 0, -1, -1, 0},
	{0, 0, 0, -1, -1, 0},
	{0, 0, 0, -1, -1, 0},
	{0, 0, 0, -1, -1, 0},
	{0, 0, 0, -1, -1, 0},
	{0, 0, 0, -1, -1, 0},
	{0, 0, 0, -1, -1, 0},
	{0, 0, 0, -1, -1, 0},
	{0, 0, 0, -1, -1, 0},
	{0, 0, 0, -1, -1, 0},
	{0, 0, 0, -1, -1, 0},
	{0, 0, 0, -1, -1, 0},
	{0, 0, 0, -1, -1, 0},
	{0, 0, 0, -1, -1, 0},
	{0, 0, 0, -1, -1, 0},
	{0, 0, 0, -1, -1, 0},
	{0, 0, 0, -1, -1, 0}
};

// judge definition
judge_data_t judge_data_base[20] = {
	{DBL, GTE, (ADDRESS_T)&mission_time_s, 0x0000000000000000, 0, 1, 0},	// SEQ 0 : start_time 0
	{DBL, GTE, (ADDRESS_T)&mission_time_s, 0x4024000000000000, 0, 1, 0},	// SEQ 0 : step2_time 10
	{DBL, GTE, (ADDRESS_T)&mission_time_s, 0x4034000000000000, 0, 1, 0},	// SEQ 0 : end_time 20
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0}
};

// exec definition
exec_data_t exec_data_base[20] = {
	{INT, (ADDRESS_T)&seq_cmd, 1, (ADDRESS_T)&exec_data_base[1]},	// SEQ 0 : change_seq_cmd 1
	{DBL, (ADDRESS_T)&seq_prm, 0x4024000000000000, 0},	// SEQ 0 : change_seq_param 10
	{INT, (ADDRESS_T)&seq_cmd, 2, (ADDRESS_T)&exec_data_base[3]},	// SEQ 0 : change_seq_cmd 2
	{DBL, (ADDRESS_T)&seq_prm, 0x4034000000000000, 0},	// SEQ 0 : change_seq_param 20
	{INT, (ADDRESS_T)&seq_cmd, 3, (ADDRESS_T)&exec_data_base[5]},	// SEQ 0 : change_seq_cmd 3
	{DBL, (ADDRESS_T)&seq_prm, 0x403e000000000000, 0},	// SEQ 0 : change_seq_param 30
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0}
};
