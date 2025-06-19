/**
  * SpookyV2.h - Port of SpookyHash in the C programming language.
  *
  * Repository: https://github.com/9takoyakii/SpookyHash-c
  * Reference: https://burtleburtle.net/bob/hash/spooky.html
  * All source code is in the public domain, so you are free to use, modify or distribute it.
*/

#ifndef __SPOOKY_HASH_V1_C
#define __SPOOKY_HASH_V1_C

#include <stddef.h>

#ifdef _MSVC_VER

#define INLINE __forceinline

typedef unsigned __int8 SpookyV2_uint8;
typedef unsigned __int16 SpookyV2_uint16;
typedef unsigned __int32 SpookyV2_uint32;
typedef unsigned __int64 SpookyV2_uint64;

#else

#include "stdint.h"

#define INLINE inline

typedef uint8_t SpookyV2_uint8;
typedef uint16_t SpookyV2_uint16;
typedef uint32_t SpookyV2_uint32;
typedef uint64_t SpookyV2_uint64;

#endif

#define SpookyV2_NUM_VARS 12
#define SpookyV2_BLOCK_SIZE 96 // (SpookyV2_NUM_VARS * 8)
#define SpookyV2_BUFF_SIZE 192 // (SpookyV2_BLOCK_SIZE * 2)
#define SpookyV2_CONST 0xdeadbeefdeadbeefLL


struct SpookyV2_State {
    SpookyV2_uint64 *data;
    SpookyV2_uint64 *state;
    size_t length;
    SpookyV2_uint8 remainder;
};

void SpookyV2_hash128(const void *message, size_t length, SpookyV2_uint64 *hash1, SpookyV2_uint64 *hash2);
SpookyV2_uint64 SpookyV2_hash64(const void *message, size_t length, SpookyV2_uint64 seed);
SpookyV2_uint32 SpookyV2_hash32(const void *message, size_t length, SpookyV2_uint32 seed);
void SpookyV2_init(struct SpookyV2_State *state, SpookyV2_uint64 seed1, SpookyV2_uint64 seed2);
void SpookyV2_update(struct SpookyV2_State *state, const void *message, size_t length);
void SpookyV2_final(struct SpookyV2_State *state, SpookyV2_uint64 *hash1, SpookyV2_uint64 *hash2);
void SpookyV2_free(struct SpookyV2_State *state);

#endif
