/*
 * Cipher Algorithm ARC4
 * by Alexander Postoronca (Alex Lewz)

Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0
 */

#include <stdio.h>

#define PROGRAM_AUTHORS "Alexander Postoronca"
#define PROGRAM_VERSION "2.2.8"
#define PROGRAM_NAME "arc4"

void version()
{
      fprintf(stderr, "arc4 version: %s\n", "2.2.8");
}

void help()
{
    fprintf(stderr,
            "Usage: arc4 [-d] [-r count] [[-i file]|[-n]] [-k file] [-v] [-h] file\n"
            "Purpose: This is block cipher with 256 bit key\n"
            "and 64x64 bit block, based on premutation feature\n"
            "\n"
            "Options: \n"
            "  -d   --decrypt  - Decrypt Message\n"
            "  -r   --rounds   - Set rounds count (16 by default)\n"
            "  -i   --use-iv   - CBC mode with manual 512bit IV file\n"
            "  -n   --use-internal-iv - CBC mode. load/generate IV with key file\n"
            "  -k   --key      - Set manual key file structure\n"
            "  -v   --verison  - Display version cipher information\n"
            "  -h   --help     - Show this help message\n"
            "\n"
            "stdout stream - Used for encrypt/decrypt file\n"
            "stderr stream - Used for key structure or help message\n"
            "\n"
            "Example: \n"
            "arc4 message > ecnrypt_message 2> key_file\n"
            "arc4 -dk key_file encrypt_message > message\n"
    );
}

int usage()
{
      help();
      return 1;
}