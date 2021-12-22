#include "chess.h"
#include "string.h" //To use memset()
#include <stdlib.h> //To use abs()

struct piece gamePieces[32] = {	{P_PAWN,COLOR_W,38,0,1},
								{P_PAWN,COLOR_W,39,0,1},
								{P_PAWN,COLOR_W,40,0,1},
								{P_PAWN,COLOR_W,41,0,1},
								{P_PAWN,COLOR_W,42,0,1},
								{P_PAWN,COLOR_W,43,0,1},
								{P_PAWN,COLOR_W,44,0,1},
								{P_PAWN,COLOR_W,45,0,1},
								{P_BISHOP,COLOR_W,28,0,1},
								{P_BISHOP,COLOR_W,31,0,1},
								{P_KNIGHT,COLOR_W,27,0,1},
								{P_KNIGHT,COLOR_W,32,0,1},
								{P_ROOK,COLOR_W,26,0,1},
								{P_ROOK,COLOR_W,33,0,1},
								{P_KING,COLOR_W,30,0,1},
								{P_QUEEN,COLOR_W,29,0,1},
								{P_PAWN,COLOR_B,98,0,1},
								{P_PAWN,COLOR_B,99,0,1},
								{P_PAWN,COLOR_B,100,0,1},
								{P_PAWN,COLOR_B,101,0,1},
								{P_PAWN,COLOR_B,102,0,1},
								{P_PAWN,COLOR_B,103,0,1},
								{P_PAWN,COLOR_B,104,0,1},
								{P_PAWN,COLOR_B,105,0,1},
								{P_BISHOP,COLOR_B,112,0,1},
								{P_BISHOP,COLOR_B,115,0,1},
								{P_KNIGHT,COLOR_B,111,0,1},
								{P_KNIGHT,COLOR_B,116,0,1},
								{P_ROOK,COLOR_B,110,0,1},
								{P_ROOK,COLOR_B,117,0,1},
								{P_KING,COLOR_B,114,0,1},
								{P_QUEEN,COLOR_B,113,0,1}
								};

struct piece *lastPiece = NULL;

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
	//TODO: Reset pieces
	
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
			
			case P_KING:
			getAttacks(p,output);
			if (p.color == COLOR_W) {
				//Delete moves that put the king in check
				updateAttackMap(COLOR_B);
				for (int i = 26; i<118; i++) {
					if(i%MOVE_N > 9 || i%MOVE_N < 2) {continue;} //Skip outside of the board
					if (output[i]==1 && black_attacks[i]==1) { output[i]=0; }
				}
				//Castling
				if (black_attacks[p.pos]==0) { //Not in check
					if (p.nMoves==0) {
						//Long castle
						if(gamePieces[12].nMoves==0 && black_attacks[28]==0 && black_attacks[29]==0 && actual_position[27]==0 && actual_position[28]==0 && actual_position[29]==0) { //Rook didn't move, no checks or pieces in the way
							output[28] = 1;
						}
						//Short castle
						if(gamePieces[13].nMoves==0 && black_attacks[31]==0 && black_attacks[32]==0 && actual_position[31]==0 && actual_position[32]==0) { //Rook didn't move, no checks or pieces in the way
							output[32] = 1;
						}
					}
				}
			}
			else {
				//Delete moves that put the king in check
				updateAttackMap(COLOR_W);
				for (int i = 26; i<118; i++) {
					if(i%MOVE_N > 9 || i%MOVE_N < 2) {continue;} //Skip outside of the board
					if (output[i]==1 && white_attacks[i]==1) { output[i]=0; }
				}
				//Castling
				if (white_attacks[p.pos]==0) { //Not in check
					if (p.nMoves==0) {
						//Long castle
						if(gamePieces[28].nMoves==0 && white_attacks[113]==0 && white_attacks[112]==0 && actual_position[113]==0 && actual_position[112]==0 && actual_position[111]==0) { //Rook didn't move, no checks or pieces in the way
							output[112] = 1;
						}
						//Short castle
						if(gamePieces[29].nMoves==0 && white_attacks[115]==0 && white_attacks[116]==0 && actual_position[115]==0 && actual_position[116]==0) { //Rook didn't move, no checks or pieces in the way
							output[116] = 1;
						}
					}
				}
			}
			break;

			default:
				getAttacks(p,output);
		}
	}
}

//Pre: "new" is an array of size 144 representing the lastest detection of the board. "middle" is an array of size 144 representing the middle detection of the board
//Post: Checks the move made and executes it if its legal, returning 0. Returns -1 if not legal.
int makeMove(unsigned char* new, unsigned char* middle) {
	//TODO: Handle promotion of pawns to queen
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
		if (turn == COLOR_W) {
			for (int i=0; i<16; i++) {
				if (gamePieces[i].pos == positions[0]) {
					p = &gamePieces[i];
					break;
				}
			}
		}
		else {
			for (int i=16; i<32; i++) {
				if (gamePieces[i].pos == positions[0]) {
					p = &gamePieces[i];
					break;
				}
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
		if (turn == COLOR_B) {
			for (int i=0; i<16; i++) {
				if (gamePieces[i].pos == final_pos) {
					p2 = &gamePieces[i];
					break;
				}
			}
		}
		else {
			for (int i=16; i<32; i++) {
				if (gamePieces[i].pos == final_pos) {
					p2 = &gamePieces[i];
					break;
				}
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
		if (turn == COLOR_W) {
			for (int i=0; i<16; i++) {
				if (gamePieces[i].pos == origin_pos) {
					p = &gamePieces[i];
					break;
				}
			}
		}
		else {
			for (int i=16; i<32; i++) {
				if (gamePieces[i].pos == origin_pos) {
					p = &gamePieces[i];
					break;
				}
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
	else if (nMoves == 3) { //En passant capture
		// Get pieces moved
		struct piece* p = NULL;
		struct piece* p2 = NULL;
		if (turn == COLOR_W) {
			for (int i=0; i<16; i++) {
				if (gamePieces[i].pos == positions[0] || gamePieces[i].pos == positions[1]) {
					p = &gamePieces[i];
					break;
				}
			}
			for (int i=16; i<32; i++) {
				if (gamePieces[i].pos == positions[0] || gamePieces[i].pos == positions[1]) {
					p2 = &gamePieces[i];
					break;
				}
			}
			//Check if move is in the list of possible white moves and execute if legal
			if (p!=NULL && p2!=NULL && abs(p->pos - p2->pos)==1 && (p2->pos+MOVE_N)==positions[2] && p2->nMoves==1 && p2==lastPiece) { //Pawns were side by side, final position is on top of black pawn, black pawn only moved once and is the last piece moved
				p->nMoves += 1;
				p->pos = positions[2];
				p2->nMoves += 1;
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
		else {
			for (int i=16; i<32; i++) {
				if (gamePieces[i].pos == positions[1] || gamePieces[i].pos == positions[2]) {
					p = &gamePieces[i];
					break;
				}
			}
			for (int i=0; i<16; i++) {
				if (gamePieces[i].pos == positions[1] || gamePieces[i].pos == positions[2]) {
					p2 = &gamePieces[i];
					break;
				}
			}
			//Check if move is in the list of possible white moves and execute if legal
			if (p!=NULL && p2!=NULL && abs(p->pos - p2->pos)==1 && (p2->pos+MOVE_S)==positions[0] && p2->nMoves==1 && p2==lastPiece) { //Pawns were side by side, final position is bottom of white pawn, white pawn only moved once and is the last piece moved
				p->nMoves += 1;
				p->pos = positions[0];
				p2->nMoves += 1;
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

	}
	else if (nMoves == 4) { //Castle
		if (turn == COLOR_W) {
			//Long castle
			if (positions[0]==26 && positions[1]==28 && positions[2]==29 && positions[3]==30) {
				//Check if move is in list of possible moves and execute if legal
				getMoves(gamePieces[12],possibleMoves);
				getMoves(gamePieces[14],possibleMoves);
				if (possibleMoves[positions[1]]==1 && possibleMoves[positions[2]]==1) {
					gamePieces[12].nMoves += 1;
					gamePieces[12].pos = 29;
					gamePieces[14].nMoves += 1;
					gamePieces[14].pos = 28;
					for(int i = 26; i<118; i++) {
						if(i%MOVE_N > 9 || i%MOVE_N < 2) {
							continue;
						}
						actual_position[i] = new[i];
					};
					lastPiece = &gamePieces[14];
					if(turn==COLOR_W) { turn = COLOR_B; }
				else { turn = COLOR_W; }
					return 0;
				}
			}
			//Short castle
			else if (positions[0]==30 && positions[1]==31 && positions[2]==32 && positions[3]==33) { //Short castle
				//Check if move is in list of possible moves and execute if legal
				getMoves(gamePieces[13],possibleMoves);
				getMoves(gamePieces[14],possibleMoves);
				if (possibleMoves[positions[1]]==1 && possibleMoves[positions[2]]==1) {
					gamePieces[14].nMoves += 1;
					gamePieces[14].pos = 32;
					gamePieces[13].nMoves += 1;
					gamePieces[13].pos = 31;
					for(int i = 26; i<118; i++) {
						if(i%MOVE_N > 9 || i%MOVE_N < 2) {
							continue;
						}
						actual_position[i] = new[i];
					};
					lastPiece = &gamePieces[14];
					if(turn==COLOR_W) { turn = COLOR_B; }
				else { turn = COLOR_W; }
					return 0;
				}
			}
		}
		else {
			//Long castle
			if (positions[0]==110 && positions[1]==112 && positions[2]==113 && positions[3]==114) {
				//Check if move is in list of possible moves and execute if legal
				getMoves(gamePieces[28],possibleMoves);
				getMoves(gamePieces[30],possibleMoves);
				if (possibleMoves[positions[1]]==1 && possibleMoves[positions[2]]==1) {
					gamePieces[30].nMoves += 1;
					gamePieces[30].pos = 112;
					gamePieces[28].nMoves += 1;
					gamePieces[28].pos = 113;
					for(int i = 26; i<118; i++) {
						if(i%MOVE_N > 9 || i%MOVE_N < 2) {
							continue;
						}
						actual_position[i] = new[i];
					};
					lastPiece = &gamePieces[30];
					if(turn==COLOR_W) { turn = COLOR_B; }
					else { turn = COLOR_W; }
					return 0;
				}
			}
			//Short castle
			else if (positions[0]==114 && positions[1]==115 && positions[2]==116 && positions[3]==117) { //Short castle
				//Check if move is in list of possible moves and execute if legal
				getMoves(gamePieces[29],possibleMoves);
				getMoves(gamePieces[30],possibleMoves);
				if (possibleMoves[positions[1]]==1 && possibleMoves[positions[2]]==1) {
					gamePieces[30].nMoves += 1;
					gamePieces[30].pos = 116;
					gamePieces[29].nMoves += 1;
					gamePieces[29].pos = 115;
					for(int i = 26; i<118; i++) {
						if(i%MOVE_N > 9 || i%MOVE_N < 2) {
							continue;
						}
						actual_position[i] = new[i];
					};
					lastPiece = &gamePieces[30];
					if(turn==COLOR_W) { turn = COLOR_B; }
					else { turn = COLOR_W; }
					return 0;
				}
			}
		}
	}

	return -1; //If reached here is not a legal move
}