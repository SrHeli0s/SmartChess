#include "Smartchess/piece.c"
#include "Smartchess/board.c"
#include <stdio.h>

const unsigned char MASKS[8] = {(1<<0), (1<<1), (1<<2), (1<<3), (1<<4), (1<<5), (1<<6), (1<<7)};


//Pre: "output" is an array of at least 8 chars in a known state.
//Post: The row read data is saved to the array
void readRow(unsigned char output[], unsigned char input) {
    printf("Result: ");
	if ((input & MASKS[0]) == 0) {
		output[0] = 0;
        printf("0");
	}
	else {
		output[0] = 1;
        printf("X");
	}
	if ((input & MASKS[1]) == 0) {
		output[1] = 0;
        printf("0");
	}
	else {
		output[1] = 1;
        printf("X");
	}
	if ((input & MASKS[2]) == 0) {
		output[2] = 0;
        printf("0");
	}
	else {
		output[2] = 1;
        printf("X");
	}
	
	if ((input & MASKS[3]) == 0) {
		output[3] = 0;
        printf("0");
	}
	else {
		output[3] = 1;
        printf("X");
	}
	
	if ((input & MASKS[4]) == 0) {
		output[4] = 0;
        printf("0");
	}
	else {
		output[4] = 1;
        printf("X");
	}
	
	if ((input & MASKS[5]) == 0) {
		output[5] = 0;
        printf("0");
	}
	else {
		output[5] = 1;
        printf("X");
	}
	
	if ((input & MASKS[6]) == 0) {
		output[6] = 0;
        printf("0");
	}
	else {
		output[6] = 1;
        printf("X");
	}
	
	if ((input & MASKS[7]) == 0) {
		output[7] = 0;
        printf("0");
	}
	else {
		output[7] = 1;
        printf("X");
	}
	printf("\n");
}

//Pre: "output" is an array of 144 chars in a known state.
//Post: The board is read and stored in "output".
void readBoard(unsigned char output[]) {
	int i=26; //for offset
    unsigned char inputs[8] = {0,2,4,8,16,32,64,128};
	
	printf("Reading new row...   ");
	readRow(&output[i],inputs[0]);
	i+=MOVE_N;
	
	printf("Reading new row...   ");
	readRow(&output[i],inputs[1]);
	i+=MOVE_N;
	
	printf("Reading new row...   ");
	readRow(&output[i],inputs[2]);
	i+=MOVE_N;
	
	printf("Reading new row...   ");
	readRow(&output[i],inputs[3]);
	i+=MOVE_N;
	
	printf("Reading new row...   ");
	readRow(&output[i],inputs[4]);
	i+=MOVE_N;
	
	printf("Reading new row...   ");
	readRow(&output[i],inputs[5]);
	i+=MOVE_N;
	
	printf("Reading new row...   ");
	readRow(&output[i],inputs[6]);
	i+=MOVE_N;
	
	printf("Reading new row...   ");
	readRow(&output[i],inputs[7]);
}




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
    char translation[] = "";
    translateMove(detected_position,middle_position,translation);
    printf("Move: %s+%s, translated: %s\n",move1,move2,translation);
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
            printf("=====EXPLODED on move %u=====",j);
            return -1;
        }
    }
    printf("\n\nEND OF GAME:\n");
    print_board(actual_position);

    printf("NOW PRINTING THE DIAGONAL:");
    for (int i = 26; i<118; i=i+13) {
        char output[] = "";
        translateSquare(i,output);
        printf(output);
    }


    printf("\n\n\nNEW TEST COMING!!\n");
    unsigned char actualBoard[144] = {0};

    readBoard(actualBoard);

    print_board(actualBoard);


    return 0;
}