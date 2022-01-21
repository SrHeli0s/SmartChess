#include "boardShowcase.h"
#include "pieceShowcase.h"
#include "string.h" //To use memset()
#include <stdlib.h> //To use abs()
#include <stdio.h> //To use sprintf()

unsigned char actual_position[144] = {0}; //Representation of the actual board
unsigned char detected_position[144] = {0}; //Buffer representation of the board with a new move
unsigned char middle_position[144] = {0}; //Representation of the "middle detected" board
unsigned char possibleMoves[144] = {0};
	
unsigned char white_attacks[144] = {0}; //Possible attacks white can make
unsigned char black_attacks[144] = {0}; //Possible attacks black can make

unsigned char turn = COLOR_W;
char output[20] = "";

//Pre:
//Post: Initializes variables to start a new game
void prepareGame() {
	memset(actual_position, 0, 144*sizeof(char)); //Set to all 0
	memset(detected_position, 0, 144*sizeof(char)); //Set to all 0
	actual_position[112] = (uint8_t)1;
	actual_position[113] = (uint8_t)1;
	actual_position[114] = (uint8_t)1;
	actual_position[115] = (uint8_t)1;
	turn = COLOR_W;
}


//Pre: "a" and "b" are a 144 char array that represents a board position
//Post: returns 0 if the boards are the same, otherwise the number of differences is returned
unsigned char compareBoards() {
	unsigned char output = 0;
	for (int i = 26; i<118; i++) {
		if(i%MOVE_N > 9 || i%MOVE_N < 2) {continue;} //Skip outside of the board
		if (actual_position[i] != detected_position[i]) { output++; }
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
void translateSquare(int pos, int pos2) {
	switch (pos%12) {	//Conversion to ascii using (pos%12)+'a' not possible because of strange bug with atmega...
		case 2:
			output[0]='a';
			break;
		case 3:
			output[0]='b';
			break;
		case 4:
			output[0]='c';
			break;
		case 5:
			output[0]='d';
			break;
		case 6:
			output[0]='e';
			break;
		case 7:
			output[0]='f';
			break;
		case 8:
			output[0]='g';
			break;
		case 9:
			output[0]='h';
			break;
		default: 
			output[0]='i'; //Point outside of board
	}
	//Column
	sprintf(&output[1],"%d",(pos-(pos%12))/12-1);
	
	
	switch (pos2%12) {	//Conversion to ascii using (pos2%12)+'a' not pos2sible because of strange bug with atmega...
		case 2:
		output[2]='a';
		break;
		case 3:
		output[2]='b';
		break;
		case 4:
		output[2]='c';
		break;
		case 5:
		output[2]='d';
		break;
		case 6:
		output[2]='e';
		break;
		case 7:
		output[2]='f';
		break;
		case 8:
		output[2]='g';
		break;
		case 9:
		output[2]='h';
		break;
		default:
		output[2]='i'; //Point outside of board
	}
	//Column
	sprintf(&output[3],"%d",(pos2-(pos2%12))/12-1);
	output[4] = '\n';
}

//Pre: new and middle represent a move that hasn't been done yet
//Post: the coordinate notation of the move is set in output
void translateMove() {
	unsigned char nMoves = 0;
	unsigned char positions[144] = {0}; //144 to prevent overflow
	//Get piece(s) that moved
	for (int i = 26; i<118; i++) {
		if(i%MOVE_N > 9 || i%MOVE_N < 2) {continue;} //Skip outside of the board
		if (detected_position[i] != actual_position[i]) {
			positions[nMoves] = i;
			nMoves++;
		}
	}
	
	if (actual_position[positions[0]] == 1) {
		translateSquare(positions[0],positions[1]);
	}
	else {
		translateSquare(positions[1],positions[0]);
	}
	//No en passant possible in showcase
	//No castle possible in showcase
}

//Pre: "new" is an array of size 144 representing the latest detection of the board. "middle" is an array of size 144 representing the middle detection of the board
//Post: Checks the move made and executes it if its legal, returning 0. Returns -1 if not legal.
int makeMove() {
	unsigned char nMoves = 0;
	unsigned char positions[144] = {0}; //144 to prevent overflow
	//Get piece(s) that moved
	for (int i = 26; i<118; i++) {
		if(i%MOVE_N > 9 || i%MOVE_N < 2) {continue;} //Skip outside of the board
		if (detected_position[i] != actual_position[i]) {
			positions[nMoves] = i;
			nMoves++;
		}
	}
	//Check move
	memset(possibleMoves, 0, 144*sizeof(char));
	
	
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
	getMoves(*p);
	if(possibleMoves[final_pos]==1) {			
		p->pos = final_pos;
		p->nMoves += 1;
		for(int i = 26; i<118; i++) {
			if(i%MOVE_N > 9 || i%MOVE_N < 2) {
				continue;
			}
			actual_position[i] = detected_position[i];
		};
		lastPiece = p;
		if(turn==COLOR_W) { turn = COLOR_B; }
		else { turn = COLOR_W; }
		return 0;
	}
	//No en passant possible in showcase
	//No castle possible in showcase

	return -1; //If reached here is not a legal move
}