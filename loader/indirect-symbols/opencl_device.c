/*
	This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.

	Copyright (C) 2019 Barcelona Supercomputing Center (BSC)
*/

#include "resolve.h"


#pragma GCC visibility push(default)

void nanos6_execute_opencl(struct node * args, nanos6_opencl_device_environment_t * device_env) {
{
	typedef void nanos6_execute_opencl_t(struct node * args, nanos6_opencl_device_environment_t * device_env);

	static nanos6_execute_opencl_t *symbol = NULL;
	if (__builtin_expect(symbol == NULL, 0)) {
		symbol = (nanos6_execute_opencl_t *) _nanos6_resolve_symbol("nanos6_execute_opencl_t", "essential", NULL);
	}

	(*symbol)(args, device_env);
}

#pragma GCC visibility pop
