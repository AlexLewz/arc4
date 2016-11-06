/*
 * Cipher Algorithm ARC4
 * by Alexander Postoronca (Alex Lewz)

Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef AR4AR_H_
#define AR4AR_H_

//#include "ar4ive.h"

#define NONE 0
#define ATOM 1
#define DOOO 2

#define ZERO 0
#define ONE 1
#define TWO 2

/* rw_stack.c */
extern void generate_key(AR4_KEY *);
extern void generate_iv(UINT *);
extern void read_key(char *, AR4_KEY *, long int *);
extern void read_iv(char *, UINT *);
extern void read_internal_iv(char *, UINT *);
extern void write_key_prime(AR4_KEY *);
extern void write_key_size(long int *);
extern void write_key_iv(UINT *);
/* usage.c */
extern int usage();
extern void version();
/* ar4ive2.c */
extern void ar4ive_encrypt(UINT *, AR4_KEY *);
extern void ar4ive_decrypt(UINT *, AR4_KEY *);
extern void sa_box_init( AR4_KEY );
extern void wave_rotor(AR4_KEY *);
extern void subrotor_plus(AR4_KEY *);
extern void subrotor_minus(AR4_KEY *);
/* internal declarations */
//void clean(AR4_UNIT *, long int  );
void clean(AR4_UNIT *, AR4_UNIT *);
void copy(AR4_UNIT *, AR4_UNIT *);
void xor(AR4_UNIT *, AR4_UNIT *);


#endif