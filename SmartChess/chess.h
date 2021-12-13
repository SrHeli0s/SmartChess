#ifndef CHESS_H_
#define CHESS_H_

#define COLOR_W 1
#define COLOR_B 0

#define P_KING 1
#define P_ROOK 2
#define P_BISHOP 3
#define P_KNIGHT 4
#define P_PAWN 5
#define P_QUEEN 6

#define MOVE_N 12
#define MOVE_S -12
#define MOVE_E 1
#define MOVE_W -1

struct piece {
	unsigned char type;
	unsigned char color;
	unsigned char pos;
	unsigned char nMoves;
	unsigned char alive;
};

extern struct piece gamePieces[32];
extern struct piece *lastPiece;

extern unsigned char actual_position[144]; //Representation of the actual board
extern unsigned char detected_position[144]; //Buffer representation of the board with a new move
extern unsigned char middle_position[144]; //Representation of the "middle detected" board

extern unsigned char white_attacks[144]; //Possible attacks white can make
extern unsigned char black_attacks[144]; //Possible attacks black can make

extern unsigned char turn;

void prepareGame();
void updateAttackMap(int c);
void getAttacks(struct piece p, unsigned char* output);
void getMoves(struct piece p, unsigned char* output);

int makeMove(unsigned char* new, unsigned char* middle);

#endif /* CHESS_H_ */