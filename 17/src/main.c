#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAX_ROCKS 2022
#define MAX_SIZE 16
#define GAME_ROWS 15000
#define GAME_COLS 7

typedef struct {
    char shape[MAX_SIZE];
    int rows;
    int cols;
} Rock;

typedef struct {
    Rock rocks[MAX_ROCKS];
    int count;
} RockCollection;

typedef struct {
    int x, y;
} Point;

typedef struct {
    char field[GAME_ROWS][GAME_COLS];
    int max_height;
    int fallen_rocks;
    int game_steps;
    Point falling_rock_position; 
} GameBoard;

void init_game_board(GameBoard *board) {
    memset(board->field, '.', sizeof(board->field));
    board->fallen_rocks = 0;
    board->game_steps = 0;
    board->max_height = 0;
}

void print_game_board(const GameBoard *board) {
    int start_row = (GAME_ROWS - 7) - board->max_height;
    for (int row = start_row; row < GAME_ROWS; row++) {
        printf("|");
        for (int col = 0; col < GAME_COLS; col++) {
            printf("%c", board->field[row][col]);
        }
        printf("|\n");
    }
    printf("+---------+\n"); 
}


void add_rock(RockCollection *collection, char *shape, int rows, int cols) {
    if (collection->count >= MAX_ROCKS) {
        printf("Reached the maximum number of rocks\n");
        return;
    }

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            int index = row * cols + col;
            collection->rocks[collection->count].shape[index] = shape[index];
        }
    }

    collection->rocks[collection->count].rows = rows;
    collection->rocks[collection->count].cols = cols;
    collection->count++;
}

void print_rocks(const RockCollection *collection) {
    for (int i = 0; i < collection->count; i++) {
        printf("Rock %d:\n", i + 1);
        for (int row = 0; row < collection->rocks[i].rows; row++) {
            for (int col = 0; col < collection->rocks[i].cols; col++) {
                int index = row * collection->rocks[i].cols + col;
                printf("%c", collection->rocks[i].shape[index]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

void finalize_rock(GameBoard *board, const Rock *rock, int row, int col) {
    // Iterate through each symbol of the shape
    int max_curr_height = 0;
    for (int r = 0; r < rock->rows; r++) {
        for (int c = 0; c < rock->cols; c++) {
            int index = r * rock->cols + c;
            if (rock->shape[index] != '.') { // If it's not an empty symbol
                // Replace shape symbol with fixed rock symbol
                board->field[row + r][col + c] = '#';
                if(max_curr_height < GAME_ROWS - (row + r)){
                    max_curr_height = GAME_ROWS - (row + r);
                }
            }
        }
    }
    if(board->max_height < max_curr_height) board->max_height = max_curr_height;
}

void check_finalize_rock(int current_rock_index, GameBoard *board, const RockCollection *rocks, int row, int col){
    if (current_rock_index != -1) {
        finalize_rock(board, &rocks->rocks[current_rock_index], row, col);
    }
}

int can_fall(const GameBoard *board, const Rock *rock, int row, int col) {
    int bottom_row = row + rock->rows - 1; // Bottom row of the shape

    // Go through the four lines above the bottom one
    for (int r = bottom_row; r > bottom_row - 4 && r >= 0; r--) {
        for (int c = col; c < col + rock->cols; c++) {
            if (board->field[r][c] == '@') { // If a part of the figure is found
                // Check if there's an obstacle below or if we've reached the bottom of the field
                if (r == GAME_ROWS - 1 || board->field[r + 1][c] == '#') {
                    return 0; 
                }
            }
        }
    }

    return 1;
}
int can_move_right(GameBoard *board, const Rock *rock, int row, int col) {
    if (col + rock->cols >= GAME_COLS) return 0;

   // Check if the shape will collide with other blocks on the right
    for (int r = 0; r < rock->rows; r++) {
        for (int c = 0; c < rock->cols; c++) {
            if (board->field[row + r][col + c] == '@' && board->field[row + r][col + c + 1] == '#') {
                return 0;
            }
        }
    }

    return 1;
}

int can_move_left(GameBoard *board, const Rock *rock, int row, int col) {
    // We check whether the shape will collide with the left border of the field
    if (col <= 0) return 0;
    for (int r = 0; r < rock->rows; r++) {
        for (int c = 0; c < rock->cols; c++) {
            if (board->field[row + r][col + c] == '@' && board->field[row + r][col + c - 1] == '#') {
                return 0;
            }
        }
    }
    return 1;
}




// Function to remove a figure from the board
void remove_rock(GameBoard *board, const Rock *rock, int row, int col) {
    for (int r = 0; r < rock->rows; r++) {
        for (int c = 0; c < rock->cols; c++) {
            int index = r * rock->cols + c;
            if (rock->shape[index] != '.') { // If it is not an empty character
                board->field[row + r][col + c] = '.'; // Clear the shape symbol
            }
        }
    }
}

// Function to add a figure to the board
void place_rock(GameBoard *board, const Rock *rock, int row, int col) {
    for (int r = 0; r < rock->rows; r++) {
        for (int c = 0; c < rock->cols; c++) {
            int index = r * rock->cols + c;
            if (rock->shape[index] != '.') { 
                board->field[row + r][col + c] = '@'; 
            }
        }
    }
}

void move_left(GameBoard *board, const Rock *rock, int *row, int *col) {
    if (!can_move_left(board, rock, *row, *col)) return;
    remove_rock(board, rock, *row, *col);
    (*col)--;
    place_rock(board, rock, *row, *col);
    board->falling_rock_position.x--;
}

void move_right(GameBoard *board, const Rock *rock, int *row, int *col) {
    if (!can_move_right(board, rock, *row, *col)) return;
    remove_rock(board, rock, *row, *col);
    (*col)++;
    place_rock(board, rock, *row, *col);
    board->falling_rock_position.x++;
}

void move_down(GameBoard *board, const Rock *rock, int *row, int *col) {
    if (!can_fall(board, rock, *row, *col)) return;
    remove_rock(board, rock, *row, *col);
    (*row)++;
    place_rock(board, rock, *row, *col);
    board->falling_rock_position.y++;
}



void spawn_rock(GameBoard *board, const Rock *rock, int *row, int *col) {
    // Find the line for spawning, taking into account the distance from the highest point
    *row = GAME_ROWS - board->max_height - 3 - rock->rows;
    if (*row < 0) *row = 0; // We make sure that the string does not go outside the field

    *col = 2; // Spawn shapes indented 2 blocks to the left

    // Fill the field with shape symbols
    for (int r = 0; r < rock->rows; r++) {
        for (int c = 0; c < rock->cols; c++) {
            int index = r * rock->cols + c;
            if (rock->shape[index] != '.') {
                board->field[*row + r][*col + c] = '@'; // We write the shape symbol
            }
        }
    }

    // Update the maximum height of the field, if necessary
    if (*row < board->max_height) {
        board->max_height = *row;
    }
    board->falling_rock_position.x = *col;
    board->falling_rock_position.y = *row + rock->rows - 1;
}

int simulate_game(GameBoard *board, const RockCollection *rocks, const char *actions) {
    int current_rock_index = -1; // The index of the current figure
    char permit_spawn = 1; // The index of the current figure
    int row, col; // The current position of the figure
    int steps = 0; // Step counter

    while (1) {

        if (permit_spawn == 1 ) {
            // If the previous figure has stopped, complete it (change @ to #)
            permit_spawn = 0;
            current_rock_index = (current_rock_index + 1)%rocks->count;
            if (board->fallen_rocks >= MAX_ROCKS) break; // All shapes are already used
            spawn_rock(board, &rocks->rocks[current_rock_index], &row, &col);
            board->fallen_rocks++; // We increase the counter of figures that have stopped
        }

        // If the actions are over, we end the simulation
        if (actions[steps] == '\0') steps = 0;


        if (actions[steps] == '<') {
            move_left(board, &rocks->rocks[current_rock_index], &row, &col);
        } else if (actions[steps] == '>') {
            move_right(board, &rocks->rocks[current_rock_index], &row, &col);
        }
        if (can_fall(board, &rocks->rocks[current_rock_index], row, col)) {
            move_down(board, &rocks->rocks[current_rock_index], &row, &col);
        } else {
            check_finalize_rock(current_rock_index, board, rocks, row, col);
            permit_spawn = 1;
            
        }
        //print_game_board(board); 
       

        steps++;
    }
    return board->max_height;
}



int main() {
    FILE *file = fopen("../data.txt", "r");
    if (!file) {
        perror("Could not open file");
        exit(1);
    }

    RockCollection collection = { .count = 0 };
    char shape1[] = {'#','#','#','#'};
    char shape2[] = {
        '.','#','.',
        '#','#','#',
        '.','#','.'
        };
    char shape3[] = {
        '.','.','#',
        '.','.','#',
        '#','#','#'
        };
    char shape4[] = {
        '#',
        '#',
        '#',
        '#'
        };
    char shape5[] = {
        '#','#',
        '#','#'
        };
    add_rock(&collection, shape1, 1, 4);
    add_rock(&collection, shape2, 3, 3);
    add_rock(&collection, shape3, 3, 3);
    add_rock(&collection, shape4, 4, 1);
    add_rock(&collection, shape5, 2, 2);

    GameBoard board;
    init_game_board(&board);

    //const char *actions = ">>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>";
    char actions[11000]; 
    if (fgets(actions, sizeof(actions), file) == NULL) {
        fclose(file);
        return 1;
    }

    int result = simulate_game(&board,&collection, actions);
    printf("--------------result-------------------------\n");
    //print_game_board(&board);
    printf("result = %d\n", result);
    return 0;
}
