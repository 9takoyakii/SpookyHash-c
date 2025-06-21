#include "SpookyV1.h"
#include "SpookyV2.h"
#include "tinytest.h"
#include <string.h>

const char *msg1 = "Hello World!!";
SpookyV1_uint64 s1 = 123;
const char *msg2 = "DAKARA ONE TWO THREE DE ARUKIDASE\nKYOU MO ASHITA MO KINOU MO KAWARANAI\nNARABA ONE TWO THREE DE ARUKIDASE\nMAWARU SEKAI TO GYAKU MUKI DE";
SpookyV1_uint64 s2 = 0;

int test_SpookyV1Hash32(const struct ttest_Test *test) {
    ASSERT(SpookyV1_hash32(msg1, strlen(msg1), s1) == 0x712005a8);
    ASSERT(SpookyV1_hash32(msg2, strlen(msg2), s2) == 0x2d9935e2);
    return 0;
}

int test_SpookyV1Hash64(const struct ttest_Test *test) {
    ASSERT(SpookyV1_hash64(msg1, strlen(msg1), s1) == 0x5c9ca081712005a8);
    ASSERT(SpookyV1_hash64(msg2, strlen(msg2), s2) == 0x971fd0d12d9935e2);
    return 0;
}

int test_SpookyV1Hash128(const struct ttest_Test *test) {
    SpookyV1_uint64 h1 = s1;
    SpookyV1_uint64 h2 = 0;
    SpookyV1_hash128(msg1, strlen(msg1), &h1, &h2);
    ASSERT(h1 == 0xa73bdd09c4185fa9 && h2 == 0xe029dddfbb1eff2a);

    h1 = s2;
    h2 = 0;
    SpookyV1_hash128(msg2, strlen(msg2), &h1, &h2);
    ASSERT(h1 == 0x971fd0d12d9935e2 && h2 == 0xff3fa3fc9f6598ef);
    return 0;
}

int test_SpookyV2Hash32(const struct ttest_Test *test) {
    ASSERT(SpookyV2_hash32(msg1, strlen(msg1), s1) == 0xd7605b78);
    ASSERT(SpookyV2_hash32(msg2, strlen(msg2), s2) == 0x8b934657);
    return 0;
}

int test_SpookyV2Hash64(const struct ttest_Test *test) {
    ASSERT(SpookyV2_hash64(msg1, strlen(msg1), s1) == 0x78ed9000d7605b78);
    ASSERT(SpookyV2_hash64(msg2, strlen(msg2), s2) == 0xf4c773c08b934657);
    return 0;
}

int test_SpookyV2Hash128(const struct ttest_Test *test) {
    SpookyV2_uint64 h1 = s1;
    SpookyV2_uint64 h2 = 0;
    SpookyV2_hash128(msg1, strlen(msg1), &h1, &h2);
    ASSERT(h1 == 0xd89b808bd353c2ec && h2 == 0x9bede9164600e157);

    h1 = s2;
    h2 = 0;
    SpookyV2_hash128(msg2, strlen(msg2), &h1, &h2);
    ASSERT(h1 == 0xf4c773c08b934657 && h2 == 0xe898100932d7fa1c);
    return 0;
}

int main() {
    INIT();
    DESCRIBE_BEGIN("Testing SpookyV1.c");
        IT("Testing SpookyV1_Hash32", &test_SpookyV1Hash32);
        IT("Testing SpookyV1_Hash64", &test_SpookyV1Hash64);
        IT("Testing SpookyV1_Hash128", &test_SpookyV1Hash128);
    DESCRIBE_END();

    DESCRIBE_BEGIN("Testing SpookyV2.c");
        IT("Testing SpookyV1_Hash32", &test_SpookyV1Hash32);
        IT("Testing SpookyV1_Hash64", &test_SpookyV1Hash64);
        IT("Testing SpookyV1_Hash128", &test_SpookyV1Hash128);
    DESCRIBE_END();

    return CONCLUDE();
}
