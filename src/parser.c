#include <immintrin.h>
#include <stdio.h>

/*
 * Searches for occurrences of a target character
 * within the leading 32 bytes. Returns a bitmask
 * where the nth bit is set if the nth byte was a match.
 */
static inline int GetMask32(char* data, char target) {
    __m256i chunk = _mm256_loadu_si256((__m256i*)data);
    __m256i targetChunk = _mm256_set1_epi8(target);

    __m256i byteComparison = _mm256_cmpeq_epi8(chunk, targetChunk);
    int packedByteComparison = _mm256_movemask_epi8(byteComparison);

    return packedByteComparison;
}

int ParseInt(char* str, size_t len) {
    int val = 0;
    int sign = 1;
    size_t i = 0;

    if (i < len && str[i] == '-') {
        sign = -1;
        i++;
    }

    for (; i < len; i++) {
        if (str[i] < '0' || str[i] > '9') {
            break;
        }
        val = val * 10 + (str[i] - '0');
    }

    return val * sign;
}


// TODO: actually parse
int ParseCsvRowInt(char* data, char delimiter, int* output, int* outputCount) {
    int delimMask = GetMask32(data, delimiter);

    while (delimMask) {
        int delimIndex = __builtin_ctz(delimMask);
        delimMask &= delimMask - 1;

        printf("Found delimiter at %d\n", delimIndex);
    }

    return -1;
}
