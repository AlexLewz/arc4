/*
 * Cipher Algorithm ARC4
 * by Alexander Postoronca (Alex Lewz)

Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0
 */

#include "ar4ive.h"

#define BIT(m,o) ((m >> o) & 0x01) //   (m & (0x01 << o))
#define ROL(m,o) (((m) << (o)) | ((m) >> (BLOCK_SIZE-(o))))
#define ROR(m,o) (((m) >> (o)) | ((m) << (BLOCK_SIZE-(o))))

#define MARK(ka,kb,i) (BIT(ka, i) | ((BIT(kb, i)) << 1))

#define CHOOSE(c,m)                                                     \
    switch(c)                                                           \
    {                                                                   \
        case 3: m = ~m;                  break;                         \
        case 1: m = ROL(m,CIPHER_SHIFT); break;                         \
        case 2: m = ROR(m,CIPHER_SHIFT); break;                         \
    }
/*      case 0: m ^= ((*key).v[0]^(*key).h[1]); break;                 */

/* WAVE_ROTOR (C) */
#define WAVE_SHIFT  11

#define ADDITION_MASK_P ((UINT) 0x19062C1485FB27AE) // positive
#define ADDITION_MASK_N ((UINT) 0xE7FAE3EB8B04C961) // negative

#define WAVE_L(m,o) (~(ROL(m,o) + ADDITION_MASK_P) - ADDITION_MASK_N)
#define WAVE_R(m,o) (~(ROR(m,o) - ADDITION_MASK_N) + ADDITION_MASK_P)

#define WAVE_ROTOR(ml,mr)                                                \
    ml = WAVE_L(ml,WAVE_SHIFT);                                          \
    mr = WAVE_R(mr,WAVE_SHIFT);


#define VLINE_GET(mm,m,cp)                                              \
    mm = 0ULL;                           /* cleaning static var */      \
    R lg = BLOCK_SIZE; while(lg--)                                      \
    {                                                                   \
        mm |= BIT(m[lg],cp) << lg;                                      \
    }

#define VLINE_SET(mm,m,cp)                                              \
    R ls = BLOCK_SIZE; while(ls--)                                      \
    {                                                                   \
        m[ls] &= ~(((UINT) 0x01) << cp); /* clean needed bit */         \
        m[ls] |= BIT(mm,ls) << cp;                                      \
    }

static UINT sa_table[BLOCK_SIZE];

void ar4ive_encrypt(UINT * registru, AR4_KEY * key)
{
    static UINT messager = 0ULL;

    R
    scroll_cells = BLOCK_SIZE; while(scroll_cells--)
    {
        CHOOSE(
            MARK(
                (*key).h[0],
                (*key).h[1],
                scroll_cells
                ),
            registru[scroll_cells]
            );
    }

    scroll_cells = BLOCK_SIZE; while(scroll_cells--)
    {
        VLINE_GET(messager, registru, scroll_cells);
        CHOOSE(
            MARK(
                (*key).v[0],
                (*key).v[1],
                scroll_cells
                ),
            messager
            );
        VLINE_SET(messager, registru, scroll_cells);
    }

    scroll_cells = BLOCK_SIZE; while(scroll_cells--)
    {
        registru[scroll_cells] += sa_table[scroll_cells];
    }
}

void ar4ive_decrypt(UINT * registru, AR4_KEY * key)
{
    static UINT messager = 0ULL;

    R
    scroll_cells = BLOCK_SIZE; while(scroll_cells--)
    {
        registru[scroll_cells] -= sa_table[scroll_cells];
    }

    scroll_cells = BLOCK_SIZE; while(scroll_cells--)
    {
        VLINE_GET(messager, registru, scroll_cells);
        CHOOSE(
            MARK(
                (*key).v[1],
                (*key).v[0],
                scroll_cells
                ),
            messager
            );
        VLINE_SET(messager, registru, scroll_cells);
    }

    scroll_cells = BLOCK_SIZE; while(scroll_cells--)
    {
        CHOOSE(
            MARK(
                (*key).h[1],
                (*key).h[0],
                scroll_cells
                ),
            registru[scroll_cells]
            );
    }
}

void sa_box_init(AR4_KEY key)
{
    UINT key1 = key.v[1] + key.h[0];
    UINT key2 = key.v[0] + key.h[1];

    for (int i = 0; i < BLOCK_SIZE; ++i)
    {
       key1 += WAVE_L(key2, WAVE_SHIFT);
       key2 += WAVE_R(key1, BLOCK_SIZE - WAVE_SHIFT);
       
       sa_table[i] += ROL(key1^key2, 3);
       //printf("%llX\n", sa_table[id]);
    }
}

void wave_rotor(AR4_KEY *key)
{
        WAVE_ROTOR((*key).h[1], (*key).h[0]);
        WAVE_ROTOR((*key).v[1], (*key).v[0]);
}

/* n00bo-skill method */
/*
void subrotor_plus(AR4_KEY * subkey, int index)
{
        (*subkey).v[0] += 0xAAAAAAAAAAAAAAAA*index;
        (*subkey).h[1] += 0xAAAAAAAAAAAAAAAA*index;
        (*subkey).h[0] += 0xAAAAAAAAAAAAAAAA*index;
        (*subkey).v[1] += 0xAAAAAAAAAAAAAAAA*index;
}

void subrotor_minus(AR4_KEY * subkey, int index)
{
        (*subkey).v[0] -= 0xAAAAAAAAAAAAAAAA*index;
        (*subkey).h[1] -= 0xAAAAAAAAAAAAAAAA*index;
        (*subkey).h[0] -= 0xAAAAAAAAAAAAAAAA*index;
        (*subkey).v[1] -= 0xAAAAAAAAAAAAAAAA*index; 
}
*/