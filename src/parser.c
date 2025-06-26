#include <immintrin.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "parser.h"

/*
 * __builtin_ctz is a GCC builtin to count the trailing zeros.
 * That's the same as the index of the lowest set bit.
 */
#define LOWEST_SET_BIT(n) __builtin_ctz(n)

/*
 * Unset the lowest set bit.
 *
 * Example:
 * 1110 - 0001 = 1101
 * 1110 & 1101 = 1100
 */
#define CONSUME_LOWEST_BIT(n) n &= n - 1

/*
 * Searches for occurrences of a target character
 * within the leading 32 bytes. Returns a bitmask
 * where the nth bit is set if the nth byte was a match.
 */
static inline int GetMask32(char* data, char target) {
    // load 32 bytes of data and 32 bytes of repeated target characters
    __m256i chunk = _mm256_loadu_si256((__m256i*)data);
    __m256i targetChunk = _mm256_set1_epi8(target);

    // compare the 32 bytes and pack the 0/1 results into 32 bits
    __m256i byteComparison = _mm256_cmpeq_epi8(chunk, targetChunk);
    int packedByteComparison = _mm256_movemask_epi8(byteComparison);

    return packedByteComparison;
}

static int ParseInt(char* str, size_t len) {
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


/*
 * Consumes up 32 bytes of CSV data and parses the result as integers.
 * Returns the number of read bytes.
 *
 * Assumes zero padded input so that 32 byte chunks can be read
 */
static int ParseCsvChunkInt(char* data, char delimiter, int* output, int* outputCount) {
    int delimMask = GetMask32(data, delimiter);
    int newlineMask = GetMask32(data, '\n');
    int nullMask = GetMask32(data, '\0');

    int mask = delimMask | newlineMask | nullMask;

    int valueStart = 0;
    int outputIndex = 0;
    while (mask) {
        int nonValueIndex = LOWEST_SET_BIT(mask);
        int valueLength = nonValueIndex - valueStart;

        if (valueLength == 0) {
            if (data[valueStart] == '\0' || data[valueStart+1] == '\0') {
                // reached end of data
                break;
            } else if (data[valueStart] == delimiter || data[valueStart+1] == delimiter) {
                assert(false && "Invalid CSV. Encountered consecutive delimiters.");
            } else {
                // fast forward if there are repeated newlines
                CONSUME_LOWEST_BIT(mask);
                valueStart++;
                continue;
            }
        }

        int value = ParseInt(&data[valueStart], valueLength);
        output[outputIndex++] = value;

        CONSUME_LOWEST_BIT(mask);
        valueStart += valueLength + 1;
    }

    *outputCount = outputIndex;
    return valueStart;
}

// just print the parsed data for now
void ParseCsv(char* data, char delimiter) {
    int outputBuffer[32];
    int numWritten = 0;
    int numRead = 0;
    int offset = 0;

    // skip header
    while (data[offset++] != '\n');

    // consume and print chunks until the end is reached
    do {
        printf("Parsing chunk (offset = %d)\n", offset);

        numWritten = 0;
        numRead = ParseCsvChunkInt(&data[offset], delimiter, outputBuffer, &numWritten);

        printf("Parsed values:\n");
        for (int i = 0; i < numWritten; i++) {
            printf("%d,", outputBuffer[i]);
        }
        printf("\n\n");
        offset += numRead;
    } while (numRead > 0);
}

