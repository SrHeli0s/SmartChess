#include "board.h"
#include "piece.h"
#include "string.h" //To use memset()

unsigned char actual_position[144] = {0}; //Representation of the actual board
unsigned char detected_position[144] = {0}; //Buffer representation of the board with a new move
unsigned char middle_position[144] = {0}; //Representation of the "middle detected" board

unsigned char white_attacks[144] = {0}; //Possible attacks white can make
unsigned char black_attacks[144] = {0}; //Possible attacks black can make

unsigned char turn = COLOR_W;


//Pre:
//Post: Initializes variables to start a new game
void prepareGame() {
	memset(actual_position, 0, sizeof actual_position); //Set to all 0
	memset(detected_position, 0, sizeof detected_position); //Set to all 0
	for (int i=26;i<34;i++){
		actual_position[i] = 1;
		detected_position[i] = 1;
	}
	for (int i=38;i<46;i++){
		actual_position[i] = 1;
		detected_position[i] = 1;
	}
	for (int i=98;i<106;i++){
		actual_position[i] = 1;
		detected_position[i] = 1;
	}
	for (int i=110;i<118;i++){
		actual_position[i] = 1;
		detected_position[i] = 1;
	}
	
	turn = COLOR_W;
}

//Pre: The attack maps are in an unknown state. "c" is a color (COLOR_W or COLOR_B)
//Post: The attack map of the color c is updated.
void updateAttackMap(int c) {
	if (c == COLOR_W) {
		memset(white_attacks, 0, sizeof white_attacks); //Set to all 0
		for (int i=0; i<16; i++) {
			if (gamePieces[i].alive == 1) {
				getAttacks(gamePieces[i],white_attacks);
			}
		}
	}
	else {
		memset(black_attacks, 0, sizeof black_attacks); //Set to all 0
		for (int i=16; i<32; i++) { // Offset i to use black pieces instead of white ones
			if (gamePieces[i].alive == 1) {
				getAttacks(gamePieces[i],black_attacks);
			}
		}
	}
}

//Pre: "a" and "b" are a 144 char array that represents a board position
//Post: returns 0 if the boards are the same, otherwise the number of differences is returned
unsigned char compareBoards(unsigned char* a, unsigned char* b) {
	unsigned char output = 0;
	for (int i = 26; i<118; i++) {
		if(i%MOVE_N > 9 || i%MOVE_N < 2) {continue;} //Skip outside of the board
		if (a[i] != b[i]) { output++; }
	}
	return output;
}

//Pre: "from" and "to" are a 144 char array that represents a board position
//Post: "from" is copied to "to"
void copyBoards(unsigned char* from, unsigned char* to) {
	for (int i = 26; i<118; i++) {
		if(i%MOVE_N > 9 || i%MOVE_N < 2) {continue;} //Skip outside of the board
		to[i] = from[i];
	}
}