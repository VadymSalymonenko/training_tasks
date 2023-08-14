// Link: https://adventofcode.com/2022/day/4

#include <stdio.h>

int contains(int start1, int end1, int start2, int end2) {
    return (start1 >= start2 && end1 <= end2) || (start2 >= start1 && end2 <= end1);
}

int main() {
    FILE *file = fopen("../data.txt", "r");
    if (!file) {
        printf("\033[1;31m");
        perror("Failed to open data.txt");
        printf("\033[0m"); 
        return 1;
    }

    int start1, end1, start2, end2;
    int count = 0;
    while (fscanf(file, "%d-%d,%d-%d\n", &start1, &end1, &start2, &end2) == 4) {
        if (contains(start1, end1, start2, end2)) {
            count++;
        }
    }

    printf("%d\n", count);
    fclose(file);
    return 0;
}
