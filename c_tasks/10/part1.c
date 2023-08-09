#include <stdio.h>
#include <string.h>

#define MAX_LINE 100

int x_value = 1; // Global variable for the value of the X register
int total_signal_strength = 0; // Global variable for total signal strength

void compute_cycle(const char *line, int *current_cycle);
void check_signal_strength(int current_cycle);

int main() {
    FILE *file = fopen("data.txt", "r");
    if (file == NULL) {
        printf("Error: Unable to open data.txt\n");
        return 1;
    }

    char line[MAX_LINE];
    int current_cycle = 0;  // Current cycle number

    while (fgets(line, sizeof(line), file)) {
        compute_cycle(line, &current_cycle);
    }

    printf("Total signal strength: %d\n", total_signal_strength);

    fclose(file);
    return 0;
}

void compute_cycle(const char *line, int *current_cycle) {
    if (strncmp(line, "noop", 4) == 0) {
        (*current_cycle)++;
        check_signal_strength(*current_cycle);
    } else if (strncmp(line, "addx", 4) == 0) {
        int value;
        sscanf(line, "addx %d", &value);
        if(*current_cycle >= 180){
            printf("test \n");   
        }
        (*current_cycle)++;
        check_signal_strength(*current_cycle);

        (*current_cycle)++;
        
        check_signal_strength(*current_cycle);
        x_value += value;
    }
}

void check_signal_strength(int current_cycle) {
    if (current_cycle == 20 || (current_cycle - 20) % 40 == 0) {
        int signal_strength = current_cycle * x_value;
        total_signal_strength += signal_strength;
        printf("Added signal strength: %d at cycle %d\n", signal_strength, current_cycle);
    }
}
