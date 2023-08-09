#include <stdio.h>
#include <stdlib.h>

#define MAX_INT_NUMBER_LENGTH 12
#define TOP_LIST_SIZE 3

void print_top_list(int* elf_calories, int* elf_calories_numbers, int size){
    for(int i = size; i > 0; i--){
        printf("place %d: calories -- %d, id: %d\n", size-i+1, elf_calories[i-1], elf_calories_numbers[i-1]);
    }
}
void print_sum_of_elements(int arr[], int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    printf("total sum: %d\n", sum);
}

int find_insertion_index(int arr[], int size, int key) {
    int start = 0;
    int end = size - 1;

    while (start <= end) {
        int mid = (start + end) / 2;

        if (arr[mid] == key) {
            return mid + 1;
        }

        if (arr[mid] < key) {
            start = mid + 1;
        } else {
            end = mid - 1;
        }
    }

    return start;
}

void update_elf_calories(int* elf_calories, int* elf_calories_numbers, int size, int new_calories, int curr_elf_number) {


    if (new_calories <= elf_calories[0]) {
        return;
    }

    int insertion_index = find_insertion_index(elf_calories, size, new_calories);

    for (int i = 1; i < insertion_index; i++) {
        elf_calories[i - 1] = elf_calories[i];
        elf_calories_numbers[i - 1] = elf_calories_numbers[i];
    }

    elf_calories[insertion_index - 1] = new_calories;
    elf_calories_numbers[insertion_index - 1] = curr_elf_number;
}


int main() {
    FILE *file;
    file = fopen("data.txt", "r");

    
    if (file == NULL) {
        printf("Failed to open file\n");
        return 1;
    }

    int elf_calories[TOP_LIST_SIZE] = {0};
    int elf_calories_numbers[TOP_LIST_SIZE] = {0};

    int curr_total_food = 0;
    int curr_elf_number = 0;
    
    char line[MAX_INT_NUMBER_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        
        if (line[0] == '\n' || line[0] == '\r') {
            
            update_elf_calories(elf_calories, elf_calories_numbers, TOP_LIST_SIZE, curr_total_food, curr_elf_number);
            printf("test info -- %d\n", curr_total_food);

            curr_elf_number++;
            curr_total_food = 0;
        } else {
            int input_num = atoi(line);
            curr_total_food += input_num;
        }
    }
    update_elf_calories(elf_calories, elf_calories_numbers, TOP_LIST_SIZE, curr_total_food, curr_elf_number);
    printf("test info -- %d\n", curr_total_food);

    print_top_list(elf_calories, elf_calories_numbers, TOP_LIST_SIZE);
    print_sum_of_elements(elf_calories, TOP_LIST_SIZE);

    fclose(file); 
    
    return 0;
}
