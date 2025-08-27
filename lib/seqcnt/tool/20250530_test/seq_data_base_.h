// autogenarated sequence definition header file
// generated 2025-05-23 21:35:50.527672

// define
#define SEQ_0_EVENT_BASE (0)
#define SEQ_1_EVENT_BASE (8)
#define SEQ_0_JUDGE_BASE (0)
#define SEQ_1_JUDGE_BASE (10)
#define SEQ_0_EXEC_BASE (0)
#define SEQ_1_EXEC_BASE (8)

// extern
extern int seq_id_data_base[SEQ_ID_MAX];
extern event_data_t* seq_step_data_base[SEQ_STEP_MAX];
extern event_data_t event_data_base[20];
extern judge_data_t judge_data_base[20];
extern exec_data_t exec_data_base[20];

// seq_id definition
int seq_id_data_base[SEQ_ID_MAX] = {
	0,	// SEQ 0
	6,	// SEQ 1
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
	&event_data_base[SEQ_0_EVENT_BASE+0],	// SEQ 0 : READY
	&event_data_base[SEQ_0_EVENT_BASE+1],	// SEQ 0 : WAIT_MEIG
	&event_data_base[SEQ_0_EVENT_BASE+4],	// SEQ 0 : WAIT_LIFTOFF
	&event_data_base[SEQ_0_EVENT_BASE+5],	// SEQ 0 : WAIT_MEPO
	&event_data_base[SEQ_0_EVENT_BASE+6],	// SEQ 0 : WAIT_MECO
	&event_data_base[SEQ_0_EVENT_BASE+7],	// SEQ 0 : WAIT_MSEP
	&event_data_base[SEQ_1_EVENT_BASE+0],	// SEQ 1 : VALVE_CLOSE
	&event_data_base[SEQ_1_EVENT_BASE+1],	// SEQ 1 : WAIT_VLV_CLOSE
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
	{ALL_AND, (ADDRESS_T)&judge_data_base[SEQ_0_JUDGE_BASE+0], (ADDRESS_T)&exec_data_base[SEQ_0_EXEC_BASE+0], 1, -1, 0},	// SEQ 0 : SOS
	{ALL_AND, (ADDRESS_T)&judge_data_base[SEQ_0_JUDGE_BASE+1], (ADDRESS_T)&exec_data_base[SEQ_0_EXEC_BASE+1], 2, -1, (ADDRESS_T)&event_data_base[2]},	// SEQ 0 : MEIG
	{ALL_AND, (ADDRESS_T)&judge_data_base[SEQ_0_JUDGE_BASE+3], (ADDRESS_T)&exec_data_base[SEQ_0_EXEC_BASE+2], -1, 1, (ADDRESS_T)&event_data_base[3]},	// SEQ 0 : ABORT
	{ALL_AND, (ADDRESS_T)&judge_data_base[SEQ_0_JUDGE_BASE+5], (ADDRESS_T)&exec_data_base[SEQ_0_EXEC_BASE+3], -1, -1, 0},	// SEQ 0 : GSE_CMD_STOP
	{ALL_AND, (ADDRESS_T)&judge_data_base[SEQ_0_JUDGE_BASE+6], (ADDRESS_T)&exec_data_base[SEQ_0_EXEC_BASE+4], 3, -1, 0},	// SEQ 0 : LIFTOFF
	{ALL_AND, (ADDRESS_T)&judge_data_base[SEQ_0_JUDGE_BASE+7], (ADDRESS_T)&exec_data_base[SEQ_0_EXEC_BASE+5], 4, -1, 0},	// SEQ 0 : MEPO
	{ALL_AND, (ADDRESS_T)&judge_data_base[SEQ_0_JUDGE_BASE+8], (ADDRESS_T)&exec_data_base[SEQ_0_EXEC_BASE+6], 5, -1, 0},	// SEQ 0 : MECO
	{ALL_AND, (ADDRESS_T)&judge_data_base[SEQ_0_JUDGE_BASE+9], (ADDRESS_T)&exec_data_base[SEQ_0_EXEC_BASE+7], -1, -1, 0},	// SEQ 0 : MSEP
	{ALL_AND, 0, (ADDRESS_T)&exec_data_base[SEQ_1_EXEC_BASE+0], 1, -1, 0},	// SEQ 1 : VALVE_CLOSE
	{ALL_AND, (ADDRESS_T)&judge_data_base[SEQ_1_JUDGE_BASE+0], (ADDRESS_T)&exec_data_base[SEQ_1_EXEC_BASE+1], -1, -1, 0},	// SEQ 1 : STOP
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
	{INT, GTE, (ADDRESS_T)&mission_time, -240, 0, 1, 0},	// SEQ 0 : sos_time
	{INT, GTE, (ADDRESS_T)&mission_time, -5, 0, 1, (ADDRESS_T)&judge_data_base[2]},	// SEQ 0 : meig_time
	{INT, EQ, (ADDRESS_T)&fcu_if_get.flight_mode, FMODE_ON, 0, 1, 0},	// SEQ 0 : filghtmode_on
	{INT, GTE, (ADDRESS_T)&mission_time, -5, 0, 1, (ADDRESS_T)&judge_data_base[4]},	// SEQ 0 : meig_time
	{INT, NEQ, (ADDRESS_T)&fcu_if_get.flight_mode, FMODE_ON, 0, 1, 0},	// SEQ 0 : filghtmode_on
	{INT, EQ, (ADDRESS_T)&fcu_if_get.stop_from_gse, GSE_CMD_STOP, 0, 1, 0},	// SEQ 0 : stop_cmd
	{INT, GTE, (ADDRESS_T)&mission_time, 0, 0, 1, 0},	// SEQ 0 : liftoff_time
	{INT, GTE, (ADDRESS_T)&mission_time, 98, 0, 1, 0},	// SEQ 0 : mepo_time
	{INT, GTE, (ADDRESS_T)&mission_time, 164, 0, 1, 0},	// SEQ 0 : meco_time
	{INT, GTE, (ADDRESS_T)&mission_time, 169, 0, 1, 0},	// SEQ 0 : msep_time
	{INT, GTE, (ADDRESS_T)&step_timer, 50, 0, 1, 0},	// SEQ 1 : valve_close_time
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
	{INT, (ADDRESS_T)&fcu_if_set.state, FSEQ_WAIT_MEIG, 0},	// SEQ 0 : change_state_to_wait_meig
	{INT, (ADDRESS_T)&fcu_if_set.state, FSEQ_WAIT_LIFTOFF, 0},	// SEQ 0 : change_state_to_wait_liftoff
	{INT, (ADDRESS_T)&fcu_if_set.state, FSEQ_STOP, 0},	// SEQ 0 : change_state_to_stop
	{INT, (ADDRESS_T)&fcu_if_set.state, FSEQ_STOP, 0},	// SEQ 0 : change_state_to_wait_meig
	{INT, (ADDRESS_T)&fcu_if_set.state, FSEQ_WAIT_MEPO, 0},	// SEQ 0 : change_state_to_wait_mepo
	{INT, (ADDRESS_T)&fcu_if_set.state, FSEQ_WAIT_MECO, 0},	// SEQ 0 : change_state_to_wait_meco
	{INT, (ADDRESS_T)&fcu_if_set.state, FSEQ_WAIT_MSEP, 0},	// SEQ 0 : change_state_to_wait_msep
	{INT, (ADDRESS_T)&fcu_if_set.state, FSEQ_STOP, 0},	// SEQ 0 : change_state_to_stop
	{INT, (ADDRESS_T)&fcu_if_set.state, FSEQ_WAIT_VLV_CLOSE, 0},	// SEQ 1 : change_state_to_wait_valve_close
	{INT, (ADDRESS_T)&fcu_if_set.state, FSEQ_STOP, 0},	// SEQ 1 : change_state_to_stop
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
