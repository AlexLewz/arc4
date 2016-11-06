/*
 * Cipher Algorithm ARC4
 * by Alexander Postoronca (Alex Lewz)

Date 22/10/2016 

Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0
 */

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>

#include "ar4ive.h"
#include "ar4_256.h"
//#include "debug.h"

void
clean(AR4_UNIT * block, AR4_UNIT * mask)
{
    R i = BLOCK_SIZE; while(i--)
    {
        block[i] = mask[i];
    }
}

void
copy(AR4_UNIT * block, AR4_UNIT * save)
{
    R i = BLOCK_SIZE; while(i--)
    {
        save[i] = block[i];
    }
}

void
xor(AR4_UNIT * block, AR4_UNIT * xkey)
{
    R i = BLOCK_SIZE; while(i--)
    {
        block[i] ^= xkey[i];
    }
}

static struct option const long_options[] =
{
  {"decrypt",         no_argument,  0, 'd'},
  {"rounds",    required_argument,  0, 'r'},
  {"use-iv",    required_argument,  0, 'i'},
  {"key",       required_argument,  0, 'k'},
  {"use-internal-iv", no_argument,  0, 'n'},
  {"version",         no_argument,  0, 'v'},
  {"help",            no_argument,  0, 'h'},
  {NULL,        0,              NULL,   0 }
};

static int uncrypt     = NONE;
static int rounds      = MIN_ROUNDS;
static int custom_key  = NONE;
static int external_iv = NONE;
static int internal_iv = NONE;

int
main(int argc, char **argv)
{
           R it11      = NONE;
    long int file_size = NONE;
    long int fn        = NONE;
    int opt            = NONE;

    AR4_KEY key; // AR4_KEY *key = calloc(8,BLOCK_SIZE);
    AR4_KEY subkey;

    AR4_UNIT registru[BLOCK_SIZE]; // Main
    AR4_UNIT unpluggg[BLOCK_SIZE]; // For random fill block (clean())

    /* For Input Vector */
    AR4_UNIT store_iv[BLOCK_SIZE]; // Read/Write
    AR4_UNIT tmp_save[BLOCK_SIZE]; // Temp
    AR4_UNIT works_iv[BLOCK_SIZE]; // Link

    char key_fine[FILENAME_MAX];

    FILE *fp_message;

    while ((opt = getopt_long (argc, argv, "dr:i:nk:hv", long_options, NULL)) != -1)
    {
        switch (opt)
        {
            case 'd': uncrypt = ATOM; break;

            case 'r':
                if(atoi(optarg) > MIN_ROUNDS) rounds = atoi(optarg);
            break;

            case 'i':
                    read_iv(optarg, store_iv);
                    external_iv = ATOM;
                    internal_iv = NONE;
            case 'n':
                    external_iv = NONE;
                    internal_iv = ATOM;
            break;

            case 'k':
                custom_key = ATOM;
                strncpy(key_fine, optarg, strlen(optarg));
            break;

            case 'v':
                version();
                return EXIT_FAILURE;
            break;
            case '?':
                fprintf(stderr, "Error - param not found\n");
                fprintf(stderr, "Use: arc4 --help for param info\n");
                return EXIT_FAILURE;
            break;
            
            case 'h': default: return usage(); break;
        }
    }

    if(!strcmp(argv[0], argv[argc-1])) return usage();

    if( uncrypt && !custom_key)          return usage();
    if(custom_key) read_key(key_fine, &key, &file_size);
    if(!uncrypt && !custom_key)      generate_key(&key);
    if(!uncrypt && internal_iv)   generate_iv(store_iv);
    if( uncrypt && internal_iv)   read_internal_iv(key_fine, store_iv);
    if(!uncrypt)                  write_key_prime(&key);
    if(!uncrypt)                  generate_iv(unpluggg);
    if(!uncrypt)               copy(unpluggg, registru);
    // copy(unpluggg, registru); if we have once unfilled block

    sa_box_init(key); // prepare substitution table

    fp_message = fopen(argv[argc-1], "rb");
    if(fp_message == NULL)
    {
        fprintf(stderr, "Text open - error\n");
        exit(EXIT_FAILURE);
    }
    
    if(external_iv || internal_iv) /* Cipher Block Chaining */
    {
        copy(store_iv, works_iv);

        while((fn = fread(registru, sizeof(char), UNIT_CHAR, fp_message)))
        {
            it11 = rounds;
            wave_rotor(&key);

            switch(uncrypt)
            {
                case NONE: /* IF ENCRYPT MODE */
                    xor(registru, works_iv);
                    while(it11--)
                    {
                        ar4ive_encrypt(registru, &key);
                    }
                    copy(registru, works_iv);

                    file_size += fn;
                    fwrite(registru, 1, UNIT_CHAR, stdout);

                    clean(registru, unpluggg);
                break;
                    
                case ATOM: /* IF DECRYPT MODE */
                    copy(registru, tmp_save);
                    while(it11--)
                    {
                        ar4ive_decrypt(registru, &key);
                    } file_size -= fn;
                    xor (registru, works_iv);
                    copy(tmp_save, works_iv);

                    if(file_size < 0)
                        fwrite(registru, 1, UNIT_CHAR + (file_size), stdout);
                    else
                        fwrite(registru, 1, UNIT_CHAR, stdout);
                break;
            }
        }
    }
    else /* Electronic Code Book */
    {
        while((fn = fread(registru, sizeof(char), UNIT_CHAR, fp_message)))
        {
            it11 = rounds;
            wave_rotor(&key);

            switch(uncrypt)
            {
                case ATOM: /* IF DECRYPT MODE */

                    while(it11--)
                    {
                        ar4ive_decrypt(registru, &key);
                    } file_size -= fn;

                    if(file_size < 0)
                        fwrite(registru, 1, UNIT_CHAR + (file_size), stdout);
                    else
                        fwrite(registru, 1, UNIT_CHAR, stdout);
                break;

                case NONE: /* IF ENCRYPT MODE */
                    while(it11--)
                    {
                        ar4ive_encrypt(registru, &key);
                    } file_size += fn;

                    fwrite(registru, 1, UNIT_CHAR, stdout);

                    clean(registru,unpluggg);
                break;
            }
        }
    }
    if(!uncrypt)
    {
        write_key_size(&file_size);
        if(internal_iv) write_key_iv(store_iv);
    }

    fflush(stdout);
    fflush(stderr);

    fclose(fp_message);

    return EXIT_SUCCESS;
}