// autogenarated sequence definition header file
// generated 2025-08-21 16:59:55.814658

// define
#define SEQ_0_EVENT_BASE (0)
#define SEQ_0_JUDGE_BASE (0)
#define SEQ_0_EXEC_BASE (0)

// extern
extern int seq_id_data_base_sqvcon[SEQ_ID_MAX];
extern event_data_t* seq_step_data_base_sqvcon[SEQ_STEP_MAX];
extern event_data_t event_data_base_sqvcon[20];
extern judge_data_t judge_data_base_sqvcon[20];
extern exec_data_t exec_data_base_sqvcon[20];

// seq_id definition
int seq_id_data_base_sqvcon[SEQ_ID_MAX] = {
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
event_data_t* seq_step_data_base_sqvcon[SEQ_STEP_MAX] = {
	&event_data_base_sqvcon[SEQ_0_EVENT_BASE+0],	// SEQ 0 : STEP0
	&event_data_base_sqvcon[SEQ_0_EVENT_BASE+1],	// SEQ 0 : STEP1
	&event_data_base_sqvcon[SEQ_0_EVENT_BASE+2],	// SEQ 0 : STEP2
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
event_data_t event_data_base_sqvcon[20] = {
	{ALL_AND, (ADDRESS_T)&judge_data_base_sqvcon[SEQ_0_JUDGE_BASE+0], (ADDRESS_T)&exec_data_base_sqvcon[SEQ_0_EXEC_BASE+0], 1, -1, 0},	// SEQ 0 : START_SEQ
	{ALL_AND, (ADDRESS_T)&judge_data_base_sqvcon[SEQ_0_JUDGE_BASE+1], (ADDRESS_T)&exec_data_base_sqvcon[SEQ_0_EXEC_BASE+5], 2, -1, 0},	// SEQ 0 : MOVE_STEP2
	{ALL_AND, (ADDRESS_T)&judge_data_base_sqvcon[SEQ_0_JUDGE_BASE+2], 0, -1, -1, 0},	// SEQ 0 : STOP_SEQ
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
judge_data_t judge_data_base_sqvcon[20] = {
	{DBL, GTE, (ADDRESS_T)&mission_time_s, 0x0000000000000000, 0, 1, 0},	// SEQ 0 : start_time 0
	{INT, EQ, (ADDRESS_T)&cro.flg_stbl, CNTROT_STBL_ON, 0, 2000, 0},	// SEQ 0 : control_stable CNTROT_STBL_ON
	{INT, EQ, (ADDRESS_T)&cro.flg_stbl, CNTROT_STBL_ON, 0, 2000, 0},	// SEQ 0 : control_stable CNTROT_STBL_ON
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
exec_data_t exec_data_base_sqvcon[20] = {
	{INT, (ADDRESS_T)&siii.mode, SIIDEAL_MODE_ON, (ADDRESS_T)&exec_data_base_sqvcon[1]},	// SEQ 0 : change_siideal_mode SIIDEAL_MODE_ON
	{INT, (ADDRESS_T)&nri.mode, NAVROT_MODE_ON, (ADDRESS_T)&exec_data_base_sqvcon[2]},	// SEQ 0 : change_navrot_mode NAVROT_MODE_ON
	{INT, (ADDRESS_T)&gri.mode, GUIROT_MODE_TGT_1, (ADDRESS_T)&exec_data_base_sqvcon[3]},	// SEQ 0 : change_guirot_mode GUIROT_MODE_TGT_1
	{INT, (ADDRESS_T)&cri.mode, CNTROT_MODE_PD, (ADDRESS_T)&exec_data_base_sqvcon[4]},	// SEQ 0 : change_cntrot_mode CNTROT_MODE_PD
	{INT, (ADDRESS_T)&aiii.mode, AIIDEAL_MODE_ON, 0},	// SEQ 0 : change_aiideal_mode AIIDEAL_MODE_ON
	{INT, (ADDRESS_T)&gri.mode, GUIROT_MODE_TGT_2, 0},	// SEQ 0 : change_guirot_mode GUIROT_MODE_TGT_2
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
