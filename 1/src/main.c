// Link: https://adventofcode.com/2022/day/1

#include <stdio.h>
#include <stdlib.h>

#define MAX_INT_NUMBER_LENGTH 12
#define TOP_LIST_SIZE 3

// Структура для зберігання ID ельфа та загальної кількості калорій.
typedef struct {
    unsigned int id;
    unsigned int totalCalories;
} Elf;

void print_top_elves(const Elf topElves[]) {
    for (int i = TOP_LIST_SIZE - 1; i >= 0; i--) {
        printf("Place %d: Elf ID -- %d, Calories -- %d\n", TOP_LIST_SIZE - i, topElves[i].id, topElves[i].totalCalories);
    }
}

void print_sum_of_elements(const Elf elfs[]) {
    int sum = 0;
    for (int i = 0; i < TOP_LIST_SIZE; i++) {
        sum += elfs[i].totalCalories;
    }
    printf("total sum: %d\n", sum);
}

unsigned int find_insertion_index(Elf elfs[],unsigned int key) {
    int start = 0;
    int end = TOP_LIST_SIZE - 1;

    while (start <= end) {
        int mid = (start + end) / 2;

        if (elfs[mid].totalCalories == key) {
            return mid + 1;
        }

        if (elfs[mid].totalCalories < key) {
            start = mid + 1;
        } else {
            end = mid - 1;
        }
    }

    return start;
}

void update_elf_calories(Elf* elfs, int new_calories, int curr_elf_number) {
    int insertion_index = find_insertion_index(elfs, new_calories);

    for (int i = 1; i < insertion_index; i++) {
        elfs[i - 1] = elfs[i];
    }
    elfs[insertion_index - 1].totalCalories = new_calories;
    elfs[insertion_index - 1].id = curr_elf_number;
}
int main() {
    FILE *file;
    file = fopen("../data.txt", "r");

    if (file == NULL) {
        printf("Failed to open file\n");
        return 1;
    }

    Elf elfs[TOP_LIST_SIZE];
    for(int i = 0; i < TOP_LIST_SIZE; i++) {
        elfs[i].id = 0;
        elfs[i].totalCalories = 0;
    }

    unsigned int curr_total_food = 0;
    unsigned int curr_elf_number = 0;
    
    char line[MAX_INT_NUMBER_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        
        if (line[0] == '\n' || line[0] == '\r') {
            
            update_elf_calories(elfs, curr_total_food, curr_elf_number);
            //printf("test info -- %d\n", curr_total_food);

            curr_elf_number++;
            curr_total_food = 0;
        } else {
            curr_total_food += atoi(line);
        }
    }
    update_elf_calories(elfs, curr_total_food, curr_elf_number);

    print_top_elves(elfs);
    print_sum_of_elements(elfs);

    fclose(file); 
    
    return 0;
}
