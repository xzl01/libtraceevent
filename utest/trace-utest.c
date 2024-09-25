// SPDX-License-Identifier: LGPL-2.1
/*
 * Copyright (C) 2020, VMware, Tzvetomir Stoyanov <tz.stoyanov@gmail.com>
 *
 * Modified from libtracefs to libtraceevent:
 *   Copyright (C) 2021, VMware, Steven Rostedt <rostedt@goodmis.org>
 *
 */
#include <libgen.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "trace-utest.h"

enum unit_tests {
	RUN_NONE	= 0,
	RUN_TRACEEVENT	= (1 << 0),
	RUN_ALL		= 0xFFFF
};

static void print_help(char **argv)
{
	printf("Usage: %s [OPTIONS]\n", basename(argv[0]));
	printf("\t-s, --silent\tPrint test summary\n");
	printf("\t-r, --run test\tRun specific test:\n");
	printf("\t\t  traceevent   run libtraceevent tests\n");
	printf("\t-h, --help\tPrint usage information\n");
	exit(0);
}

int main(int argc, char **argv)
{
	CU_BasicRunMode verbose = CU_BRM_VERBOSE;
	enum unit_tests tests = RUN_NONE;
	int failed_tests;

	for (;;) {
		int c;
		int index = 0;
		const char *opts = "+hsr:";
		static struct option long_options[] = {
			{"silent", no_argument, NULL, 's'},
			{"run", required_argument, NULL, 'r'},
			{"help", no_argument, NULL, 'h'},
			{NULL, 0, NULL, 0}
		};

		c = getopt_long (argc, argv, opts, long_options, &index);
		if (c == -1)
			break;
		switch (c) {
		case 'r':
			if (strcmp(optarg, "traceevent") == 0)
				tests |= RUN_TRACEEVENT;
			else
				print_help(argv);
			break;
		case 's':
			verbose = CU_BRM_SILENT;
			break;
		case 'h':
		default:
			print_help(argv);
			break;
		}
	}

	if (tests == RUN_NONE)
		tests = RUN_ALL;

	if (CU_initialize_registry() != CUE_SUCCESS) {
		printf("Test registry cannot be initialized\n");
		return -1;
	}

	if (tests & RUN_TRACEEVENT)
		test_traceevent_lib();

	CU_basic_set_mode(verbose);
	CU_basic_run_tests();
	failed_tests = CU_get_number_of_tests_failed();
	CU_cleanup_registry();
	return failed_tests != 0;
}
