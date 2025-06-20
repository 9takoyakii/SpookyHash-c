What is this?
--------------------------
SpookyHash-c is a port of SpookyHash in C programming language.

SpookyHash-c is written using the original code as its base (https://burtleburtle.net/bob/hash/spooky.html).

All source code in this repository is public domain, so you are free to use it, modify it and distribute it.

SpookyHash-c has almost the same interface as the original SpookyHash.

Functions, Variables, Types, and Macros:
--------------------------
** SpookyV1.c:
//////////////////////////
**** Variables:
==========================
1. SpookyV1_uint16 SpookyV1_NUM_VARS:
    Number of uint64's in internal state.

2. SpookyV1_uint16 SpookyV1_BLOCK_SIZE:
    Size of the internal state

3. SpookyV1_uint16 SpookyV1_BUFF_SIZE:
    Size of buffer of unhashed data, in bytes.

4. SpookyV1_uint64 SpookyV1_CONST:
    A constant which:
        * Is not zero.
        * Is odd.
        * Is a not-very-regular mix of 1's and 0's.
        * Does not need any other special mathematical properties.

**** Macros:
==========================
1. SpookyV1_INLINE:
    Internal macro of inline keyword.

**** Types:
==========================
1. SpookyV1_uint8:
    Unsigned integer of length 8 bits.

2. SpookyV1_uint16:
    Unsigned integer of length 16 bits.

3. SpookyV1_uint32:
    Unsigned integer of length 32 bits.

4. SpookyV1_uint64:
    Unsigned integer of length 64 bits.

5. SpookyV1_State:
    Internal state of SpookyHash.

**** Functions:
==========================
1. void SpookyV1_hash128(const void *message, size_t length, SpookyV1_uint64 *hash1, SpookyV1_uint64 *hash2):
    Function to hash a message in 1 call and produce a hash result of 128 bits in size.

2. SpookyV1_uint64 SpookyV1_hash64(const void *message, size_t length, SpookyV1_uint64 seed):
    Function to hash a message in 1 call and produce a hash result of 64 bits in size.

3. SpookyV1_uint32 SpookyV1_hash32(const void *message, size_t length, SpookyV1_uint32 seed):
    Function to hash a message in 1 call and produce a hash result of 32 bits in size.

4. void SpookyV1_init(struct SpookyV1_State *state, SpookyV1_uint64 seed1, SpookyV1_uint64 seed2):
    Function to initialize the internal state with seed1 and seed2.

5. void SpookyV1_update(struct SpookyV1_State *state, const void *message, size_t length):
    Function to update the internal state by adding a message to the internal state.

6. void SpookyV1_final(struct SpookyV1_State *state, SpookyV1_uint64 *hash1, SpookyV1_uint64 *hash2):
    Function that provides a hash result of the current internal state.

7. void SpookyV1_free(struct SpookyV1_State *state):
    Function to free SpookyV1_State.


** SpookyV2.c:
//////////////////////////
**** Variables:
==========================
1. SpookyV1_uint16 SpookyV1_NUM_VARS:
    Number of uint64's in internal state.

2. SpookyV1_uint16 SpookyV1_BLOCK_SIZE:
    Size of the internal state

3. SpookyV1_uint16 SpookyV1_BUFF_SIZE:
    Size of buffer of unhashed data, in bytes.

4. SpookyV1_uint64 SpookyV1_CONST:
    A constant which:
        * Is not zero.
        * Is odd.
        * Is a not-very-regular mix of 1's and 0's.
        * Does not need any other special mathematical properties.

**** Macros:
==========================
1. SpookyV1_INLINE:
    Internal macro of inline keyword.

**** Types:
==========================
1. SpookyV2_uint8:
    Unsigned integer of length 8 bits.

2. SpookyV2_uint16:
    Unsigned integer of length 16 bits.

3. SpookyV2_uint32:
    Unsigned integer of length 32 bits.

4. SpookyV2_uint64:
    Unsigned integer of length 64 bits.

5. SpookyV2_State:
    Internal state of SpookyHash.

**** Functions:
==========================
1. void SpookyV2_hash128(const void *message, size_t length, SpookyV2_uint64 *hash1, SpookyV2_uint64 *hash2):
    Function to hash a message in 1 call and produce a hash result of 128 bits in size.

2. SpookyV2_uint64 SpookyV2_hash64(const void *message, size_t length, SpookyV2_uint64 seed):
    Function to hash a message in 1 call and produce a hash result of 64 bits in size.

3. SpookyV2_uint32 SpookyV2_hash32(const void *message, size_t length, SpookyV2_uint32 seed):
    Function to hash a message in 1 call and produce a hash result of 32 bits in size.

4. void SpookyV2_init(struct SpookyV2_State *state, SpookyV2_uint64 seed1, SpookyV2_uint64 seed2):
    Function to initialize the internal state with seed1 and seed2.

5. void SpookyV2_update(struct SpookyV2_State *state, const void *message, size_t length):
    Function to update the internal state by adding a message to the internal state.

6. void SpookyV2_final(struct SpookyV2_State *state, SpookyV2_uint64 *hash1, SpookyV2_uint64 *hash2):
    Function that provides a hash result of the current internal state.

7. void SpookyV2_free(struct SpookyV2_State *state):
    Function to free SpookyV2_State.

Reference
--------------------------
SpookyHash Website: https://burtleburtle.net/bob/hash/spooky.html
