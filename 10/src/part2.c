#include <stdio.h>
#include <string.h>

#define MAX_LINE 100
#define SCREEN_WIDTH 40
#define SCREEN_HEIGHT 6

typedef struct {
    int x_value;
    int current_cycle;
    char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1]; // +1 for null terminator
} CPU;

void compute_cycle(const char *line, CPU *cpu);
void update_screen(CPU *cpu);
int is_sprite_pixel(int position, int x_value);

void compute_cycle(const char *line, CPU *cpu) {
    if (strncmp(line, "noop", 4) == 0) {
        cpu->current_cycle++;
        update_screen(cpu);
    } else if (strncmp(line, "addx", 4) == 0) {
        int value;
        sscanf(line, "addx %d", &value);

        cpu->current_cycle++;
        update_screen(cpu);

        cpu->current_cycle++;
        update_screen(cpu);
        cpu->x_value += value;
    }
}

void update_screen(CPU *cpu) {
    int row = (cpu->current_cycle - 1) / SCREEN_WIDTH;
    int col = (cpu->current_cycle - 1) % SCREEN_WIDTH;

    if (is_sprite_pixel(col, cpu->x_value)) {
        cpu->screen[row][col] = '#';
    } else {
        cpu->screen[row][col] = '.';
    }

    // If we've reached the end of the row, print it
    if (cpu->current_cycle % SCREEN_WIDTH == 0) {
        printf("%s\n", cpu->screen[row]);
    }
}

int is_sprite_pixel(int position, int x_value) {
    int sprite_start = x_value - 1;
    int sprite_end = x_value + 1;

    // If the sprite goes off the screen on the left or right
    if (sprite_start < 0) sprite_start += SCREEN_WIDTH;
    if (sprite_end >= SCREEN_WIDTH) sprite_end -= SCREEN_WIDTH;

    return position == x_value || position == sprite_start || position == sprite_end;
}

int main() {
    FILE *file = fopen("../data.txt", "r");
    if (!file) {
        printf("\033[1;31m");
        perror("Failed to open data.txt");
        printf("\033[0m"); 
        return 1;
    }

    CPU cpu = { .x_value = 1, .current_cycle = 0 };

    // Initialize the screen with dots
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            cpu.screen[i][j] = '.';
        }
        cpu.screen[i][SCREEN_WIDTH] = '\0';
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), file)) {
        compute_cycle(line, &cpu);
    }

    fclose(file);
    return 0;
}