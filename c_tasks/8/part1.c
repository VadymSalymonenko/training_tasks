// Link: https://adventofcode.com/2022/day/8

#include <stdio.h>
#include <stdbool.h>

#define MAX_SIZE 500

// Function for reading data from a file into a two-dimensional array.
void read_tree_data(char filename[], int grid[][MAX_SIZE], int *rows, int *cols) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    char line[MAX_SIZE];
    *rows = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        for (*cols = 0; line[*cols] != '\n' && line[*cols] != '\0'; (*cols)++) {
            grid[*rows][*cols] = line[*cols] - '0';// Change the character to an integer.
        }
        (*rows)++;
    }

    fclose(file);
}

bool is_tree_visible(int grid[][MAX_SIZE], int rows, int cols, int x, int y) {
    int height = grid[x][y];

    // сheck left
    bool visible_from_left = true;
    for (int i = y - 1; i >= 0; i--) {
        if (grid[x][i] >= height) {
            visible_from_left = false;
            break;
        }
    }
    if (visible_from_left) return true;

    // сheck right
    bool visible_from_right = true;
    for (int i = y + 1; i < cols; i++) {
        if (grid[x][i] >= height) {
            visible_from_right = false;
            break;
        }
    }
    if (visible_from_right) return true;

    // сheck top
    bool visible_from_top = true;
    for (int i = x - 1; i >= 0; i--) {
        if (grid[i][y] >= height) {
            visible_from_top = false;
            break;
        }
    }
    if (visible_from_top) return true;

    // сheck bottom
    bool visible_from_bottom = true;
    for (int i = x + 1; i < rows; i++) {
        if (grid[i][y] >= height) {
            visible_from_bottom = false;
            break;
        }
    }
    if (visible_from_bottom) return true;

    return false;
}

int count_visible_trees(int grid[][MAX_SIZE], int rows, int cols) {
    int count = 0;

    // All trees on the edge are always visible
    count += (rows * 2) + (cols * 2) - 4;

    // Check for all other trees
    for (int i = 1; i < rows - 1; i++) {
        for (int j = 1; j < cols - 1; j++) {
            if (is_tree_visible(grid, rows, cols, i, j)) {
                count++;
            }
        }
    }

    return count;
}

int main() {
    int grid[MAX_SIZE][MAX_SIZE];
    int rows, cols;

    read_tree_data("data.txt", grid, &rows, &cols);

    int result = count_visible_trees(grid, rows, cols);
    printf("The number of visible trees is: %d\n", result);

    return 0;
}
