#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MATRIX_SIZE 500

typedef struct {
    int x, y;
} point;

bool visited[MATRIX_SIZE][MATRIX_SIZE] = {0};

void move_tail(point *head, point *tail) {
    // If head and tail are on the same line:
    if (head->x == tail->x) { // vertical line
        if (abs(head->y - tail->y) <= 1)return;
        if (head->y < tail->y) tail->y--;
        else if (head->y > tail->y) tail->y++;
    } else if (head->y == tail->y) { // horizontal line
        if (abs(head->x - tail->x) <= 1)return;
        if (head->x < tail->x) tail->x--;
        else if (head->x > tail->x) tail->x++;
    } 
    // If head and tail are not on the same line:
    else {
        // If head and tail are adjacent (diagonal adjacency):
        if (abs(head->x - tail->x) == 1 && abs(head->y - tail->y) == 1) {
            return;
        } 
        // If head and tail are not adjacent:
        else {
            if (head->x < tail->x) tail->x--;
            else tail->x++;
            
            if (head->y < tail->y) tail->y--;
            else tail->y++;
        }
    }

    // Add the current tail position to the visited ones:
    visited[tail->x][tail->y] = 1;
}

void move_head(point *head, point *tail, char direction, int steps) {
    for (int j = 0; j < steps; j++) {
        switch (direction) {
            case 'R': head->x++; break;
            case 'L': head->x--; break;
            case 'U': head->y--; break;
            case 'D': head->y++; break;
        }
        move_tail(head, tail);
    }
}

int main() {
    FILE *file = fopen("data.txt", "r");
    char direction;
    int steps;
    point head = {MATRIX_SIZE/2, MATRIX_SIZE/2};
    point tail = {MATRIX_SIZE/2, MATRIX_SIZE/2};

    visited[head.x][head.y] = 1;

    while (fscanf(file, " %c %d", &direction, &steps) == 2) {
        move_head(&head, &tail, direction, steps);
    }

    fclose(file);

    int count = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            if (visited[i][j]) {
                count++;
            }
        }
    }

    printf("The tail visited %d positions.\n", count);

    return 0;
}
