// Link: https://adventofcode.com/2022/day/3

#include <stdio.h>
#include <string.h>

int get_item_priority(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a';
    } else if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 26;
    } 
    return -1;
}

int find_common_item_priority(char* rucksack) {
    unsigned long long common;
    int n = strlen(rucksack);
    unsigned long long half1 = 0, half2 = 0;

    for (int i = 0; i < n; i++) {
        if(i < n / 2){
            half1 |= (unsigned long long)1 << get_item_priority(rucksack[i]);
        }else{
            half2 |= (unsigned long long)1 << get_item_priority(rucksack[i]);
        }
    }
    common = half1 & half2;
    for (int i = 0; i < 52; i++) {
        if (common & ((unsigned long long)1 << i)) {
            return i + 1;
        }
    }

    return -1;
}

int main() {
    FILE* file = fopen("../data.txt", "r");
    if (!file) {
        printf("\033[1;31m");
        perror("Failed to open data.txt");
        printf("\033[0m"); 
        return 1;
    }

    char rucksack[1000];
    int cumulativePriority  = 0;

    while (fgets(rucksack, sizeof(rucksack), file)) {
        
        rucksack[strcspn(rucksack, "\n")] = 0;  // Remove the newline
        cumulativePriority  += find_common_item_priority(rucksack);
    }

    fclose(file);

    printf("Total priority: %d\n", cumulativePriority );

    return 0;
}
