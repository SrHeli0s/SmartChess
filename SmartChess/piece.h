#ifndef PIECE_H_
#define PIECE_H_

#define COLOR_W 1
#define COLOR_B 0

#define P_KING 1
#define P_ROOK 2
#define P_BISHOP 3
#define P_KNIGHT 4
#define P_PAWN 5
#define P_QUEEN 6

struct piece {
	unsigned char type;
	unsigned char color;
	unsigned char pos;
	unsigned char nMoves;
	unsigned char alive;
};

extern struct piece gamePieces[32];
extern struct piece *lastPiece;


void getAttacks(struct piece p, unsigned char* output);
void getMoves(struct piece p, unsigned char* output);

int makeMove(unsigned char* new, unsigned char* middle);

#endif /* PIECE_H_ */