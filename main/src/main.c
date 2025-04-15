#include "common.h"
#include "main_ext.h"
#include "guidance.h"
#include "plant.h"
#include "userif.h"

main_params_t mp;
main_state_t ms;

int main(int argc, char* argv[]) {
	// # declaration
	int i;
	int _ret;

	enum {
		ARG_PROG_NAME,
		ARG_PATH_IN,
		ARG__COUNT,
	};

	// # handle args
	if (argc != ARG__COUNT) {
		fprintf(stderr, "Usage:\n");
		fprintf(stderr, "  %s <INPUT>\n", argv[ARG_PROG_NAME]);
		return EXIT_FAILURE;
	}

	// # initialize
	// ## user interface init
	_ret = userif_init(ms.t, argv[ARG_PATH_IN]);
	if (EXIT_SUCCESS != _ret) return EXIT_FAILURE;

	// ## sensor init
	// none

	// ## sensor interface init
	// none

	// ## navigation init
	// none

	// ## guidance init
	// none
	guidance_init();

	// ## control init
	// none

	// ## actuator interface init
	// none

	// ## sequence init
	// none

	// ## actuator init
	// none

	// ## plant init
	// none

	// # simulation loop
	userif_main_loop_start(ms.t);
	for (i=0; i < mp.num_of_loop; i++) {
		// ## sensor
		// none

		// ## sensor interface
		// none

		// ## navigation
		// none

		// ## guidance
		guidance_main(&ps, &gs);

		// ## control
		// none

		// ## actuator interface
		// none

		// ## sequence
		// none

		// ## actuator
		// none

		// ## plant
		plant_main(&ms.t, mp.dt, &ps);

		// ## user interface (log)
		userif_main(i, ms.t);
	}
	userif_main_loop_end();

	// # finalize
	// ## user interface (log) finish
	userif_finish();

	return EXIT_SUCCESS;
}