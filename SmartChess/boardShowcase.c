#include "boardShowcase.h"
#include "pieceShowcase.h"
#include "string.h" //To use memset()
#include <stdlib.h> //To use abs()
#include <stdio.h> //To use sprintf()

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
	
	turn = COLOR_W;
}

//Pre: The attack maps are in an unknown state. "c" is a color (COLOR_W or COLOR_B)
//Post: The attack map of the color c is updated.
void updateAttackMap(int c) {
	if (c == COLOR_W) {
		memset(white_attacks, 0, sizeof white_attacks); //Set to all 0
		for (int i=0; i<4; i++) {
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

//Pre: "pos" is an index of the map. "output" is a string
//Post: The square name ([a-h][1-8]) is appended to output
void translateSquare(int pos, char* output) {
	switch (pos%12) {	//Conversion to ascii using (pos%12)+'a' not possible because of strange bug with atmega...
		case 2:
			strcat(output,"a");
			break;
		case 3:
			strcat(output,"b");
			break;
		case 4:
			strcat(output,"c");
			break;
		case 5:
			strcat(output,"d");
			break;
		case 6:
			strcat(output,"e");
			break;
		case 7:
			strcat(output,"f");
			break;
		case 8:
			strcat(output,"g");
			break;
		case 9:
			strcat(output,"h");
			break;
		default: 
			strcat(output,"i"); //Point outside of board
	}
	//Column
	char col[] = "";
	sprintf(col,"%d",(pos-(pos%12))/12-1);
	strcat(output, col);
}

//Pre: new and middle represent a move that hasn't been done yet
//Post: the coordinate notation of the move is set in output
void translateMove(unsigned char* new, unsigned char* middle, char* output) {
	unsigned char nMoves = 0;
	unsigned char positions[144] = {0}; //144 to prevent overflow
	//Get piece(s) that moved
	for (int i = 26; i<118; i++) {
		if(i%MOVE_N > 9 || i%MOVE_N < 2) {continue;} //Skip outside of the board
		if (new[i] != actual_position[i]) {
			positions[nMoves] = i;
			nMoves++;
		}
	}
	if (nMoves==1) { //Capture move
		//Origin is positions[0]
		//Final is the difference between middle and actual
		translateSquare(positions[0],output);
		
		for (int i = 26; i<118; i++) {
			if(i%MOVE_N > 9 || i%MOVE_N < 2) {continue;} //Skip outside of the board
			if (actual_position[i] != middle[i]) {
				translateSquare(i,output);
				break;
			}
		}
	}
	else if (nMoves==2) { //Normal move
		//Origin is the one in positions that does have a piece
		//Final is the one in positions that doesn't have a piece
		
		if (actual_position[positions[0]] == 1) {
			translateSquare(positions[0],output);
			translateSquare(positions[1],output);
		}
		else {
			translateSquare(positions[1],output);
			translateSquare(positions[0],output);
		}
		
	}
	//No en passant possible in showcase
	//No castle possible in showcase
}

//Pre: "new" is an array of size 144 representing the latest detection of the board. "middle" is an array of size 144 representing the middle detection of the board
//Post: Checks the move made and executes it if its legal, returning 0. Returns -1 if not legal.
int makeMove(unsigned char* new, unsigned char* middle) {
	unsigned char nMoves = 0;
	unsigned char positions[144] = {0}; //144 to prevent overflow
	//Get piece(s) that moved
	for (int i = 26; i<118; i++) {
		if(i%MOVE_N > 9 || i%MOVE_N < 2) {continue;} //Skip outside of the board
		if (new[i] != actual_position[i]) {
			positions[nMoves] = i;
			nMoves++;
		}
	}
	//Check move
	unsigned char possibleMoves[144] = {0};
	if (nMoves==1) { //Capture move
		//Get piece moved
		struct piece* p = NULL;
		for (int i=0; i<4; i++) {
			if (gamePieces[i].pos == positions[0]) {
				p = &gamePieces[i];
				break;
			}
		}
		//Get which piece was captured
		unsigned char final_pos = 0;
		for (int i = 26; i<118; i++) {
			if (i%MOVE_N > 9 || i%MOVE_N < 2) {continue;} //Skip outside of the board
			if (middle[i] != actual_position[i]) {
				final_pos = i;
				break;
			}
		}
		struct piece* p2 = NULL;
		for (int i=0; i<4; i++) {
			if (gamePieces[i].pos == final_pos) {
				p2 = &gamePieces[i];
				break;
			}
		}
		// Check if move is in the list of possible moves and execute if legal
		getAttacks(*p,possibleMoves);
		if(possibleMoves[final_pos]==1) {
			p->pos = final_pos;
			p->nMoves += 1;
			p2->pos = 0; //Move out of the board
			p2->alive = 0;
			for(int i = 26; i<118; i++) {
				if(i%MOVE_N > 9 || i%MOVE_N < 2) {
					continue;
				}
				actual_position[i] = new[i];
			};
			lastPiece = p;
			if(turn==COLOR_W) { turn = COLOR_B; }
			else { turn = COLOR_W; }
			return 0;
		}
	}
	else if (nMoves == 2) { //Normal move
		//Get origin and final pos of the move
		unsigned char origin_pos;
		unsigned char final_pos;
		if (actual_position[positions[0]] == 1) {
			origin_pos = positions[0];
			final_pos = positions[1];
		}
		else {
			origin_pos = positions[1];
			final_pos = positions[0];
		}
		//Get piece moved
		struct piece* p = 0;
		for (int i=0; i<4; i++) {
			if (gamePieces[i].pos == origin_pos) {
				p = &gamePieces[i];
				break;
			}
		}
		//Check if move is in the list of possible moves and execute if legal
		getMoves(*p,possibleMoves);
		if(possibleMoves[final_pos]==1) {			
			p->pos = final_pos;
			p->nMoves += 1;
			for(int i = 26; i<118; i++) {
				if(i%MOVE_N > 9 || i%MOVE_N < 2) {
					continue;
				}
				actual_position[i] = new[i];
			};
			lastPiece = p;
			if(turn==COLOR_W) { turn = COLOR_B; }
			else { turn = COLOR_W; }
			return 0;
		}
	}
	//No en passant possible in showcase
	//No castle possible in showcase

	return -1; //If reached here is not a legal move
}