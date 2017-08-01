/**
 * CSCI-S38 Final Project
 * author: Pratinav Bagla
 *
 * board.h
 *
 * board.h and board.cpp define the board class
 */

using namespace std;

const int NUM_ROWS = 4, NUM_COLS = 4;

class board
{
  int score;
  int grid[NUM_ROWS][NUM_COLS];
  int colWidth[NUM_COLS];
  bool _fail = false;

  void printBorder();
  void moveTile(int &x, int &y, int xColIndex, int yColIndex = -1);
  void updateColWidth(int colIndex);
  void updateColWidth(int colIndex, int num);

public:
  board();
  board(string path);
  board(const board &b);
  bool save(string path);
  bool load(string path);
  void reset();
  void populate();
  void display(ostream &out = cout);
  int getScore();
  bool moveUp();
  bool moveDown();
  bool moveLeft();
  bool moveRight();
  bool isGameOver();
  bool fail();
};

ostream & operator << (ostream &out, board &b);
int getNumDigits(int x);