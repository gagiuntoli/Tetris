
#include <string>
#include <cstdlib>
#include <curses.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

using namespace std;

enum {
	EMPTY = 0,
	PIEC1, 
	PIEC2,
	PIEC3,
	PIEC4,
	PIEC5,
	PIEC6,
	PIEC7,
	BORDER

};

// [piece ID][Rotation
constexpr int piecesCodes[7][4][16] = {
	{
{EMPTY, PIEC1, EMPTY, EMPTY, PIEC1, PIEC1, PIEC1, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
{EMPTY, PIEC1, EMPTY, EMPTY, EMPTY, PIEC1, PIEC1, EMPTY, EMPTY, PIEC1, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
{EMPTY, EMPTY, EMPTY, EMPTY, PIEC1, PIEC1, PIEC1, EMPTY, EMPTY, PIEC1, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
{EMPTY, PIEC1, EMPTY, EMPTY, PIEC1, PIEC1, EMPTY, EMPTY, EMPTY, PIEC1, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
	},
	{
{EMPTY, PIEC2, EMPTY, EMPTY, EMPTY, PIEC2, EMPTY, EMPTY, PIEC2, PIEC2, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
{PIEC2, EMPTY, EMPTY, EMPTY, PIEC2, PIEC2, PIEC2, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
{EMPTY, PIEC2, PIEC2, EMPTY, EMPTY, PIEC2, EMPTY, EMPTY, EMPTY, PIEC2, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
{EMPTY, EMPTY, EMPTY, EMPTY, PIEC2, PIEC2, PIEC2, EMPTY, EMPTY, EMPTY, PIEC2, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
	},
	{
{EMPTY, PIEC3, EMPTY, EMPTY, EMPTY, PIEC3, EMPTY, EMPTY, EMPTY, PIEC3, PIEC3, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
{EMPTY, EMPTY, EMPTY, EMPTY, PIEC3, PIEC3, PIEC3, EMPTY, PIEC3, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
{PIEC3, PIEC3, EMPTY, EMPTY, EMPTY, PIEC3, EMPTY, EMPTY, EMPTY, PIEC3, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
{EMPTY, EMPTY, PIEC3, EMPTY, PIEC3, PIEC3, PIEC3, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
	},
	{
{EMPTY, PIEC4, EMPTY, EMPTY, EMPTY, PIEC4, EMPTY, EMPTY, EMPTY, PIEC4, EMPTY, EMPTY, EMPTY, PIEC4, EMPTY, EMPTY},
{EMPTY, EMPTY, EMPTY, EMPTY, PIEC4, PIEC4, PIEC4, PIEC4, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
{EMPTY, PIEC4, EMPTY, EMPTY, EMPTY, PIEC4, EMPTY, EMPTY, EMPTY, PIEC4, EMPTY, EMPTY, EMPTY, PIEC4, EMPTY, EMPTY},
{EMPTY, EMPTY, EMPTY, EMPTY, PIEC4, PIEC4, PIEC4, PIEC4, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
	},
	{
{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, PIEC5, PIEC5, EMPTY, PIEC5, PIEC5, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
{PIEC5, EMPTY, EMPTY, EMPTY, PIEC5, PIEC5, EMPTY, EMPTY, EMPTY, PIEC5, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, PIEC5, PIEC5, EMPTY, PIEC5, PIEC5, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
{PIEC5, EMPTY, EMPTY, EMPTY, PIEC5, PIEC5, EMPTY, EMPTY, EMPTY, PIEC5, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
	},
	{
{PIEC6, PIEC6, EMPTY, EMPTY, EMPTY, PIEC6, PIEC6, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
{EMPTY, EMPTY, PIEC6, EMPTY, EMPTY, PIEC6, PIEC6, EMPTY, EMPTY, PIEC6, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
{PIEC6, PIEC6, EMPTY, EMPTY, EMPTY, PIEC6, PIEC6, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
{EMPTY, EMPTY, PIEC6, EMPTY, EMPTY, PIEC6, PIEC6, EMPTY, EMPTY, PIEC6, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
	},
	{
{PIEC7, PIEC7, EMPTY, EMPTY, PIEC7, PIEC7, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
{PIEC7, PIEC7, EMPTY, EMPTY, PIEC7, PIEC7, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
{PIEC7, PIEC7, EMPTY, EMPTY, PIEC7, PIEC7, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
{PIEC7, PIEC7, EMPTY, EMPTY, PIEC7, PIEC7, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
	}
};

constexpr int colors[9] = {
	' ' | COLOR_PAIR(EMPTY),
	' ' | COLOR_PAIR(PIEC1),
	' ' | COLOR_PAIR(PIEC2),
	' ' | COLOR_PAIR(PIEC3),
	' ' | COLOR_PAIR(PIEC4),
	' ' | COLOR_PAIR(PIEC5),
	' ' | COLOR_PAIR(PIEC6),
	' ' | COLOR_PAIR(PIEC7),
	' ' | COLOR_PAIR(BORDER)};

constexpr int COLS_PER_CELL = 2;


class Piece {
	public:
		int x, y;
		int data[16];
		int mRotation;
		int mPiece_ID;
		Piece();
		Piece(int piece);
		void reset();
		void MoveRight() {x++;}
		void MoveLeft() {x--;}
		void MoveUp() {y--;}
		void MoveDown() {y++;}
		void rotateLeft();
		void rotateRight();
};

Piece::Piece()
: x(4), y(0), mPiece_ID(5), mRotation(0)
{
	memcpy(data, piecesCodes[0][mRotation], 16 * sizeof(int));
}

Piece::Piece(int piece_ID)
:
x(1), y(1), mPiece_ID(piece_ID), mRotation(0)
{
	memcpy(data, piecesCodes[mPiece_ID][mRotation], 16 * sizeof(int));
}

void Piece::reset()
{
	x = 4; y = 0;
	mPiece_ID = rand() % 7;
	memcpy(data, piecesCodes[mPiece_ID][mRotation], 16 * sizeof(int));
}

void Piece::rotateLeft()
{
	mRotation = (--mRotation < 0 ? 3 : mRotation);
	memcpy(data, piecesCodes[mPiece_ID][mRotation], 16 * sizeof(int));
}

void Piece::rotateRight()
{
	mRotation = (++mRotation) % 4;
	memcpy(data, piecesCodes[mPiece_ID][mRotation], 16 * sizeof(int));
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
		bool lastRowCompleted();
		void deleteRow();
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

bool Board::lastRowCompleted()
{
	for (int j = 1; j < mWidth - 1; j++) {
		if(data[(mHeight - 2) * mWidth + j] == EMPTY)
			return false;
	}
	return true;
}

void Board::deleteRow()
{
	for (int i = mHeight - 2; i > 0; i--) {
		for (int j = 1; j < mWidth - 1; j++) {
			data[i * mWidth + j] = data[(i - 1) * mWidth + j];
		}
	}
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
		while(lastRowCompleted())
			deleteRow();
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
			case 's':
				activePiece.rotateRight();
				break;
			default:
				break;
		}

		drawPiece();
	}
}

int main()
{
	initscr();             // initialize curses
	noecho();              // no echo
	cbreak();              // one character at the time
	nodelay(stdscr, TRUE);
	keypad(stdscr, TRUE);  // allow arrow keys
	timeout(0);            // no blocking on getch()
	curs_set(0);           // set the cursor to invisible
	start_color();         // setup tetris colors

	int height = 25;
	int width = 12;
	int x0 = 10, y0 = 10;

	WINDOW *win = newwin(height, width * COLS_PER_CELL, y0, x0);

	start_color();
	init_pair(EMPTY, COLOR_YELLOW, COLOR_BLACK);
	init_pair(PIEC1, COLOR_YELLOW, COLOR_GREEN);
	init_pair(PIEC2, COLOR_YELLOW, COLOR_RED);
	init_pair(PIEC3, COLOR_YELLOW, COLOR_CYAN);
	init_pair(PIEC4, COLOR_YELLOW, COLOR_BLUE);
	init_pair(PIEC5, COLOR_YELLOW, COLOR_WHITE);
	init_pair(PIEC6, COLOR_YELLOW, COLOR_MAGENTA);
	init_pair(PIEC7, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(BORDER, COLOR_YELLOW, COLOR_WHITE);

	Board board(win, height, width);
        board.addNewPiece();
	board.refresh();

	while(true) {
		int ch = getch();
		board.movePiece(ch);
		board.refresh();
		sleep(0.1);
	}

	endwin();               // restore terminal settings
	return 0;
}
