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
		switch(p.type) {
			case P_PAWN:
			if (p.color == COLOR_W) {
				output[offset+MOVE_N] = 1;
				if (p.nMoves==0) {
					output[offset+MOVE_N+MOVE_N] = 1;
				}
			}
			else {
				output[offset+MOVE_S] = 1;
				if (p.nMoves==0) {
					output[offset+MOVE_S+MOVE_S] = 1;
				}
			}
			break;
			
			//Commented to avoid possible errors in showcase:
			
			//case P_KING:
			//getAttacks(p,output);
			//if (p.color == COLOR_W) {
				////Delete moves that put the king in check
				//updateAttackMap(COLOR_B);
				//for (int i = 26; i<118; i++) {
					//if(i%MOVE_N > 9 || i%MOVE_N < 2) {continue;} //Skip outside of the board
					//if (output[i]==1 && black_attacks[i]==1) { output[i]=0; }
				//}
				////Castling
				//if (black_attacks[p.pos]==0) { //Not in check
					//if (p.nMoves==0) {
						////Long castle
						//if(gamePieces[12].nMoves==0 && black_attacks[28]==0 && black_attacks[29]==0 && actual_position[27]==0 && actual_position[28]==0 && actual_position[29]==0) { //Rook didn't move, no checks or pieces in the way
							//output[28] = 1;
						//}
						////Short castle
						//if(gamePieces[13].nMoves==0 && black_attacks[31]==0 && black_attacks[32]==0 && actual_position[31]==0 && actual_position[32]==0) { //Rook didn't move, no checks or pieces in the way
							//output[32] = 1;
						//}
					//}
				//}
			//}
			//else {
				////Delete moves that put the king in check
				//updateAttackMap(COLOR_W);
				//for (int i = 26; i<118; i++) {
					//if(i%MOVE_N > 9 || i%MOVE_N < 2) {continue;} //Skip outside of the board
					//if (output[i]==1 && white_attacks[i]==1) { output[i]=0; }
				//}
				////Castling
				//if (white_attacks[p.pos]==0) { //Not in check
					//if (p.nMoves==0) {
						////Long castle
						//if(gamePieces[28].nMoves==0 && white_attacks[113]==0 && white_attacks[112]==0 && actual_position[113]==0 && actual_position[112]==0 && actual_position[111]==0) { //Rook didn't move, no checks or pieces in the way
							//output[112] = 1;
						//}
						////Short castle
						//if(gamePieces[29].nMoves==0 && white_attacks[115]==0 && white_attacks[116]==0 && actual_position[115]==0 && actual_position[116]==0) { //Rook didn't move, no checks or pieces in the way
							//output[116] = 1;
						//}
					//}
				//}
			//}
			//break;

			default:
				getAttacks(p,output);
		}
	}
}

