/**
 * Calcuate the averge numbers from the file.
 */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

double cal_average(const char *filename)
{
    uint32_t data_count = 0;
    double sum = 0.0;
    char buffer[16];
    FILE *fp = fopen(filename, "r");
    if (!fp)
        exit(1);

    while (fgets(buffer, 16, fp)) {
        ++data_count;
        sum += atof(buffer);
    }

    fclose(fp);
    return sum / data_count;
}

int main(int argc, char *argv[])
{
    assert(argc == 2 && "Usage: ./util-average <input_file>");
    printf("Average: %.6lf\n", cal_average(argv[1]));
    return 0;
}
