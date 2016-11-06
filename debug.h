/*
 * Cipher Algorithm ARC4
 * by Alexander Postoronca (Alex Lewz)

Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef AR4_DEBUG
#define AR4_DEBUG

#include <stdio.h>
#include <string.h>

#define DBG 1
#define g DBG && 

#define DBG_FILE "dbg.out"

static char dbg_char[20241*256];
static int dbg_cc = 0;

void dbg_out()
{
	FILE *fp_dbg = fopen(DBG_FILE, "a");
	if (fp_dbg != NULL)
	{
		fwrite(dbg_char, 1, dbg_cc, fp_dbg);
		fclose(fp_dbg);
	}
}

void dbg_read_block(char * header, AR4_UNIT * registru)
{
	dbg_cc += snprintf(&dbg_char[dbg_cc], strlen(header)+2,
        "%s\n", header);

	for(int i = 0; i < BLOCK_SIZE; i++)
	    dbg_cc += snprintf(&dbg_char[dbg_cc], 28,
            "block:  (%016llX)\n", registru[i]);

	dbg_cc += snprintf(&dbg_char[dbg_cc], 28,
        "::::::::::::::::::::::::::\n");
            
}

#endif