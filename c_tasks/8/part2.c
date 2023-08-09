// Link: https://adventofcode.com/2022/day/8

#include <stdio.h>
#include <stdbool.h>

#define MAX_SIZE 500

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
            grid[*rows][*cols] = line[*cols] - '0'; // Change the character to an integer.
        }
        (*rows)++;
    }

    fclose(file);
}

int tree_view_count(int grid[][MAX_SIZE], int rows, int cols, int x, int y, int dx, int dy) {
    int count = 0;
    int height = grid[x][y];
    x += dx;
    y += dy;

    while (x >= 0 && y >= 0 && x < rows && y < cols) {
        if (grid[x][y] >= height) {
            count++;
            break;
        }
        count++;
        x += dx;
        y += dy;
    }

    return count;
}

int tree_scenic_score(int grid[][MAX_SIZE], int rows, int cols, int x, int y) {
    int up = tree_view_count(grid, rows, cols, x, y, -1, 0);
    int down = tree_view_count(grid, rows, cols, x, y, 1, 0);
    int left = tree_view_count(grid, rows, cols, x, y, 0, -1);
    int right = tree_view_count(grid, rows, cols, x, y, 0, 1);

    return up * down * left * right;
}

int main() {
    int grid[MAX_SIZE][MAX_SIZE];
    int rows, cols;

    read_tree_data("data.txt", grid, &rows, &cols);

    int maxScore = 0;
    for (int i = 1; i < rows - 1; i++) {
        for (int j = 1; j < cols - 1; j++) {
            int score = tree_scenic_score(grid, rows, cols, i, j);
            if (score > maxScore) {
                maxScore = score;
            }
        }
    }

    printf("The highest scenic score possible for any tree is: %d\n", maxScore);

    return 0;
}
