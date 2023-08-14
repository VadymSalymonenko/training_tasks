// Link: https://adventofcode.com/2022/day/2

#include <stdio.h>

typedef enum{
    ROCK = 1,
    PAPER,
    SCISSORS,
    INVALID = -1
} MoveType;


MoveType decode_move(char move) {
    switch (move) {
        case 'A':  // Rock
            return ROCK;
        case 'B':  // Paper
            return PAPER;
        case 'C':  // Scissors
            return SCISSORS;
        default:
            return INVALID;
    }
}

int choose_your_move(int opponent_move, char desired_outcome) {
    switch (desired_outcome) {
        case 'X':  // You need to lose
            if(opponent_move - 1 > 0){
                return opponent_move - 1;
            }else{
                return SCISSORS;
            }
        case 'Y':  // You need a draw
            return opponent_move;
        case 'Z':  // You need to win
            if(opponent_move + 1 < 4){
                return opponent_move + 1;
            }else{
                return ROCK;
            }        
        default:
            return 0;
    }
}

int get_score_for_outcome(char outcome) {
    switch (outcome) {
        case 'X':  // You lost
            return 0;
        case 'Y':  // Draw
            return 3;
        case 'Z':  // You won
            return 6;
        default:
            return 0;
    }
}

int main() {
    FILE *file = fopen("../strategy.txt", "r");
    if (!file) {
        printf("\033[1;31m"); 
        perror("Error opening file"); 
        printf("\033[0m");  
        return 1;
    }
    char opponent_move_char, desired_outcome;
    MoveType opponent_move, your_move;
    int your_total_score = 0;

    while (fscanf(file, " %c %c", &opponent_move_char, &desired_outcome) == 2) {
        opponent_move = decode_move(opponent_move_char);
        your_move = choose_your_move(opponent_move, desired_outcome);
        your_total_score += your_move + get_score_for_outcome(desired_outcome);
    }

    printf("Your total score: %d\n", your_total_score);

    fclose(file);
    return 0;
}
