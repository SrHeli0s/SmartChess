#include "pieceShowcase.h"
#include "boardShowcase.h"
#include <stdlib.h> //To use NULL

struct piece gamePieces[4] = {	{P_BISHOP,COLOR_W,28,0,1},//TODO: set pieces positions
								{P_KNIGHT,COLOR_W,32,0,1},
								{P_ROOK,COLOR_W,33,0,1},
								{P_QUEEN,COLOR_W,29,0,1}
								};

struct piece *lastPiece = NULL;


//Pre: "p" is a piece in the game, "output" is an array of size 144 empty or in a known state 
//Post: In output is stored the map of all the possible attacks of "p"
void getAttacks(struct piece p, unsigned char* output) {
	if (p.alive==1) {
		unsigned char offset = p.pos;
		unsigned char offset_temp;

		switch(p.type) {
			case P_PAWN:
			if (p.color == COLOR_W) {
				output[offset+MOVE_N+MOVE_E] = 1;
				output[offset+MOVE_N+MOVE_W] = 1;
			}
			else {
				output[offset+MOVE_S+MOVE_E] = 1;
				output[offset+MOVE_S+MOVE_W] = 1;
			}
			break;

			case P_KNIGHT:
			output[offset+MOVE_N+MOVE_N+MOVE_E] = 1;
			output[offset+MOVE_S+MOVE_S+MOVE_E] = 1;
			output[offset+MOVE_N+MOVE_N+MOVE_W] = 1;
			output[offset+MOVE_S+MOVE_S+MOVE_W] = 1;
			output[offset+MOVE_E+MOVE_E+MOVE_N] = 1;
			output[offset+MOVE_E+MOVE_E+MOVE_S] = 1;
			output[offset+MOVE_W+MOVE_W+MOVE_N] = 1;
			output[offset+MOVE_W+MOVE_W+MOVE_S] = 1;
			break;

			case P_BISHOP:
			//NE
			offset_temp = offset;
			do {
				offset_temp += MOVE_N+MOVE_E;
				output[offset_temp] = 1;
			} while (offset_temp%MOVE_N >1 && offset_temp%MOVE_N <10 && offset_temp>26 && offset_temp<118 && actual_position[offset_temp] ==0); //Not out of the board and not a piece blocking
			//NW
			offset_temp = offset;
			do {
				offset_temp += MOVE_N+MOVE_W;
				output[offset_temp] = 1;
			} while (offset_temp%MOVE_N >1 && offset_temp%MOVE_N <10 && offset_temp>26 && offset_temp<118 && actual_position[offset_temp] ==0); //Not out of the board and not a piece blocking
			//SE
			offset_temp = offset;
			do {
				offset_temp += MOVE_S+MOVE_E;
				output[offset_temp] = 1;
			} while (offset_temp%MOVE_N >1 && offset_temp%MOVE_N <10 && offset_temp>26 && offset_temp<118 && actual_position[offset_temp] ==0); //Not out of the board and not a piece blocking
			//SW
			offset_temp = offset;
			do {
				offset_temp += MOVE_S+MOVE_W;
				output[offset_temp] = 1;
			} while (offset_temp%MOVE_N >1 && offset_temp%MOVE_N <10 && offset_temp>26 && offset_temp<118 && actual_position[offset_temp] ==0); //Not out of the board and not a piece blocking
			break;

			case P_QUEEN:
			//N
			offset_temp = offset;
			do {
				offset_temp += MOVE_N;
				output[offset_temp] = 1;
			} while (offset_temp%MOVE_N >1 && offset_temp%MOVE_N <10 && offset_temp>26 && offset_temp<118 && actual_position[offset_temp] ==0); //Not out of the board and not a piece blocking
			//S
			offset_temp = offset;
			do {
				offset_temp += MOVE_S;
				output[offset_temp] = 1;
			} while (offset_temp%MOVE_N >1 && offset_temp%MOVE_N <10 && offset_temp>26 && offset_temp<118 && actual_position[offset_temp] ==0); //Not out of the board and not a piece blocking
			//E
			offset_temp = offset;
			do {
				offset_temp += MOVE_E;
				output[offset_temp] = 1;
			} while (offset_temp%MOVE_N >1 && offset_temp%MOVE_N <10 && offset_temp>26 && offset_temp<118 && actual_position[offset_temp] ==0); //Not out of the board and not a piece blocking
			//W
			offset_temp = offset;
			do {
				offset_temp += MOVE_W;
				output[offset_temp] = 1;
			} while (offset_temp%MOVE_N >1 && offset_temp%MOVE_N <10 && offset_temp>26 && offset_temp<118 && actual_position[offset_temp] ==0); //Not out of the board and not a piece blocking
			//NE
			offset_temp = offset;
			do {
				offset_temp += MOVE_N+MOVE_E;
				output[offset_temp] = 1;
			} while (offset_temp%MOVE_N >1 && offset_temp%MOVE_N <10 && offset_temp>26 && offset_temp<118 && actual_position[offset_temp] ==0); //Not out of the board and not a piece blocking
			//NW
			offset_temp = offset;
			do {
				offset_temp += MOVE_N+MOVE_W;
				output[offset_temp] = 1;
			} while (offset_temp%MOVE_N >1 && offset_temp%MOVE_N <10 && offset_temp>26 && offset_temp<118 && actual_position[offset_temp] ==0); //Not out of the board and not a piece blocking
			//SE
			offset_temp = offset;
			do {
				offset_temp += MOVE_S+MOVE_E;
				output[offset_temp] = 1;
			} while (offset_temp%MOVE_N >1 && offset_temp%MOVE_N <10 && offset_temp>26 && offset_temp<118 && actual_position[offset_temp] ==0); //Not out of the board and not a piece blocking
			//SW
			offset_temp = offset;
			do {
				offset_temp += MOVE_S+MOVE_W;
				output[offset_temp] = 1;
			} while (offset_temp%MOVE_N >1 && offset_temp%MOVE_N <10 && offset_temp>26 && offset_temp<118 && actual_position[offset_temp] ==0); //Not out of the board and not a piece blocking
			break;

			case P_KING:
			output[offset+MOVE_N] = 1;
			output[offset+MOVE_S] = 1;
			output[offset+MOVE_E] = 1;
			output[offset+MOVE_W] = 1;
			output[offset+MOVE_N+MOVE_E] = 1;
			output[offset+MOVE_S+MOVE_E] = 1;
			output[offset+MOVE_N+MOVE_W] = 1;
			output[offset+MOVE_S+MOVE_W] = 1;
			break;

			case P_ROOK:
			//N
			offset_temp = offset;
			do {
				offset_temp += MOVE_N;
				output[offset_temp] = 1;
			} while (offset_temp%MOVE_N >1 && offset_temp%MOVE_N <10 && offset_temp>26 && offset_temp<118 && actual_position[offset_temp] ==0); //Not out of the board and not a piece blocking
			//S
			offset_temp = offset;
			do {
				offset_temp += MOVE_S;
				output[offset_temp] = 1;
			} while (offset_temp%MOVE_N >1 && offset_temp%MOVE_N <10 && offset_temp>26 && offset_temp<118 && actual_position[offset_temp] ==0); //Not out of the board and not a piece blocking
			//E
			offset_temp = offset;
			do {
				offset_temp += MOVE_E;
				output[offset_temp] = 1;
			} while (offset_temp%MOVE_N >1 && offset_temp%MOVE_N <10 && offset_temp>26 && offset_temp<118 && actual_position[offset_temp] ==0); //Not out of the board and not a piece blocking
			//W
			offset_temp = offset;
			do {
				offset_temp += MOVE_W;
				output[offset_temp] = 1;
			} while (offset_temp%MOVE_N >1 && offset_temp%MOVE_N <10 && offset_temp>26 && offset_temp<118 && actual_position[offset_temp] ==0); //Not out of the board and not a piece blocking
		}
	}
}

//Pre: "p" is a piece in the game, "output" is an array of size 144 empty or in a known state
//Post: In output is stored the map of all the possible moves of "p"
void getMoves(struct piece p, unsigned char* output) {
	if (p.alive==1) {
		unsigned char offset = p.pos;
		//No kings or pawns in showcase
		getAttacks(p,output);
	}
}

