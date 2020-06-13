
#include <string>
#include <curses.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

using namespace std;

enum {
	EMPTY = 0,
	PIECE_1, 
	PIECE_2,
	PIECE_3,
	PIECE_4,
	PIECE_5,
	PIECE_6,
	PIECE_7,
	BORDER

};

constexpr int piecesCodes[1][16] = {
	{       EMPTY, PIECE_1, EMPTY, EMPTY,
		PIECE_1, PIECE_1, PIECE_1, EMPTY, 
		EMPTY, EMPTY, EMPTY, EMPTY,
	       	EMPTY, EMPTY, EMPTY, EMPTY}};

constexpr int colors[9] = {
	' ' | COLOR_PAIR(EMPTY),
	' ' | COLOR_PAIR(PIECE_1),
	' ' | COLOR_PAIR(PIECE_2),
	' ' | COLOR_PAIR(PIECE_3),
	' ' | COLOR_PAIR(PIECE_4),
	' ' | COLOR_PAIR(PIECE_5),
	' ' | COLOR_PAIR(PIECE_6),
	' ' | COLOR_PAIR(PIECE_7),
	' ' | COLOR_PAIR(BORDER)};

constexpr int COLS_PER_CELL = 2;


class Piece {
	public:
		int x, y;
		int data[16];
		Piece();
		Piece(int piece);
		void reset() {x = 4; y = 0;}
		void MoveRight() {x++;}
		void MoveLeft() {x--;}
		void MoveUp() {y--;}
		void MoveDown() {y++;}
		void rotateLeft();
		void rotateRight();
};

Piece::Piece()
: x(4), y(0)
{
	memcpy(data, piecesCodes[0], 16 * sizeof(int));
}

Piece::Piece(int pieceNum)
:
x(1), y(1)
{
	memcpy(data, piecesCodes[pieceNum], 16 * sizeof(int));
}

void Piece::rotateLeft()
{
}

void Piece::rotateRight()
{
}

class Board {
	public:
		int mHeight, mWidth;
		int *data;
		WINDOW *mWin;
		Piece activePiece;

		Board(WINDOW *win, int height, int width);
		~Board();
		void refresh();
		void addNewPiece();
		void movePiece(int ch);
		void drawPiece();
		void deleteOldPosition();
		bool checkCollisionVertical();
		bool checkCollisionToLeft();
		bool checkCollisionToRight();
};

Board::Board(WINDOW *win, int height, int width)
	: mHeight(height), mWidth(width), mWin(win)
{
	data = new int[height * width];

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			data[i * width + j] = (i == height - 1 || j == 0 || j == width - 1) ? BORDER : EMPTY;
		}
	}
}

Board::~Board()
{
	delete [] data;
}

void Board::refresh()
{
	for (int i = 0; i < mHeight; i++) {
		for (int j = 0; j < mWidth; j++) {
			for (int k = 0; k < COLS_PER_CELL; k++) {
				mvwaddch(mWin, i, j * COLS_PER_CELL + k, colors[data[i * mWidth + j]]);
			}
		}
	}
	wrefresh(mWin);
}

void Board::addNewPiece()
{
	activePiece.reset();
	drawPiece();
}

void Board::drawPiece()
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (activePiece.data[i * 4 + j] != 0) {
				data[(activePiece.y + i) * mWidth + activePiece.x + j] =
					activePiece.data[i * 4 + j];
			}
		}
	}
}

void Board::deleteOldPosition()
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (activePiece.data[i * 4 + j] != 0)
				data[(activePiece.y + i) * mWidth + activePiece.x + j] = EMPTY;
		}
	}
}

bool Board::checkCollisionVertical()
{
	int maxY = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (activePiece.data[i * 4 + j] != 0)
				maxY = i;
		}
	}
	for (int j = 0; j < 4; j++) {
		if (activePiece.data[maxY * 4 + j] != 0 &&
			data[(activePiece.y + maxY + 1) * mWidth + activePiece.x + j] != EMPTY)
			return true;
	}
	return false;
}

bool Board::checkCollisionToLeft()
{
	int minX = 0;
	for (int j = 4; j < 0; j--) {
		for (int i = 0; i < 4; i++) {
			if (activePiece.data[i * 4 + j] != 0)
				minX = j;
		}
	}
	for (int i = 0; i < 4; i++) {
		if (activePiece.data[i * 4 + minX] != 0 &&
			data[(activePiece.y + i) * mWidth + activePiece.x + minX - 1] != EMPTY)
			return true;
	}
	return false;
}

bool Board::checkCollisionToRight()
{
	int maxX = 0;
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			if (activePiece.data[i * 4 + j] != 0)
				maxX = j;
		}
	}
	for (int i = 0; i < 4; i++) {
		if (activePiece.data[i * 4 + maxX] != 0 &&
			data[(activePiece.y + i) * mWidth + activePiece.x + maxX + 1] != EMPTY)
			return true;
	}
	return false;
}

void Board::movePiece(int ch)
{
	if (checkCollisionVertical()) {
		addNewPiece();
	} else {

		deleteOldPosition();

		switch (ch) {
			case KEY_UP:
				activePiece.MoveUp();
				break;
			case KEY_DOWN:
				activePiece.MoveDown();
				break;
			case KEY_LEFT:
				if (!checkCollisionToLeft()) {
					activePiece.MoveLeft();
				}
				break;
			case KEY_RIGHT:
				if (!checkCollisionToRight()) {
					activePiece.MoveRight();
				}
				break;
			case 'a':
				activePiece.rotateLeft();
				break;
			default:
				break;
		}

		drawPiece();
	}
}

int main()
{
	initscr(); // initialize curses
	noecho(); // no echo
	cbreak(); // one character at the time
	nodelay(stdscr, TRUE);
	keypad(stdscr, TRUE);  // allow arrow keys
	timeout(0);            // no blocking on getch()
	curs_set(0);           // set the cursor to invisible
	start_color();         // setup tetris colors

	int height = 21;
	int width = 12;
	int x0 = 10, y0 = 10;

	WINDOW *win = newwin(height, width * COLS_PER_CELL, y0, x0);

	start_color();
	init_pair(PIECE_1, COLOR_YELLOW, COLOR_GREEN);
	init_pair(PIECE_2, COLOR_YELLOW, COLOR_RED);
	init_pair(PIECE_3, COLOR_YELLOW, COLOR_CYAN);
	init_pair(PIECE_4, COLOR_YELLOW, COLOR_BLUE);
	init_pair(PIECE_5, COLOR_YELLOW, COLOR_WHITE);
	init_pair(PIECE_6, COLOR_YELLOW, COLOR_MAGENTA);
	init_pair(PIECE_7, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(BORDER, COLOR_YELLOW, COLOR_WHITE);
	init_pair(EMPTY, COLOR_YELLOW, COLOR_BLACK);

	Board board(win, height, width);
        board.addNewPiece();
	board.refresh();

	while(true) {
		int ch = getch();
		board.movePiece(ch);
		board.refresh();
		sleep(0.1);
	}

	endwin(); // restore terminal settings
	return 0;
}
