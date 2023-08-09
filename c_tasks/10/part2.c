// Link: https://adventofcode.com/2022/day/10

#include <stdio.h>
#include <string.h>

#define MAX_LINE 100
#define SCREEN_WIDTH 40
#define SCREEN_HEIGHT 6

int x_value = 1; // Global variable for the value of the X register
char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1]; // +1 for null terminator

void compute_cycle(const char *line, int *current_cycle);
void update_screen(int current_cycle);
int is_sprite_pixel(int position);


void compute_cycle(const char *line, int *current_cycle) {
    if (strncmp(line, "noop", 4) == 0) {
        (*current_cycle)++;
        update_screen(*current_cycle);
    } else if (strncmp(line, "addx", 4) == 0) {
        int value;
        sscanf(line, "addx %d", &value);

        (*current_cycle)++;
        update_screen(*current_cycle);

        (*current_cycle)++;
        update_screen(*current_cycle);
        x_value += value;
    }
}
void update_screen(int current_cycle) {
    int row = (current_cycle - 1) / SCREEN_WIDTH;
    int col = (current_cycle - 1) % SCREEN_WIDTH;

    if (is_sprite_pixel(col)) {
        screen[row][col] = '#';
    } else {
        screen[row][col] = '.';
    }

    // Якщо ми досягли кінця рядка, виведіть його на друк
    if (current_cycle % SCREEN_WIDTH == 0) {
        printf("%s\n", screen[row]);
    }
}

int is_sprite_pixel(int position) {
    int sprite_start = x_value - 1;
    int sprite_end = x_value + 1;

    // Якщо спрайт виходить за межі екрану зліва або справа
    if (sprite_start < 0) sprite_start += SCREEN_WIDTH;
    if (sprite_end >= SCREEN_WIDTH) sprite_end -= SCREEN_WIDTH;

    return position == x_value || position == sprite_start || position == sprite_end;
}

int main() {
    FILE *file = fopen("data.txt", "r");
    if (file == NULL) {
        printf("Error: Unable to open data.txt\n");
        return 1;
    }

    // Initialize the screen with dots
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            screen[i][j] = '.';
        }
        screen[i][SCREEN_WIDTH] = '\0';
    }

    char line[MAX_LINE];
    int current_cycle = 0;  // Current cycle number

    while (fgets(line, sizeof(line), file)) {
        compute_cycle(line, &current_cycle);
    }

   // // Print the final screen state
   // for (int i = 0; i < SCREEN_HEIGHT; i++) {
   //     printf("%s\n", screen[i]);
   // }

    fclose(file);
    return 0;
}
