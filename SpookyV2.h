/**
  * SpookyV2.h - Port of SpookyHash in the C programming language.
  *
  * Repository: https://github.com/9takoyakii/SpookyHash-c
  * Reference: https://burtleburtle.net/bob/hash/spooky.html
  * All source code is in the public domain, so you are free to use, modify or distribute it.
*/

#ifndef __SPOOKY_HASH_V2_C
#define __SPOOKY_HASH_V2_C

#include <stddef.h>

#ifdef _MSVC_VER

#define SpookyV2_INLINE __forceinline

typedef unsigned __int8 SpookyV2_uint8;
typedef unsigned __int16 SpookyV2_uint16;
typedef unsigned __int32 SpookyV2_uint32;
typedef unsigned __int64 SpookyV2_uint64;

#else

#include "stdint.h"

#define SpookyV2_INLINE inline

typedef uint8_t SpookyV2_uint8;
typedef uint16_t SpookyV2_uint16;
typedef uint32_t SpookyV2_uint32;
typedef uint64_t SpookyV2_uint64;

#endif

extern SpookyV2_uint16 SpookyV2_NUM_VARS;
extern SpookyV2_uint16 SpookyV2_BLOCK_SIZE;
extern SpookyV2_uint16 SpookyV2_BUFF_SIZE;
extern SpookyV2_uint64 SpookyV2_CONST;

struct SpookyV2_State {
    SpookyV2_uint16 numVars;
    SpookyV2_uint16 blockSize;
    SpookyV2_uint16 buffSize;
    SpookyV2_uint64 magicConst;
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
