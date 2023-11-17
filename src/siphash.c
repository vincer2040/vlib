/*
   SipHash reference C implementation

   Copyright (c) 2012-2016 Jean-Philippe Aumasson
   <jeanphilippe.aumasson@gmail.com>
   Copyright (c) 2012-2014 Daniel J. Bernstein <djb@cr.yp.to>
   Copyright (c) 2017 Salvatore Sanfilippo <antirez@gmail.com>

   To the extent possible under law, the author(s) have dedicated all copyright
   and related and neighboring rights to this software to the public domain
   worldwide. This software is distributed without any warranty.

   You should have received a copy of the CC0 Public Domain Dedication along
   with this software. If not, see
   <http://creativecommons.org/publicdomain/zero/1.0/>.

   ----------------------------------------------------------------------------

   This version was modified by Vince Ronnebaum <ronnebaumvince@gmail.com>
   in the following ways:

   1. Use of SipHash 1-2.

   2. Function returns a uint64_t rather than writing to an output

   3. Remove debugging code.

   4. Some formatting preferences

 */
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#if defined(__has_attribute)
#if __has_attribute(no_sanitize)
#define NO_SANITIZE(sanitizer) __attribute__((no_sanitize(sanitizer)))
#endif
#endif

#if !defined(NO_SANITIZE)
#define NO_SANITIZE(sanitizer)
#endif

/* Test of the CPU is Little Endian and supports not aligned accesses.
 * Two interesting conditions to speedup the function that happen to be
 * in most of x86 servers. */
#if defined(__X86_64__) || defined(__x86_64__) || defined(__i386__) ||         \
    defined(__aarch64__) || defined(__arm64__)
#define UNALIGNED_LE_CPU
#endif

#define ROTL(x, b) (uint64_t)(((x) << (b)) | ((x) >> (64 - (b))))

#define U32TO8_LE(p, v)                                                        \
    (p)[0] = (uint8_t)((v));                                                   \
    (p)[1] = (uint8_t)((v) >> 8);                                              \
    (p)[2] = (uint8_t)((v) >> 16);                                             \
    (p)[3] = (uint8_t)((v) >> 24);

#define U64TO8_LE(p, v)                                                        \
    U32TO8_LE((p), (uint32_t)((v)));                                           \
    U32TO8_LE((p) + 4, (uint32_t)((v) >> 32));

#ifdef UNALIGNED_LE_CPU
#define U8TO64_LE(p) (*((uint64_t*)(p)))
#else
#define U8TO64_LE(p)                                                           \
    (((uint64_t)((p)[0])) | ((uint64_t)((p)[1]) << 8) |                        \
     ((uint64_t)((p)[2]) << 16) | ((uint64_t)((p)[3]) << 24) |                 \
     ((uint64_t)((p)[4]) << 32) | ((uint64_t)((p)[5]) << 40) |                 \
     ((uint64_t)((p)[6]) << 48) | ((uint64_t)((p)[7]) << 56))
#endif

#define U8TO64_LE_NOCASE(p)                                                    \
    (((uint64_t)(siptlw((p)[0]))) | ((uint64_t)(siptlw((p)[1])) << 8) |        \
     ((uint64_t)(siptlw((p)[2])) << 16) | ((uint64_t)(siptlw((p)[3])) << 24) | \
     ((uint64_t)(siptlw((p)[4])) << 32) | ((uint64_t)(siptlw((p)[5])) << 40) | \
     ((uint64_t)(siptlw((p)[6])) << 48) | ((uint64_t)(siptlw((p)[7])) << 56))

#define SIPROUND                                                               \
    do {                                                                       \
        v0 += v1;                                                              \
        v1 = ROTL(v1, 13);                                                     \
        v1 ^= v0;                                                              \
        v0 = ROTL(v0, 32);                                                     \
        v2 += v3;                                                              \
        v3 = ROTL(v3, 16);                                                     \
        v3 ^= v2;                                                              \
        v0 += v3;                                                              \
        v3 = ROTL(v3, 21);                                                     \
        v3 ^= v0;                                                              \
        v2 += v1;                                                              \
        v1 = ROTL(v1, 17);                                                     \
        v1 ^= v2;                                                              \
        v2 = ROTL(v2, 32);                                                     \
    } while (0)

NO_SANITIZE("alignment")
uint64_t siphash(const uint8_t* in, const size_t inlen, const uint8_t* k) {
#ifndef UNALIGNED_LE_CPU
    uint64_t hash;
    uint8_t* out = (uint8_t*)&hash;
#endif
    uint64_t v0 = 0x736f6d6570736575;
    uint64_t v1 = 0x646f72616e646f6d;
    uint64_t v2 = 0x6c7967656e657261;
    uint64_t v3 = 0x7465646279746573;
    uint64_t k0 = U8TO64_LE(k);
    uint64_t k1 = U8TO64_LE(k + 8);
    uint64_t m;
    const uint8_t* end = in + inlen - (inlen % sizeof(uint64_t));
    const int left = inlen & 7;
    uint64_t b = ((uint64_t)inlen) << 56;
    v3 ^= k1;
    v2 ^= k0;
    v1 ^= k1;
    v0 ^= k0;

    for (; in != end; in += 8) {
        m = U8TO64_LE(in);
        v3 ^= m;

        SIPROUND;

        v0 ^= m;
    }

    switch (left) {
    case 7:
        b |= ((uint64_t)in[6]) << 48; /* fall-thru */
    case 6:
        b |= ((uint64_t)in[5]) << 40; /* fall-thru */
    case 5:
        b |= ((uint64_t)in[4]) << 32; /* fall-thru */
    case 4:
        b |= ((uint64_t)in[3]) << 24; /* fall-thru */
    case 3:
        b |= ((uint64_t)in[2]) << 16; /* fall-thru */
    case 2:
        b |= ((uint64_t)in[1]) << 8; /* fall-thru */
    case 1:
        b |= ((uint64_t)in[0]);
        break;
    case 0:
        break;
    }

    v3 ^= b;

    SIPROUND;

    v0 ^= b;
    v2 ^= 0xff;

    SIPROUND;
    SIPROUND;

    b = v0 ^ v1 ^ v2 ^ v3;
#ifndef UNALIGNED_LE_CPU
    U64TO8_LE(out, b);
    return hash;
#else
    return b;
#endif
}
