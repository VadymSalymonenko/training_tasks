// Link: https://adventofcode.com/2022/day/4

#include <stdio.h>

int main() {
    FILE *file = fopen("data.txt", "r");
    if (file == NULL) {
        printf("Failed to open file\n");
        return -1;
    }

    int start1, end1, start2, end2;
    int count = 0;
    while (fscanf(file, "%d-%d,%d-%d\n", &start1, &end1, &start2, &end2) != EOF) {
        if ((start1 >= start2 && end1 <= end2) || (start2 >= start1 && end2 <= end1)) {
            count++;
        }
    }

    printf("%d\n", count);
    fclose(file);
    return 0;
}
