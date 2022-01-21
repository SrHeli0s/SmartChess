#ifndef BOARDSHOWCASE_H_
#define BOARDSHOWCASE_H_

#define MOVE_N 12
#define MOVE_S -12
#define MOVE_E 1
#define MOVE_W -1

extern unsigned char actual_position[144]; //Representation of the actual board
extern unsigned char detected_position[144]; //Buffer representation of the board with a new move
extern unsigned char middle_position[144]; //Representation of the "middle detected" board
extern unsigned char possibleMoves[144];

extern unsigned char white_attacks[144]; //Possible attacks white can make
extern unsigned char black_attacks[144]; //Possible attacks black can make

extern unsigned char turn;
extern char output[20];

void prepareGame();

void translateSquare(int pos, int pos2);
void translateMove();

unsigned char compareBoards();
void copyBoards(unsigned char* from, unsigned char* to);

int makeMove();
	
#endif /* BOARDSHOWCASE_H_ */