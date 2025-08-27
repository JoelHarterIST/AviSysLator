// autogenarated sequence definition header file
// generated 2025-08-21 16:47:47.814338

// define
#define SEQ_0_EVENT_BASE (0)
#define SEQ_0_JUDGE_BASE (0)
#define SEQ_0_EXEC_BASE (0)

// extern
extern int seq_id_data_base_sqmdl[SEQ_ID_MAX];
extern event_data_t* seq_step_data_base_sqmdl[SEQ_STEP_MAX];
extern event_data_t event_data_base_sqmdl[20];
extern judge_data_t judge_data_base_sqmdl[20];
extern exec_data_t exec_data_base_sqmdl[20];

// seq_id definition
int seq_id_data_base_sqmdl[SEQ_ID_MAX] = {
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
event_data_t* seq_step_data_base_sqmdl[SEQ_STEP_MAX] = {
	&event_data_base_sqmdl[SEQ_0_EVENT_BASE+0],	// SEQ 0 : STEP0
	&event_data_base_sqmdl[SEQ_0_EVENT_BASE+1],	// SEQ 0 : STEP1
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
	0,
	0};

// event definition
event_data_t event_data_base_sqmdl[20] = {
	{ALL_AND, (ADDRESS_T)&judge_data_base_sqmdl[SEQ_0_JUDGE_BASE+0], (ADDRESS_T)&exec_data_base_sqmdl[SEQ_0_EXEC_BASE+0], 1, -1, 0},	// SEQ 0 : START_SEQ
	{ALL_AND, (ADDRESS_T)&judge_data_base_sqmdl[SEQ_0_JUDGE_BASE+1], (ADDRESS_T)&exec_data_base_sqmdl[SEQ_0_EXEC_BASE+3], -1, -1, 0},	// SEQ 0 : STOP_SEQ
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
	{0, 0, 0, -1, -1, 0},
	{0, 0, 0, -1, -1, 0}
};

// judge definition
judge_data_t judge_data_base_sqmdl[20] = {
	{DBL, GTE, (ADDRESS_T)&mission_time_s, 0x0000000000000000, 0, 1, 0},	// SEQ 0 : start_time 0
	{DBL, GTE, (ADDRESS_T)&mission_time_s, 0x4056800000000000, 0, 1, 0},	// SEQ 0 : end_time 90
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
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0}
};

// exec definition
exec_data_t exec_data_base_sqmdl[20] = {
	{INT, (ADDRESS_T)&aii.mode, AIDEAL_MODE_ON, (ADDRESS_T)&exec_data_base_sqmdl[1]},	// SEQ 0 : change_aideal_mode AIDEAL_MODE_ON
	{INT, (ADDRESS_T)&pri.mode, PROT_MODE_INIT, (ADDRESS_T)&exec_data_base_sqmdl[2]},	// SEQ 0 : change_prot_mode PROT_MODE_INIT
	{INT, (ADDRESS_T)&sii.mode, SIDEAL_MODE_ON, 0},	// SEQ 0 : change_sideal_mode SIDEAL_MODE_ON
	{INT, (ADDRESS_T)&aii.mode, AIDEAL_MODE_OFF, (ADDRESS_T)&exec_data_base_sqmdl[4]},	// SEQ 0 : change_aideal_mode AIDEAL_MODE_OFF
	{INT, (ADDRESS_T)&pri.mode, PROT_MODE_OFF, (ADDRESS_T)&exec_data_base_sqmdl[5]},	// SEQ 0 : change_prot_mode PROT_MODE_OFF
	{INT, (ADDRESS_T)&sii.mode, SIDEAL_MODE_OFF, 0},	// SEQ 0 : change_sideal_mode SIDEAL_MODE_OFF
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
