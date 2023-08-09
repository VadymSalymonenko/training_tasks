#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MATRIX_SIZE 500
#define TAIL_COUNT 9

typedef struct {
    int x, y;
} point;

bool visited[MATRIX_SIZE][MATRIX_SIZE] = {0};

void print_field(point head, point tails[TAIL_COUNT]) {
    char field[MATRIX_SIZE][MATRIX_SIZE];

    // Fill the field with default characters
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            field[i][j] = '-';
        }
    }

    // Place the tails
    for (int i = TAIL_COUNT-1; i >= 0; i--) {
        if (tails[i].x >= 0 && tails[i].x < MATRIX_SIZE && tails[i].y >= 0 && tails[i].y < MATRIX_SIZE) {
            field[tails[i].y][tails[i].x] = '1' + i; // The first tail will be assigned 1, the 9th tail - 9
        }
    }

    // Place the head
    if (head.x >= 0 && head.x < MATRIX_SIZE && head.y >= 0 && head.y < MATRIX_SIZE) {
        field[head.y][head.x] = 'H';
    }

    // Print the field
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%c", field[i][j]);
        }
        printf("\n");
    }
}

void move_tail(point *head, point *tail, int tail_num) {
    // If the head and tail are on the same line:
    if (head->x == tail->x) { // vertical line
        if (abs(head->y - tail->y) <= 1) return;
        if (head->y < tail->y) tail->y--;
        else if (head->y > tail->y) tail->y++;
    } else if (head->y == tail->y) { // horizontal line
        if (abs(head->x - tail->x) <= 1) return;
        if (head->x < tail->x) tail->x--;
        else if (head->x > tail->x) tail->x++;
    } 
    // If the head and tail are not on the same line:
    else {
        // If the head and tail are adjacent (diagonal adjacency):
        if (abs(head->x - tail->x) == 1 && abs(head->y - tail->y) == 1) {
            return;
        } 
        // If the head and tail are not adjacent:
        else {
            if (head->x < tail->x) tail->x--;
            else tail->x++;

            if (head->y < tail->y) tail->y--;
            else tail->y++;
        }
    }

    // Add the current tail position to visited:
    if(tail_num == TAIL_COUNT-1){
        visited[tail->x][tail->y] = 1;
    }
}

void move_head(point *head, point *tails, char direction, int steps) {
    for (int i = 0; i < steps; i++) {
        switch (direction) {
            case 'R': head->x++; break;
            case 'L': head->x--; break;
            case 'U': head->y--; break;
            case 'D': head->y++; break;
        }
        move_tail(head, &tails[0], 0);
        for(int j = 1; j < TAIL_COUNT; j++) {
            move_tail(&tails[j-1], &tails[j], j);   
        }
        //print_field(*head, tails);
        //printf("***********************************\n");
    }
}

int main() {
    FILE *file = fopen("data.txt", "r");
    char direction;
    int steps;
    point head = {MATRIX_SIZE/2, MATRIX_SIZE/2};
    point tails[TAIL_COUNT];

    for (int i = 0; i < TAIL_COUNT; i++) {
        tails[i].x = head.x;
        tails[i].y = head.y;
    }

    visited[head.x][head.y] = 1;

    while (fscanf(file, " %c %d", &direction, &steps) == 2) {
        move_head(&head, tails, direction, steps);
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
