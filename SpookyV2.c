/**
  * SpookyV2.c - Port of SpookyHash in the C programming language.
  *
  * Repository: https://github.com/9takoyakii/SpookyHash-c
  * Reference: https://burtleburtle.net/bob/hash/spooky.html
  * All source code is in the public domain, so you are free to use, modify or distribute it.
*/

#include "SpookyV2.h"
#include <string.h>
#include <stdlib.h>

#define ALLOW_UNALIGNED_READS 1

static SpookyV2_INLINE SpookyV2_uint64 Rot64(SpookyV2_uint64 x, int k) {
    return (x << k) | (x >> (64 - k));
}

static SpookyV2_INLINE void Mix(
    const SpookyV2_uint64 *data,
    SpookyV2_uint64 *s0, SpookyV2_uint64 *s1, SpookyV2_uint64 *s2,  SpookyV2_uint64 *s3,
    SpookyV2_uint64 *s4, SpookyV2_uint64 *s5, SpookyV2_uint64 *s6,  SpookyV2_uint64 *s7,
    SpookyV2_uint64 *s8, SpookyV2_uint64 *s9, SpookyV2_uint64 *s10, SpookyV2_uint64 *s11
) {
  *s0 += data[0];    *s2 ^= *s10;  *s11 ^= *s0;  *s0 = Rot64(*s0, 11);    *s11 += *s1;
  *s1 += data[1];    *s3 ^= *s11;  *s0 ^= *s1;   *s1 = Rot64(*s1, 32);    *s0 += *s2;
  *s2 += data[2];    *s4 ^= *s0;   *s1 ^= *s2;   *s2 = Rot64(*s2, 43);    *s1 += *s3;
  *s3 += data[3];    *s5 ^= *s1;   *s2 ^= *s3;   *s3 = Rot64(*s3, 31);    *s2 += *s4;
  *s4 += data[4];    *s6 ^= *s2;   *s3 ^= *s4;   *s4 = Rot64(*s4, 17);    *s3 += *s5;
  *s5 += data[5];    *s7 ^= *s3;   *s4 ^= *s5;   *s5 = Rot64(*s5, 28);    *s4 += *s6;
  *s6 += data[6];    *s8 ^= *s4;   *s5 ^= *s6;   *s6 = Rot64(*s6, 39);    *s5 += *s7;
  *s7 += data[7];    *s9 ^= *s5;   *s6 ^= *s7;   *s7 = Rot64(*s7, 57);    *s6 += *s8;
  *s8 += data[8];    *s10 ^= *s6;  *s7 ^= *s8;   *s8 = Rot64(*s8, 55);    *s7 += *s9;
  *s9 += data[9];    *s11 ^= *s7;  *s8 ^= *s9;   *s9 = Rot64(*s9, 54);    *s8 += *s10;
  *s10 += data[10];  *s0 ^= *s8;   *s9 ^= *s10;  *s10 = Rot64(*s10, 22);  *s9 += *s11;
  *s11 += data[11];  *s1 ^= *s9;  *s10 ^= *s11;  *s11 = Rot64(*s11, 46);  *s10 += *s0;
}

static SpookyV2_INLINE void EndPartial(
    SpookyV2_uint64 *h0, SpookyV2_uint64 *h1, SpookyV2_uint64 *h2,  SpookyV2_uint64 *h3,
    SpookyV2_uint64 *h4, SpookyV2_uint64 *h5, SpookyV2_uint64 *h6,  SpookyV2_uint64 *h7,
    SpookyV2_uint64 *h8, SpookyV2_uint64 *h9, SpookyV2_uint64 *h10, SpookyV2_uint64 *h11
) {
    *h11 += *h1;    *h2 ^= *h11;   *h1 = Rot64(*h1,44);
    *h0 += *h2;    *h3 ^= *h0;    *h2 = Rot64(*h2,15);
    *h1 += *h3;    *h4 ^= *h1;    *h3 = Rot64(*h3,34);
    *h2 += *h4;    *h5 ^= *h2;    *h4 = Rot64(*h4,21);
    *h3 += *h5;    *h6 ^= *h3;    *h5 = Rot64(*h5,38);
    *h4 += *h6;    *h7 ^= *h4;    *h6 = Rot64(*h6,33);
    *h5 += *h7;    *h8 ^= *h5;    *h7 = Rot64(*h7,10);
    *h6 += *h8;    *h9 ^= *h6;    *h8 = Rot64(*h8,13);
    *h7 += *h9;    *h10^= *h7;    *h9 = Rot64(*h9,38);
    *h8 += *h10;   *h11^= *h8;    *h10= Rot64(*h10,53);
    *h9 += *h11;   *h0 ^= *h9;    *h11= Rot64(*h11,42);
    *h10+= *h0;    *h1 ^= *h10;   *h0 = Rot64(*h0,54);
}

static SpookyV2_INLINE void End(
    const SpookyV2_uint64 *data,
    SpookyV2_uint64 *h0, SpookyV2_uint64 *h1, SpookyV2_uint64 *h2,  SpookyV2_uint64 *h3,
    SpookyV2_uint64 *h4, SpookyV2_uint64 *h5, SpookyV2_uint64 *h6,  SpookyV2_uint64 *h7,
    SpookyV2_uint64 *h8, SpookyV2_uint64 *h9, SpookyV2_uint64 *h10, SpookyV2_uint64 *h11
) {
    h0 += data[0];   h1 += data[1];   h2 += data[2];    h3 += data[3];
    h4 += data[4];   h5 += data[5];   h6 += data[6];    h7 += data[7];
    h8 += data[8];   h9 += data[9];   h10 += data[10];  h11 += data[11];
    EndPartial(h0, h1, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11);
    EndPartial(h0, h1, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11);
    EndPartial(h0, h1, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11);
}

static SpookyV2_INLINE void ShortMix(SpookyV2_uint64 *h0, SpookyV2_uint64 *h1, SpookyV2_uint64 *h2, SpookyV2_uint64 *h3) {
    *h2 = Rot64(*h2, 50);  *h2 += *h3;  *h0 ^= *h2;
    *h3 = Rot64(*h3, 52);  *h3 += *h0;  *h1 ^= *h3;
    *h0 = Rot64(*h0, 30);  *h0 += *h1;  *h2 ^= *h0;
    *h1 = Rot64(*h1, 41);  *h1 += *h2;  *h3 ^= *h1;
    *h2 = Rot64(*h2, 54);  *h2 += *h3;  *h0 ^= *h2;
    *h3 = Rot64(*h3, 48);  *h3 += *h0;  *h1 ^= *h3;
    *h0 = Rot64(*h0, 38);  *h0 += *h1;  *h2 ^= *h0;
    *h1 = Rot64(*h1, 37);  *h1 += *h2;  *h3 ^= *h1;
    *h2 = Rot64(*h2, 62);  *h2 += *h3;  *h0 ^= *h2;
    *h3 = Rot64(*h3, 34);  *h3 += *h0;  *h1 ^= *h3;
    *h0 = Rot64(*h0, 5);   *h0 += *h1;  *h2 ^= *h0;
    *h1 = Rot64(*h1, 36);  *h1 += *h2;  *h3 ^= *h1;
}

static SpookyV2_INLINE void ShortEnd(
    SpookyV2_uint64 *h0, SpookyV2_uint64 *h1, SpookyV2_uint64 *h2, SpookyV2_uint64 *h3
) {
    *h3 ^= *h2;  *h2 = Rot64(*h2, 15);  *h3 += *h2;
    *h0 ^= *h3;  *h3 = Rot64(*h3, 52);  *h0 += *h3;
    *h1 ^= *h0;  *h0 = Rot64(*h0, 26);  *h1 += *h0;
    *h2 ^= *h1;  *h1 = Rot64(*h1, 51);  *h2 += *h1;
    *h3 ^= *h2;  *h2 = Rot64(*h2, 28);  *h3 += *h2;
    *h0 ^= *h3;  *h3 = Rot64(*h3, 9);   *h0 += *h3;
    *h1 ^= *h0;  *h0 = Rot64(*h0, 47);  *h1 += *h0;
    *h2 ^= *h1;  *h1 = Rot64(*h1, 54);  *h2 += *h1;
    *h3 ^= *h2;  *h2 = Rot64(*h2, 32);  *h3 += *h2;
    *h0 ^= *h3;  *h3 = Rot64(*h3, 25);  *h0 += *h3;
    *h1 ^= *h0;  *h0 = Rot64(*h0, 63);  *h1 += *h0;
}

static void Short(const void *message, size_t length, SpookyV2_uint64 *hash1, SpookyV2_uint64 *hash2) {
    union {
        const SpookyV2_uint8 *p8;
        SpookyV2_uint32 *p32;
        SpookyV2_uint64 *p64;
        size_t i;
    } u;
    u.p8 = (const SpookyV2_uint8 *) message;
    SpookyV2_uint64 buff[2 * SpookyV2_BLOCK_SIZE];

    if (!ALLOW_UNALIGNED_READS && (u.i & 0x7)) {
        memcpy(buff, message, length);
        u.p64 = buff;
    }

    size_t r = length % 32;
    SpookyV2_uint64 a = *hash1;
    SpookyV2_uint64 b = *hash2;
    SpookyV2_uint64 c = SpookyV2_CONST;
    SpookyV2_uint64 d = SpookyV2_CONST;

    if (length > 15) {
        const SpookyV2_uint64 *end = u.p64 + (length / 32) * 4;

        for (; u.p64 < end; u.p64 += 4) {
            c += u.p64[0];
            d += u.p64[1];
            ShortMix(&a, &b, &c, &d);
            a += u.p64[2];
            b += u.p64[3];
        }

        if (r >= 16) {
            c += u.p64[0];
            d += u.p64[1];
            ShortMix(&a, &b, &c, &d);
            u.p64 += 2;
            r -= 16;
        }
    }

    d += ((SpookyV2_uint64)length) << 56;
    switch (r) {
        case 15: d += ((SpookyV2_uint64) u.p8[14]) << 48;
        case 14: d += ((SpookyV2_uint64) u.p8[13]) << 40;
        case 13: d += ((SpookyV2_uint64) u.p8[12]) << 32;
        case 12:
            d += u.p32[2];
            c += u.p64[0];
            break;
        case 11: d += ((SpookyV2_uint64)u.p8[10]) << 16;
        case 10: d += ((SpookyV2_uint64)u.p8[9]) << 8;
        case 9: d += (SpookyV2_uint64)u.p8[8];
        case 8:
            c += u.p64[0];
            break;
        case 7: c += ((SpookyV2_uint64)u.p8[6]) << 48;
        case 6: c += ((SpookyV2_uint64)u.p8[5]) << 40;
        case 5: c += ((SpookyV2_uint64)u.p8[4]) << 32;
        case 4:
            c += u.p32[0];
            break;
        case 3: c += ((SpookyV2_uint64)u.p8[2]) << 16;
        case 2: c += ((SpookyV2_uint64)u.p8[1]) << 8;
        case 1:
            c += (SpookyV2_uint64)u.p8[0];
            break;
        case 0:
            c += SpookyV2_CONST;
            d += SpookyV2_CONST;
    }

    ShortEnd(&a, &b, &c, &d);
    *hash1 = a;
    *hash2 = b;
}

void SpookyV2_hash128(const void *message, size_t length, SpookyV2_uint64 *hash1, SpookyV2_uint64 *hash2) {
    if (length < SpookyV2_BUFF_SIZE) {
        Short(message, length, hash1, hash2);
        return;
    }

    SpookyV2_uint64 h0, h1, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11;
    SpookyV2_uint64 buf[SpookyV2_NUM_VARS];
    SpookyV2_uint64 *end;
    union {
        const SpookyV2_uint8 *p8;
        SpookyV2_uint64 *p64;
        size_t i;
    } u;
    size_t r;

    h0 = h3 = h6 = h9 = *hash1;
    h1 = h4 = h7 = h10 = *hash2;
    h2 = h5 = h8 = h11 = SpookyV2_CONST;

    u.p8 = (const SpookyV2_uint8 *) message;
    end = u.p64 + (length / SpookyV2_BLOCK_SIZE) * SpookyV2_NUM_VARS;

    if (ALLOW_UNALIGNED_READS || !(u.i & 0x7)) {
        while (u.p64 < end) {
            Mix(u.p64, &h0, &h1, &h2, &h3, &h4, &h5, &h6, &h7, &h8, &h9, &h10, &h11);
            u.p64 += SpookyV2_NUM_VARS;
        }
    } else {
        while (u.p64 < end) {
            memcpy(buf, u.p64, SpookyV2_BLOCK_SIZE);
            Mix(buf, &h0, &h1, &h2, &h3, &h4, &h5, &h6, &h7, &h8, &h9, &h10, &h11);
            u.p64 += SpookyV2_NUM_VARS;
        }
    }

    r = (length - ((const SpookyV2_uint8 *) end - (const SpookyV2_uint8 *) message));
    memcpy(buf, end, r);
    memset(((SpookyV2_uint8 *) buf) + r, 0, SpookyV2_BLOCK_SIZE - r);
    ((SpookyV2_uint8 *) buf)[SpookyV2_BLOCK_SIZE - 1] = r;
    End(buf, &h0, &h1, &h2, &h3, &h4, &h5, &h6, &h7, &h8, &h9, &h10, &h11);
    *hash1 = h0;
    *hash2 = h1;
}

SpookyV2_uint64 SpookyV2_hash64(const void *message, size_t length, SpookyV2_uint64 seed) {
    SpookyV2_uint64 hash1 = seed;
    SpookyV2_hash128(message, length, &hash1, &seed);
    return hash1;
}

SpookyV2_uint32 SpookyV2_hash32(const void *message, size_t length, SpookyV2_uint32 seed) {
    SpookyV2_uint64 hash1 = seed, hash2 = seed;
    SpookyV2_hash128(message, length, &hash1, &hash2);
    return hash1;
}

void SpookyV2_init(struct SpookyV2_State *state, SpookyV2_uint64 seed1, SpookyV2_uint64 seed2) {
    state->data = calloc(2 * SpookyV2_NUM_VARS, sizeof(SpookyV2_uint64));
    state->state = calloc(SpookyV2_NUM_VARS, sizeof(SpookyV2_uint64));
    state->length = 0;
    state->remainder = 0;
    state->state[0] = seed1;
    state->state[1] = seed2;
}

void SpookyV2_update(struct SpookyV2_State *state, const void *message, size_t length) {
    SpookyV2_uint64 h0, h1, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11;
    size_t newLength = length + state->remainder;
    SpookyV2_uint8  r;
    union {
        const SpookyV2_uint8 *p8;
        SpookyV2_uint64 *p64;
        size_t i;
    } u;
    const SpookyV2_uint64 *end;

    if (newLength < SpookyV2_BUFF_SIZE) {
        memcpy(&((SpookyV2_uint8 *) state->data)[state->remainder], message, length);
        state->length += length;
        state->remainder = (SpookyV2_uint8)newLength;
        return;
    }

    if (state->length < SpookyV2_BUFF_SIZE) {
        h0 = h3 = h6 = h9 = state->state[0];
        h1 = h4 = h7 = h10 = state->state[1];
        h2 = h5 = h8 = h11 = SpookyV2_CONST;
    } else {
        h0 = state->state[0];
        h1 = state->state[1];
        h2 = state->state[2];
        h3 = state->state[3];
        h4 = state->state[4];
        h5 = state->state[5];
        h6 = state->state[6];
        h7 = state->state[7];
        h8 = state->state[8];
        h9 = state->state[9];
        h10 = state->state[10];
        h11 = state->state[11];
    }
    state->length += length;

    if (state->remainder) {
        SpookyV2_uint8 prefix = SpookyV2_BUFF_SIZE - state->remainder;
        memcpy(&(((SpookyV2_uint8 *) state->data)[state->remainder]), message, prefix);
        u.p64 = state->data;
        Mix(u.p64, &h0, &h1, &h2, &h3, &h4, &h5, &h6, &h7, &h8, &h9, &h10, &h11);
        Mix(&u.p64[SpookyV2_NUM_VARS], &h0, &h1, &h2, &h3, &h4, &h5, &h6, &h7, &h8, &h9, &h10, &h11);
        u.p8 = ((const SpookyV2_uint8 *) message) + prefix;
        length -= prefix;
    } else {
        u.p8 = (const SpookyV2_uint8 *) message;
    }

    end = u.p64 + (length / SpookyV2_BLOCK_SIZE) * SpookyV2_NUM_VARS;
    r = (SpookyV2_uint8) (length - ((const SpookyV2_uint8 *) end - u.p8));
    if (ALLOW_UNALIGNED_READS || (u.i & 0x7) == 0) {
        while (u.p64 < end) {
            Mix(u.p64, &h0, &h1, &h2, &h3, &h4, &h5, &h6, &h7, &h8, &h9, &h10, &h11);
            u.p64 += SpookyV2_NUM_VARS;
        }
    } else {
        while (u.p64 < end) {
            memcpy(state->data, u.p8, SpookyV2_BLOCK_SIZE);
            Mix(state->data, &h0, &h1, &h2, &h3, &h4, &h5, &h6, &h7, &h8, &h9, &h10, &h11);
            u.p64 += SpookyV2_NUM_VARS;
        }
    }

    state->remainder = r;
    memcpy(state->data, end, r);

    state->state[0] = h0;
    state->state[1] = h1;
    state->state[2] = h2;
    state->state[3] = h3;
    state->state[4] = h4;
    state->state[5] = h5;
    state->state[6] = h6;
    state->state[7] = h7;
    state->state[8] = h8;
    state->state[9] = h9;
    state->state[10] = h10;
    state->state[11] = h11;
}

void SpookyV2_final(struct SpookyV2_State *state, SpookyV2_uint64 *hash1, SpookyV2_uint64 *hash2) {
    if (state->length < SpookyV2_BUFF_SIZE) {
        *hash1 = state->state[0];
        *hash2 = state->state[1];
        Short(state->data, state->length, hash1, hash2);
        return;
    }

    const SpookyV2_uint64 *data = (const SpookyV2_uint64 *) state->data;
    SpookyV2_uint8 r = state->remainder;
    SpookyV2_uint64 h0 = state->state[0];
    SpookyV2_uint64 h1 = state->state[1];
    SpookyV2_uint64 h2 = state->state[2];
    SpookyV2_uint64 h3 = state->state[3];
    SpookyV2_uint64 h4 = state->state[4];
    SpookyV2_uint64 h5 = state->state[5];
    SpookyV2_uint64 h6 = state->state[6];
    SpookyV2_uint64 h7 = state->state[7];
    SpookyV2_uint64 h8 = state->state[8];
    SpookyV2_uint64 h9 = state->state[9];
    SpookyV2_uint64 h10 = state->state[10];
    SpookyV2_uint64 h11 = state->state[11];

    if (r >= SpookyV2_BLOCK_SIZE) {
        Mix(data, &h0, &h1, &h2, &h3, &h4, &h5, &h6, &h7, &h8, &h9, &h10, &h11);
        data += SpookyV2_NUM_VARS;
        r -= SpookyV2_BLOCK_SIZE;
    }

    memset(&((SpookyV2_uint8 *) data)[r], 0, SpookyV2_BLOCK_SIZE - r);
    ((SpookyV2_uint8 *) data)[SpookyV2_BLOCK_SIZE - 1] = r;
    End(data, &h0, &h1, &h2, &h3, &h4, &h5, &h6, &h7, &h8, &h9, &h10, &h11);

    *hash1 = h0;
    *hash2 = h1;
}

void SpookyV2_free(struct SpookyV2_State *state) {
    free(state->data);
    free(state->state);
    state->length = 0;
    state->remainder = 0;
}
