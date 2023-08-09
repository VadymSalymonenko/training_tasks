// Link: https://adventofcode.com/2022/day/6

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 4096

// Check if all characters in the string are distinct.
int all_unique(char *str) {
    for(int i = 0; i < strlen(str); i++) {
        for(int j = i + 1; j < strlen(str); j++) {
            if(str[i] == str[j]) {
                return 0;
            }
        }
    }
    return 1;
}

int find_marker_position(char *data) {
    char buffer[5] = {0};
    for(int i = 0; i < strlen(data); i++) {
        buffer[i % 4] = data[i];
        if(i >= 3 && all_unique(buffer)) {
            return i;
        }
    }
    return -1;  // Return -1 if the marker is not found.
}

int main() {
    FILE *file = fopen("data.txt", "r");
    if(file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    char data[MAX_LINE_LENGTH];
    if(fgets(data, sizeof(data), file) == NULL) {
        printf("Error reading file.\n");
        return 1;
    }

    fclose(file);

    // Find and display the position of the marker.
    int pos = find_marker_position(data);
    if(pos != -1) {
        printf("The first start-of-packet marker is detected after character %d\n", pos + 1);
    } else {
        printf("No start-of-packet marker found.\n");
    }
    
    return 0;
}
