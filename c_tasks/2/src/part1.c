// Link: https://adventofcode.com/2022/day/2

#include <stdio.h>

enum {
    ROCK = 1,
    PAPER,
    SCISSORS
};


int decode_move(char move) {
    switch (move) {
        case 'A': case 'X':  // Rock
            return ROCK;
        case 'B': case 'Y':  // Paper
            return PAPER;
        case 'C': case 'Z':  // Scissors
            return SCISSORS;
        default:
            return 0;
    }
}

int get_round_winner(int opponent_move, int your_move) {
    if (opponent_move == your_move) {  // Draw
        return 3;
    }
    
    switch (opponent_move) {
        case ROCK:  // Opponent Rock
            return (your_move == PAPER) ? 6 : 0;
        case PAPER:  // Opponent Paper
            return (your_move == SCISSORS) ? 6 : 0;
        case SCISSORS:  // Opponent Scissors
            return (your_move == ROCK) ? 6 : 0;
        default:
            return 0;
    }
}

int main() {
    FILE *file = fopen("../strategy.txt", "r");
    char opponent_move_char, your_move_char;
    int opponent_move, your_move;
    int your_total_score = 0, opponent_total_score = 0;

    while (fscanf(file, " %c %c", &opponent_move_char, &your_move_char) == 2) {
        opponent_move = decode_move(opponent_move_char);
        your_move = decode_move(your_move_char);
        your_total_score += your_move + get_round_winner(opponent_move, your_move);
        opponent_total_score += opponent_move;
    }

    printf("Your total score: %d\n", your_total_score);
    printf("Opponent's total score: %d\n", opponent_total_score);

    fclose(file);
    return 0;
}
