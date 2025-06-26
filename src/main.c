#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "parser.h"

char* GenerateTestData(int rows, int cols, char delimiter);

int main() {
    char delimiter = ',';

    char* csvData = GenerateTestData(20, 10, delimiter);
    printf("Data to parse:\n%s\n", csvData);

    ParseCsv(csvData, delimiter);

    return 0;
}

char* GenerateTestData(int rows, int cols, char delimiter)
{
    assert(cols <= 10 && "The current test data uses single digit values.");

    // make sure to the data can be consumed in 32 byte chunks and ends with null characters
    size_t estimatedSize = sizeof(char) * rows * cols * 3;
    size_t paddedSize = estimatedSize + CHUNK_SIZE - (estimatedSize % CHUNK_SIZE);
    char* buf = calloc(1, paddedSize);
    int bufIndex = 0;

    for (int j = 0; j < cols; j++) {
        buf[bufIndex++] = 'x';
        buf[bufIndex++] = j + '0';
        buf[bufIndex++] = delimiter;
    }
    buf[bufIndex - 1] = '\n';

    for (int i = 1; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            buf[bufIndex++] = j + '0';
            buf[bufIndex++] = delimiter;
        }
        buf[bufIndex - 1] = '\n';
    }

    return buf;
}
