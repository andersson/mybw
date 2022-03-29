/*
 * Copyright (c) 2022, Linaro Ltd.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* gcc -O2 -Wall -fno-builtin -o mybw mybw.c */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define NSEC_PER_SEC 1000000000UL

#define TEST_SIZE (50 * 1024ULL*1024*1024) /* 50G */

static char *sizestr(uint64_t sz)
{
	static char str[16];

	if (sz > 1024 * 1024)
		snprintf(str, sizeof(str), "%5luM", sz / (1024*1024));
	else if (sz > 1024)
		snprintf(str, sizeof(str), "%5luK", sz / 1024);
	else
		snprintf(str, sizeof(str), "%6lu", sz);

	return str;
}

static void measure(uint64_t block_size)
{
	struct timespec start;
	struct timespec end;
	uint64_t block;
	uint64_t ts;
	void *source;
	void *sink;

	if (block_size > TEST_SIZE)
		return;

	sink = malloc(block_size);
	memset(sink, 0, block_size);

	source = malloc(block_size);
	memset(source, 0xcd, block_size);

	clock_gettime(CLOCK_MONOTONIC, &start);

	for (block = 0; block < (TEST_SIZE / block_size); block++)
		memcpy(sink, source, block_size);

	clock_gettime(CLOCK_MONOTONIC, &end);

	free(sink);
	free(source);

	ts = ((end.tv_sec - start.tv_sec) * NSEC_PER_SEC) + (end.tv_nsec - start.tv_nsec);

	printf("%s: %.02fMB/s\n", sizestr(block_size), (double)TEST_SIZE * 1000 / ts);
}

int main()
{
	int i;

	for (i = 64; i <= 32 * 1024 * 1024; i <<= 1)
		measure(i);

	return 0;
}
