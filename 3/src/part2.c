// Link: https://adventofcode.com/2022/day/3

#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 100

int char_to_index(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a';
    } else if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 26;
    }
    return 0;
}

unsigned long long get_common_item_priority(char* str1, char* str2, char* str3) {
    unsigned long long badge1 = 0, badge2 = 0, badge3 = 0;
    for (int i = 0; i < strlen(str1); ++i) {
        badge1 |= 1ULL << char_to_index(str1[i]);
    }
    for (int i = 0; i < strlen(str2); ++i) {
        badge2 |= 1ULL << char_to_index(str2[i]);
    }
    for (int i = 0; i < strlen(str3); ++i) {
        badge3 |= 1ULL << char_to_index(str3[i]);
    }

    unsigned long long common_badge = badge1 & badge2 & badge3;

    unsigned int priority = 0;
    for (int i = 0; i < 52; ++i) {
        if (common_badge & (1ULL << i)) {
            priority += i + 1;
        }
    }
    return priority;
}

int main() {
    FILE* file = fopen("../data.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    char str1[MAX_LINE_LENGTH], str2[MAX_LINE_LENGTH], str3[MAX_LINE_LENGTH];
    unsigned int total_priority = 0;
    while (fgets(str1, MAX_LINE_LENGTH, file) && fgets(str2, MAX_LINE_LENGTH, file) && fgets(str3, MAX_LINE_LENGTH, file)) {

        str1[strcspn(str1, "\n")] = 0;
        str2[strcspn(str2, "\n")] = 0;
        str3[strcspn(str3, "\n")] = 0;
        total_priority += get_common_item_priority(str1, str2, str3);
    }

    printf("Total priority: %u\n", total_priority);
    fclose(file);
    return 0;
}
