#include <stdio.h>
#include "../../src/seqcnt.h"
#include "fcu_if.h"

int mission_time;

#include "seq_data_base.h"

#define END_TIME (500)

seq_context_t seq_context;
seq_context_t seq_context_next;

void main() {

	int i;
	mission_time = -300;
	seq_context_next.state = EXEC;
	// fcu_if_get.flight_mode = FMODE_ON;

	printf("start\n");
	for (i=0; i<END_TIME; i++) {
		printf("i=%d, mission_time=%d, state=%d, id=%d, step=%d\n",
			i, mission_time, seq_context.state, seq_context.id, seq_context.step);
		// printf("%p\n", seq_step_to_event[0]);
		seqcnt(&seq_context, 
					&seq_context_next, 
					&seq_id_data_base[0], 
					&seq_step_data_base[0]);
		mission_time++;
	}

	printf("end\n");

	return ;
}
