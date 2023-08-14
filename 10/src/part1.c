#include <stdio.h>
#include <string.h>

#define MAX_LINE 100

typedef struct {
    int x_value;
    int total_signal_strength;
    int current_cycle;
} CPU;

void compute_cycle(const char *line, CPU *cpu);
void check_signal_strength(CPU *cpu);

void compute_cycle(const char *line, CPU *cpu) {
    if (strncmp(line, "noop", 4) == 0) {
        cpu->current_cycle++;
        check_signal_strength(cpu);
    } else if (strncmp(line, "addx", 4) == 0) {
        int value;
        sscanf(line, "addx %d", &value);
        cpu->current_cycle++;
        check_signal_strength(cpu);

        cpu->current_cycle++;
        
        check_signal_strength(cpu);
        cpu->x_value += value;
    }
}

void check_signal_strength(CPU *cpu) {
    if (cpu->current_cycle == 20 || (cpu->current_cycle - 20) % 40 == 0) {
        int signal_strength = cpu->current_cycle * cpu->x_value;
        cpu->total_signal_strength += signal_strength;
        printf("Added signal strength: %d at cycle %d\n", signal_strength, cpu->current_cycle);
    }
}

int main() {
    FILE *file = fopen("../data.txt", "r");
    if (!file) {
        printf("\033[1;31m");
        perror("Failed to open data.txt");
        printf("\033[0m"); 
        return 1;
    }

    CPU cpu = {1, 0, 0};

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), file)) {
        compute_cycle(line, &cpu);
    }

    printf("Total signal strength: %d\n", cpu.total_signal_strength);

    fclose(file);
    return 0;
}