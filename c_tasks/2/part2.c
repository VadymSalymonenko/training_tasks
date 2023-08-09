// Link: https://adventofcode.com/2022/day/2

#include <stdio.h>

enum {
    ROCK = 1,
    PAPER,
    SCISSORS
};

int decode_move(char move) {
    switch (move) {
        case 'A':  // Rock
            return ROCK;
        case 'B':  // Paper
            return PAPER;
        case 'C':  // Scissors
            return SCISSORS;
        default:
            return 0;
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
    FILE *file = fopen("strategy.txt", "r");
    char opponent_move_char, desired_outcome;
    int opponent_move, your_move;
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
