// Link: https://adventofcode.com/2022/day/3

#include <stdio.h>
#include <string.h>

int char_to_index(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a';
    } else if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 26;
    } else {
        return -1;
    }
}

int get_common_item_priority(char* str) {
    unsigned long long common;
    int n = strlen(str);
    unsigned long long half1 = 0, half2 = 0;

    for (int i = 0; i < n / 2; i++) {
        half1 |= (unsigned long long)1 << char_to_index(str[i]);
    }
    for (int i = n / 2; i < n; i++) {
        half2 |= (unsigned long long)1 << char_to_index(str[i]);
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
    if (file == NULL) {
        printf("Could not open file data.txt\n");
        return 1;
    }

    char line[1000];
    int total_priority = 0;

    while (fgets(line, sizeof(line), file)) {
        
        line[strcspn(line, "\n")] = 0;  // Remove the newline
        total_priority += get_common_item_priority(line);
    }

    fclose(file);

    printf("Total priority: %d\n", total_priority);

    return 0;
}
