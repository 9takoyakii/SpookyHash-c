/**
  * SpookyV1.h - Port of SpookyHash in the C programming language.
  *
  * Repository: https://github.com/9takoyakii/SpookyHash-c
  * Reference: https://burtleburtle.net/bob/hash/spooky.html
  * All source code is in the public domain, so you are free to use, modify or distribute it.
*/

#ifndef __SPOOKY_HASH_V1_C
#define __SPOOKY_HASH_V1_C

#include <stddef.h>

#ifdef _MSVC_VER

#define SpookyV1_INLINE __forceinline

typedef unsigned __int8 SpookyV1_uint8;
typedef unsigned __int16 SpookyV1_uint16;
typedef unsigned __int32 SpookyV1_uint32;
typedef unsigned __int64 SpookyV1_uint64;

#else

#include "stdint.h"

#define SpookyV1_INLINE inline

typedef uint8_t SpookyV1_uint8;
typedef uint16_t SpookyV1_uint16;
typedef uint32_t SpookyV1_uint32;
typedef uint64_t SpookyV1_uint64;

#endif

#define SpookyV1_NUM_VARS 12
#define SpookyV1_BLOCK_SIZE 96 // (SpookyV1_NUM_VARS * 8)
#define SpookyV1_BUFF_SIZE 192 // (SpookyV1_BLOCK_SIZE * 2)
#define SpookyV1_CONST 0xdeadbeefdeadbeefLL


struct SpookyV1_State {
    SpookyV1_uint64 *data;
    SpookyV1_uint64 *state;
    size_t length;
    SpookyV1_uint8 remainder;
};

void SpookyV1_hash128(const void *message, size_t length, SpookyV1_uint64 *hash1, SpookyV1_uint64 *hash2);
SpookyV1_uint64 SpookyV1_hash64(const void *message, size_t length, SpookyV1_uint64 seed);
SpookyV1_uint32 SpookyV1_hash32(const void *message, size_t length, SpookyV1_uint32 seed);
void SpookyV1_init(struct SpookyV1_State *state, SpookyV1_uint64 seed1, SpookyV1_uint64 seed2);
void SpookyV1_update(struct SpookyV1_State *state, const void *message, size_t length);
void SpookyV1_final(struct SpookyV1_State *state, SpookyV1_uint64 *hash1, SpookyV1_uint64 *hash2);
void SpookyV1_free(struct SpookyV1_State *state);

#endif
