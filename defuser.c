#include "defuser.h"

#include <inttypes.h>
#include <stdio.h>

#define MAX_FIELDS 8

/*
 * 把 values 中的 count 个整数写成一行“空格分隔的无符号十进制整数”，存入 out。
 * 成功返回 0；count 为 0（还没有实现）、snprintf 出错或结果被截断时返回非零。
 */
static int write_answer(char *out, size_t cap, const uint32_t *values, size_t count) {
    size_t used = 0;
    for (size_t i = 0; i < count; ++i) {
        int size = snprintf(out + used, cap - used, "%s%" PRIu32, i ? " " : "", values[i]);
        if (size < 0 || (size_t)size >= cap - used) return 1;
        used += (size_t)size;
    }
    return count == 0;
}

int solve_phase_1(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_01 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
	uint32_t high = (uint32_t)(challenge >> 32);
	uint32_t mid = (uint32_t)(challenge >> 16);
	uint32_t low = (uint32_t)challenge;

	uint32_t a = (mid ^ 0x9e28u) + 4u * high;
	uint32_t b = (low + 8u * high) ^ 0x66e8u;

	values[0] = (a + 0xd1dbu) ^ b;
	count = 1;
    return write_answer(out, cap, values, count);
}

int solve_phase_2(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_02 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    values[0] = 3u * (uint32_t)((challenge >> 27) & 0xffu);

	uint64_t x = challenge;
	uint32_t shift = 15;

	for (uint32_t i = 1; i < 6; i++) {
	    x >>= shift;
	    uint32_t t = (uint32_t)((challenge >> shift) & 0xfu);

	    values[i] = 5u * (values[i - 1] + i) - t;

	    shift += 5;
	}

	count = 6;
    return write_answer(out, cap, values, count);
}

int solve_phase_3(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_03 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
	uint32_t index = (uint32_t)challenge & 7u;

	uint32_t key = (uint32_t)(challenge >> 32) ^ (uint32_t)(challenge >> 3);

	uint32_t result;

	switch (index) {
	    case 0:
	        result = 5u * key + 0xafffu;
        	break;

	    case 1:
	        result = 3u * key + 0xe084u;
        	break;

	    case 2:
	        result = 5u * key + 0xde6cu;
        	break;

	    case 3:
	        result = 5u * key + 0x13df3u;
        	break;

	    case 4:
	        result = (key ^ 0xb640u) + (key >> 1);
        	break;

	    case 5:
	        result = (2u * key) ^ 0x7ee7u;
        	break;

	    case 6:
	        result = (4u * key) ^ 0x74bdu;
        	break;

	    case 7:
        	result = (key ^ 0x8244u) + (key >> 2);
        	break;

	    default:
	        result = 0;
        	break;
	}

	values[0] = index;
	values[1] = result;
	count = 2;
    return write_answer(out, cap, values, count);
}

static uint32_t solve_p4_walk(uint32_t target, uint32_t low, uint32_t high, uint32_t key)
{
    uint32_t mid = low + (high - low) / 2u;

    if (target == mid) {
        return (mid + 100u) ^ key;
    }

    uint32_t r;

    if (target < mid) {
        r = solve_p4_walk(target, low, mid - 1u, key);
        return (5u * r) ^ (mid + key) ^ 0x1au;
    } else {
        r = solve_p4_walk(target, mid + 1u, high, key);
        return (5u * r) ^ (mid + key) ^ 0x19u;
    }
}

int solve_phase_4(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_04 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    uint32_t target = (uint32_t)challenge & 0x7fu;

	uint32_t key = (uint32_t)(challenge >> 7) ^ (uint32_t)(challenge >> 32);
	values[0] = solve_p4_walk(target, 0u, 127u, key);

	count = 1;
    return write_answer(out, cap, values, count);
}

int solve_phase_5(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_05 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
	static const uint32_t node_value[12] = {
	    0x779u, 0x971u, 0x31cu, 0x555u,
	    0xa70u, 0x15cu, 0x079u, 0x817u,
	    0xb1bu, 0x444u, 0x24du, 0x680u
	};

	static const uint32_t node_tag[12] = {
	    0x9dcfu, 0x1da7u, 0x0588u, 0xfaf3u,
	    0x68adu, 0x32e1u, 0xbec7u, 0xe547u,
	    0x37e6u, 0x7fb3u, 0xa570u, 0xdce2u
	};

	static const int node_next[12] = {
	    10, 8, 7, 5,
	    0, 2, 3, 9,
	    -1, 1, 6, 4
	};

	static const int heads[8] = {
	    11, 4, 0, 10, 6, 3, 5, 2
	};

	uint32_t head_index = (uint32_t)challenge & 7u;
	uint32_t steps = (uint32_t)((challenge >> 3) & 3u);

	int node = heads[head_index];

	uint32_t result =
	    (uint32_t)(challenge >> 6)
	    ^ (uint32_t)(challenge >> 32)
	    ^ node_tag[node]
	    ^ 0x2b83u;

	result += node_value[node];

	for (uint32_t i = 0; i < steps; i++) {
	    node = node_next[node];
	    result ^= node_tag[node];
	    result += node_value[node];
	}

	values[0] = result;
	values[1] = node_value[node];
	count = 2;
    return write_answer(out, cap, values, count);
}

static uint32_t p6_apply(uint32_t index, uint32_t x)
{
    switch (index) {
        case 0:
            return (x ^ 0xd101u) + 2u * x;

        case 1:
        case 2:
            return ((x >> 2) ^ x) + 0x4f32u;

        case 3:
            return (8u * x + 0xba1bu) ^ (3u * x);

        case 4:
            return (x + 0x91feu) ^ (x >> 3);

        case 5:
        case 6:
            return (x >> 7) | (x << 25);

        case 7: {
            uint32_t y = (x << 7) | (x >> 25);
            return y ^ 0x9d37u;
        }

        default:
            return 0;
    }
}

int solve_phase_6(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_06 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
	uint32_t key = (uint32_t)(challenge >> 6) ^ (uint32_t)(challenge >> 32);

	uint32_t index1 = (uint32_t)challenge & 7u;
	uint32_t index2 = (uint32_t)(challenge >> 3) & 7u;

	uint32_t result = p6_apply(index1, key);

	result ^= 0xc8ddu;

	result = p6_apply(index2, result);

	result += 0xe1e6u;

	values[0] = result;
	count = 1;
    return write_answer(out, cap, values, count);
}

int solve_secret(uint64_t challenge, uint64_t context, char *out, size_t cap) {
    // TODO（选做）: 根据 secret_gate、secret_copy 和 secret_phase 的汇编构造完整的第七行，写入 out 后返回 0。
    // 第七行不一定只有整数，可以直接用 snprintf 写入；还没完成时保持返回 1，测试会显示 not attempted。
	uint32_t a = (uint32_t)context & 3;

	uint32_t s = (uint32_t)(challenge >> 32) ^ (uint32_t)challenge;

	uint32_t t = (uint32_t)(context >> 32) ^ (uint32_t)context;

	uint32_t x = s ^ t;

	uint32_t mix = ((x >> 7) ^ UINT32_C(0xddc3)) + (x ^ UINT32_C(0xb1c1));

	uint32_t u = a * UINT32_C(0xb1c1) ^ (uint32_t)(context >> 16);

	uint32_t b = mix ^ u;

	uint32_t c = b ^ (t + a + UINT32_C(0xddc3) + (s >> 7));

	int n = snprintf(out, cap, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAOPEN%u %u %u", (unsigned)a, (unsigned)b, (unsigned)c);

	return n < 0 || (size_t)n >= cap;
}
