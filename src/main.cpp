
#include <string>
#include <curses.h>
#include <unistd.h>
#include <limits.h>

using namespace std;

const string piecesCodes[] = {
	"A   A   A   A   ",
	" A  AAA         "};

class Piece {
	public:
		int x, y;
		string data;
		Piece();
		Piece(string piece);
		void reset() {x = 4; y = 0;}
		void MoveRight() {x++;}
		void MoveLeft() {x--;}
		void MoveUp() {y--;}
		void MoveDown() {y++;}
};

Piece::Piece()
: x(4), y(0), data(piecesCodes[1])
{
}

Piece::Piece(string piece)
: data(piece),
x(1), y(1)
{
}

class Board {
	public:
		int mHeight, mWidth;
		char *data;
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
		//void checkCollision();
};

Board::Board(WINDOW *win, int height, int width)
	: mHeight(height), mWidth(width), mWin(win)
{
	data = new char[height * width];

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			data[i * width + j] = (i == height - 1 || j == 0 || j == width - 1) ? '#' : ' ';
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
			mvwaddch(mWin, i, j, data[i * mWidth + j]);
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
			if (activePiece.data[i * 4 + j] != ' ') {
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
			if (activePiece.data[i * 4 + j] != ' ')
				data[(activePiece.y + i) * mWidth + activePiece.x + j] = ' ';
		}
	}
}

bool Board::checkCollisionVertical()
{
	int maxY = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (activePiece.data[i * 4 + j] != ' ')
				maxY = i;
		}
	}
	for (int j = 0; j < 4; j++) {
		if (activePiece.data[maxY * 4 + j] != ' ' &&
			data[(activePiece.y + maxY + 1) * mWidth + activePiece.x + j] != ' ')
			return true;
	}
	return false;
}

bool Board::checkCollisionToLeft()
{
	int minX = 0;
	for (int j = 4; j < 0; j--) {
		for (int i = 0; i < 4; i++) {
			if (activePiece.data[i * 4 + j] != ' ')
				minX = j;
		}
	}
	for (int i = 0; i < 4; i++) {
		if (activePiece.data[i * 4 + minX] != ' ' &&
			data[(activePiece.y + i) * mWidth + activePiece.x + minX - 1] != ' ')
			return true;
	}
	return false;
}

bool Board::checkCollisionToRight()
{
	int maxX = 0;
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			if (activePiece.data[i * 4 + j] != ' ')
				maxX = j;
		}
	}
	for (int i = 0; i < 4; i++) {
		if (activePiece.data[i * 4 + maxX] != ' ' &&
			data[(activePiece.y + i) * mWidth + activePiece.x + maxX + 1] != ' ')
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

	int height = 25;
	int width = 30;
	int x0 = 10, y0 = 10;

	WINDOW *win = newwin(height, width, y0, x0);

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
