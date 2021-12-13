#include "Smartchess/chess.c"
#include <assert.h>
#include <stdio.h>

/*
http://marianogappa.github.io/ostinato-examples/convert.html
game: https://lichess.org/study/fh4EeQUg
e2e4 g8f6
d2d4 e7e6
e4e5 d7d6
e5f6 g7f6
g1f3 f8h6
c1g5 f6g5
d1d3 c8d7
b1d2 d8f6
e1c1 e8g8
c2c4 b8a6
c4c5 d6c5
d4c5 b7b5
c5b6 c7b6
d3a6 f8c8
c1b1 c8c1
b1c1 h6g7
f3d4 f6d4
a6d3 d4b2
*/

//Fancy prints a board
void print_board(unsigned char* board) {
    for(int i = 26; i<118; i++) {
        if(i%MOVE_N > 9 || i%MOVE_N < 2) { continue; }
        printf("%u",board[i]);
        if (i%12 == 9) {
            printf("\n");
        }
    }
}

//Tries to execute the following moves: move1 as middle possition and move2 as final position
int tryMove(char* move1, char* move2) {
    //Calculate offset of moves
    int origin1 = (move1[1]-'1')*MOVE_N + move1[0]-'a' + 26;
    int final1 = (move1[3]-'1')*MOVE_N + move1[2]-'a' + 26;
    
    int origin2 = (move2[1]-'1')*MOVE_N + move2[0]-'a' + 26;
    int final2 = (move2[3]-'1')*MOVE_N + move2[2]-'a' + 26;
    
    //Reset middle_position to actual_position
    for(int i = 26; i<118; i++) { //NOTE: I HATE C ARRAYS
        if(i%12 > 9 || i%12 < 2) {
            continue;
        }
        middle_position[i] = actual_position[i];
    }

    //Prepare boards
    middle_position[origin1] = 0;
    middle_position[final1] = 1;
    detected_position[origin1] = 0;
    detected_position[final1] = 1;
    detected_position[origin2] = 0;
    detected_position[final2] = 1;

    if (strcmp(move1,move2) != 0) {
        printf("Middle board:\n");
        print_board(middle_position);
    }
    printf("Final board:\n");
    print_board(detected_position);
    
    //Simulate move
    return makeMove(detected_position,middle_position);
}



int main() {
    prepareGame();
    char* game[36][2] = {{"e2e4","e2e4"}, {"g8f6","g8f6"}, {"d2d4","d2d4"}, {"e7e6","e7e6"}, {"e4e5","e4e5"}, {"d7d6","d7d6"}, {"f6_0","e5f6"}, {"f6_0","g7f6"}, {"g1f3","g1f3"}, {"f8h6","f8h6"}, {"c1g5","c1g5"}, {"g5_0","f6g5"}, {"d1d3","d1d3"}, {"c8d7","c8d7"}, {"b1d2","b1d2"}, {"d8f6","d8f6"}, {"a1d1","e1c1"}, {"h8f8","e8g8"}, {"c2c4","c2c4"}, {"b8a6","b8a6"}, {"c4c5","c4c5"}, {"c5_0","d6c5"}, {"c5_0","d4c5"}, {"b7b5","b7b5"}, {"b5_0","c5b6"}, {"b6_0","c7b6"}, {"a6_0","d3a6"}, {"f8c8","f8c8"}, {"c1b1","c1b1"}, {"c8c1","c8c1"}, {"c1_0","b1c1"}, {"h6g7","h6g7"}, {"f3d4","f3d4"}, {"d4_0","f6d4"}, {"a6d3","a6d3"}, {"b2_0","d4b2"}};
    for (int j = 0; j<36; j++) {
        printf("\n\n----Trying move %u \nActual board:\n",j);
        print_board(actual_position);
        if(tryMove(game[j][0],game[j][1]) == -1) {
            printf("EXPLODED");
            return -1;
        }
    }
    printf("\n\nEND OF GAME:\n");
    print_board(actual_position);
    
    return 0;
}