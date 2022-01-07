#include "board.h"
#include "piece.h"
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

//Pre: "pos" is an index of the map. "output" is a string
//Post: The square name ([a-h][1-8]) is appended to output
void translateSquare(int pos, char* output) {
	switch (pos%12) {
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
//Post: the coordinate notation of the move is returned
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
	else if (nMoves==3) { //En passant
		//Origin is square of the capturing pawn
		//Final is empty square
		for (int i=0; i<32; i++) {
			if (gamePieces[i].pos == positions[0] || gamePieces[i].pos == positions[1] || gamePieces[i].pos == positions[2]) {
				if(gamePieces[i].color == turn) {
					translateSquare(gamePieces[i].pos,output);
					break;
				}
			}
		}
		if (actual_position[positions[0]] == 0) {
			translateSquare(positions[0],output);
		}
		else if (actual_position[positions[1]] == 0) {
			translateSquare(positions[1],output);
		}
		else if (actual_position[positions[2]] == 0) {
			translateSquare(positions[2],output);
		}
	}
	else if (nMoves==4) { //Castle (Represented by the movement of the king)
		//Origin is king pos
		//Final is king pos after castle
		if (turn == COLOR_W) {
			translateSquare(30,output);
			//Long castle
			if (positions[0]==26 && positions[1]==28 && positions[2]==29 && positions[3]==30) {
				translateSquare(28,output);
			}
			//Short castle
			else if (positions[0]==30 && positions[1]==31 && positions[2]==32 && positions[3]==33) { //Short castle
				translateSquare(32,output);
			}
		}
		else {
			translateSquare(114,output);
			//Long castle
			if (positions[0]==110 && positions[1]==112 && positions[2]==113 && positions[3]==114) {
				translateSquare(112,output);
			}
			//Short castle
			else if (positions[0]==114 && positions[1]==115 && positions[2]==116 && positions[3]==117) { //Short castle
				translateSquare(116,output);
			}
		}
	}
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
			//Promotion of pawns
			if (turn == COLOR_W) {
				if(final_pos>109 && p->type==P_PAWN) { p->type = P_QUEEN; }
			}
			else {
				if(final_pos<34 && p->type==P_PAWN) { p->type = P_QUEEN; }
			}
			
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