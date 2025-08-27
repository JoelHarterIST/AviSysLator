#include <stdio.h>
#include "../../src/seqcnt.h"

double mission_time_s;
int seq_cmd;
double seq_prm;

#include "seq_data_base.h"

#define END_TIME_S (50)
#define STEP_TIME_S (1)
#define END_TIME (END_TIME_S / STEP_TIME_S)

seq_context_t seq_context;
seq_context_t seq_context_next;

void main() {

	int i;
	mission_time_s = -20;
	seq_context_next.state = EXEC;
	// fcu_if_get.flight_mode = FMODE_ON;

	printf("start\n");
	for (i=0; i<END_TIME; i++) {
		printf("i=%d, mission_time=%f, state=%d->%d, id=%d->%d, step=%d->%d", 
			i, mission_time_s, seq_context.state, seq_context_next.state, 
			seq_context.id, seq_context_next.id, seq_context.step, seq_context_next.step);
		printf(", seq_cmd=%d, seq_prm=%f\n", seq_cmd, seq_prm);
		seqcnt(&seq_context, 
					&seq_context_next, 
					&seq_id_data_base[0], 
					&seq_step_data_base[0]);
		mission_time_s = mission_time_s + STEP_TIME_S;
	}

	printf("end\n");

	return ;
}
