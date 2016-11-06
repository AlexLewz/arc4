/*
 * Cipher Algorithm ARC4
 * by Alexander Postoronca (Alex Lewz)

Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef AR4IVE_STRUCT_H_
#define AR4IVE_STRUCT_H_


#define UINT unsigned long long int
#define AR4_UNIT unsigned long long

#define R register int
#define r register 

typedef struct AR4_KEY
{
    UINT h[2]; // HORISONTAL LINES
    UINT v[2]; // VERTICAL LINES
} AR4_KEY; /* TASK: продумать ENDIANLESS */
    /* Заместо UINT: unsigned char char[8]?? */



#define BLOCK_SIZE 64
#define MIN_ROUNDS 16

#define CIPHER_SHIFT 1


#define UNIT_CHAR (BLOCK_SIZE*8)

#endif