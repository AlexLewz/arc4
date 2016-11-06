/*
 * Cipher Algorithm ARC4
 * by Alexander Postoronca (Alex Lewz)

Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0
 */

#include <stdlib.h>
#include <stdio.h>

#include "ar4ive.h"

#define RANDOM_DEVICE "/dev/urandom"

void generate_key(AR4_KEY *key)
{
    FILE *fp_rnd;
    fp_rnd = fopen(RANDOM_DEVICE, "rb");
    if(fp_rnd == NULL)
    {
        fprintf(stderr, "open error\n");
        exit(1);
    }
    fread(key, 8, 4, fp_rnd);
    fclose(fp_rnd);
}
void generate_iv(UINT *iv)
{
    FILE *fp_iv;
    fp_iv = fopen(RANDOM_DEVICE, "rb");
    if(fp_iv == NULL)
    {
        fprintf(stderr, "open error\n");
        exit(1);
    }
    fread(iv, BLOCK_SIZE, 8, fp_iv);
    fclose(fp_iv);
}

void read_key(char * file, AR4_KEY * key, long int * file_size)
{
    FILE *fp_key;
    fp_key = fopen(file, "rb");
    if(fp_key == NULL)
    {
        fprintf(stderr, "open error\n");
        exit(1);
    }
    fread(key, 8, 4, fp_key);
    fread(file_size, sizeof (long int), 1, fp_key);
    fclose(fp_key);
}

void write_key_prime(AR4_KEY * key)
{
    fwrite(key, 8, 4, stderr);
}

void write_key_size(long int * size)
{
    fwrite(size, sizeof(long int), 1, stderr);
}

void write_key_iv(UINT * iv)
{
    fwrite(iv, BLOCK_SIZE, 8, stderr);
}


void read_iv(char * file, UINT * iv)
{
    FILE *fp_iv;
    fp_iv = fopen(file, "rb");
    if(fp_iv == NULL)
    {
        fprintf(stderr, "IV open error\n");
        exit(1);
    }
    fread(iv, BLOCK_SIZE, 8, fp_iv);
    fclose(fp_iv);  
}

void read_internal_iv(char * file, UINT * iiv)
{
    FILE *fp_iiv;
    fp_iiv = fopen(file, "rb");
    if(fp_iiv == NULL)
    {
        fprintf(stderr, "IV open error\n");
        exit(1);
    }
    rewind(fp_iiv);
    fseek(fp_iiv, 32+8, SEEK_SET);
    fread(iiv, BLOCK_SIZE, 8, fp_iiv);
    fclose(fp_iiv);  
}