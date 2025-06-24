#include "parser.h"

int main() {
    char* data =
        "a,b,c\n"
        "1,2,3\n"
        "4,5,6\n";
        "7,8,9\n";
    char delimiter = ',';

    int rowBuffer[256];
    int* numWritten;
    int numRead = 0;
    int start = 0;
    do {
        numRead = ParseCsvRowInt(&data[start], delimiter, rowBuffer, numWritten);
        start += numRead;
    } while (numRead > 0);

    return 0;
}
