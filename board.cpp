/**
 * Final Project
 * author: Pratinav Bagla
 *
 * board.cpp
 *
 * board.h and board.cpp define the board class
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include "board.h"
using namespace std;

/**
 * default constructor
 */
board::board()
{
  srand(time(0)); // seed generator
  reset(); // acts like an initializer
}

/**
 * constructor to load game from file directly
 */
board::board(string path)
{
  _fail = !load(path);
}

/**
 * to check if loading a game via the constructor failed or not
 */
bool board::fail()
{
  return _fail;
}

/**
 * copy constructor
 */
board::board(const board &b)
{
  score = b.score;
  for (int x = 0; x < NUM_COLS; x++)
  {
    for (int y = 0; y < NUM_ROWS; y++)
      grid[y][x] = b.grid[y][x];

    colWidth[x] = b.colWidth[x];
  }
  _fail = b._fail;
}

/**
 * resets/initializes board
 */
void board::reset()
{
  score = 0;
  _fail = false;

  // initialize all grid elements to 0
  for (int y = 0; y < NUM_ROWS; y++)
  {
    for (int x = 0; x < NUM_COLS; x++)
      grid[y][x] = 0;
  }

  // initialize all column widths to 1
  for (int x = 0; x < NUM_COLS; x++)
    colWidth[x] = 1;

  // initialize the board by populating two spots on the grid
  for (int c = 0; c < 2; c++)
    populate();
}

/**
 * saves game from file path
 */
bool board::save(string path)
{
  // open file at path
  ofstream saveFile;
  try
  {
    saveFile.open(path);
  }
  catch (...)
  {
    cout << "Error opening save file" << endl;
    return false;
  }

  // if we can't open the file, tell the user and exit
  if (saveFile.fail())
  {
    cout << "Error opening save file" << endl;
    return false;
  }

  saveFile << score << '\n';
  for (int y = 0; y < NUM_ROWS; y++)
  {
    for (int x = 0; x < NUM_COLS; x++)
    {
      saveFile << grid[y][x];
      if (x != (NUM_COLS - 1) || y != (NUM_ROWS - 1))
        saveFile << ',';
    }
  }

  saveFile.close();
  return true;
}

/**
 * loads game from file path
 */
bool board::load(string path)
{
  // open file at path
  ifstream saveFile;
  try
  {
    saveFile.open(path);
  }
  catch (...)
  {
    cout << "Error opening save file" << endl;
    return false;
  }

  // if we can't open the file, tell the user and exit
  if (saveFile.fail())
  {
    cout << "Error opening save file" << endl;
    return false;
  }

  string s;
  getline(saveFile, s);
  score = stoi(s);
  int max;

  for (int y = 0; y < NUM_ROWS; y++)
  {
    for (int x = 0; x < NUM_COLS; x++)
    {
      getline(saveFile, s, ',');
      grid[y][x] = stoi(s);

      // keep track of the largest num in each column so we can update
      // column widths later
      if (y == 0 || grid[y][x] > colWidth[x])
        colWidth[x] = grid[y][x];

      // when we are at the end of each column, update its column width
      if (y == (NUM_ROWS - 1))
        colWidth[x] = getNumDigits(colWidth[x]);
    }
  }

  saveFile.close();
  return true;
}

/**
 * populates a random spot on the grid with either a 2 or a 4
 */
void board::populate()
{
  // randomly generate two random ints between 0 and 3
  int y, x;
  do
  {
    y = rand() % NUM_ROWS;
    x = rand() % NUM_COLS;
  } while (grid[y][x]); // regenerate incase we land a position already filled

  grid[y][x] = (rand() % 2 + 1) * 2; // set the tile value to either 2 or 4 (selected randomly)
}

/**
 * Prints out board (assuming all column widths are correct)
 * Output should look like:
 * +----------------------+
 * |   2 |    |   8 | 256 |
 * |   8 |  4 | 128 |  16 |
 * |     | 32 |   8 |     |
 * | 512 |    |  64 |   2 |
 * +----------------------+
 */
void board::display(ostream &out)
{
  printBorder();

  for (int y = 0; y < NUM_ROWS; y++)
  {
    out << '|';

    for (int x = 0; x < NUM_COLS; x++)
    {
      // print one space and set column width based on the largest number in that column
      out << ' ' << setw(colWidth[x]);

      // if the current tile is empty, print a space
      // else, print the value of the tile
      if (grid[y][x])
        out << grid[y][x];
      else
        out << ' ';

      out << " |";
    }

    out << endl;
  }

  printBorder();
}

/**
 * Helper function for board::display()
 * prints top/bottom border (assuming all column widths are correct)
 */
void board::printBorder()
{
  cout << '+';
  for (int x = 0; x < NUM_COLS; x++)
  {
    // print two extra dashes to account for spaces
    for (int i = -2; i < colWidth[x]; i++)
      cout << '-';

    if (x != 0) // print (NUM_COLS - 1) dashes to account for pipes
      cout << '-';
  }
  cout << '+' << endl;
}

/**
 * overloads the << operator to print out score and board
 */
ostream & operator << (ostream &out, board &b)
{
  out << "Score: " << b.getScore() << endl;
  b.display(out);
  return out;
}

/**
 * returns score
 */
int board::getScore()
{
  return score;
}

/**
 * updates the column index of the specified column
 */
void board::updateColWidth(int colIndex)
{
  // store the row index of the largest number in that column
  int max = 0; // set max to 0, and begin loop from 1
  for (int y = 1; y < NUM_ROWS; y++)
  {
    if (grid[y][colIndex] > grid[max][colIndex])
      max = y;
  }
  int width = getNumDigits(grid[max][colIndex]);
  if (colWidth[colIndex] != width)
    colWidth[colIndex] = width;
}

/**
 * checks if num exceeds the column width of colIndex, and updates
 * its column width accordingly
 */
void board::updateColWidth(int colIndex, int num)
{
  int numDigits = getNumDigits(num);
  if (numDigits > colWidth[colIndex])
    colWidth[colIndex] = numDigits;
}

/**
 * Used to move tile y to tile x
 */
void board::moveTile(int &x, int &y, int xColIndex, int yColIndex)
{
  // if next tile is empty, switch values
  if (x == 0)
  {
    x = y;
  }
  else // else, merge with the tile
  {
    x += y;
    score += x; // increase the score by x number of points
  }
  y = 0;

  // update x's column's width
  updateColWidth(xColIndex, x);

  // if y is in a different column, update y's column width as well
  if (yColIndex != -1 && xColIndex != yColIndex)
    updateColWidth(yColIndex);
}

/**
 * Moves grid elements up
 * returns true if any tiles were moved, else returns false
 */
bool board::moveUp()
{
  // stores if any tile has moved yet
  bool moved = false;
  for (int x = 0; x < NUM_COLS; x++)
  {
    for (int y = 1; y < NUM_ROWS; y++) // start from 1, because you can't move the tile at 0 upwards
    {
      if (grid[y][x] == 0) continue; // if tile is empty, don't do anything

      int nextY = y; // calculate the last empty non-edge tile in that direction
      while (nextY > 1 && grid[nextY - 1][x] == 0)
        nextY--;

      // if the next tile in that direction matches the current tile, merge them
      if (grid[nextY - 1][x] == grid[y][x])
      {
        moveTile(grid[nextY - 1][x], grid[y][x], x);

        // in case there is a merge, there can be a possibility of further merging
        // so set index to one less than the merged tile's index, so we are at
        // the merged tile's index in the next iteration
        if (nextY > 1)
          y = nextY - 2;

        if (!moved)
          moved = true;
      }
      // if the next tile in that direction is empty, move to it
      else if (grid[nextY - 1][x] == 0)
      {
        moveTile(grid[nextY - 1][x], grid[y][x], x);

        if (!moved)
          moved = true;
      }
      else if (y != nextY) // else, move to the farthest empty tile possible
      {
        moveTile(grid[nextY][x], grid[y][x], x);
        if (!moved)
          moved = true;
      }
    }
  }
  return moved;
}

/**
 * Moves grid elements down
 * returns true if any tiles were moved, else returns false
 */
bool board::moveDown()
{
  // stores if any tile has moved yet
  bool moved = false;
  for (int x = 0; x < NUM_COLS; x++)
  {
    for (int y = NUM_ROWS - 2; y >= 0; y--) // start from 2, because you can't move the tile at 3 downwards
    {
      if (grid[y][x] == 0) continue; // if tile is empty, don't do anything

      int nextY = y; // calculate the last empty non-edge tile in that direction
      while (nextY < (NUM_ROWS - 2) && grid[nextY + 1][x] == 0)
        nextY++;

      // if the next tile in that direction matches the current tile, merge them
      if (grid[nextY + 1][x] == grid[y][x])
      {
        moveTile(grid[nextY + 1][x], grid[y][x], x);

        // in case there is a merge, there can be a possibility of further merging
        // so set index to one less than the merged tile's index, so we are at
        // the merged tile's index in the next iteration
        if (nextY < (NUM_ROWS - 2))
          y = nextY + 2;

        if (!moved)
          moved = true;
      }
      // if the next tile in that direction is empty, move to it
      else if (grid[nextY + 1][x] == 0)
      {
        moveTile(grid[nextY + 1][x], grid[y][x], x);

        if (!moved)
          moved = true;
      }
      else if (y != nextY) // else, move to the farthest empty tile possible
      {
        moveTile(grid[nextY][x], grid[y][x], x);
        if (!moved)
          moved = true;
      }
    }
  }
  return moved;
}

/**
 * Moves grid elements left
 * returns true if any tiles were moved, else returns false
 */
bool board::moveLeft()
{
  // stores if any tile has moved yet
  bool moved = false;
  for (int y = 0; y < NUM_ROWS; y++)
  {
    for (int x = 1; x < NUM_COLS; x++) // start from 1, because you can't move the tile at 0 leftwards
    {
      if (grid[y][x] == 0) continue; // if tile is empty, don't do anything

      int nextX = x; // calculate the last empty non-edge tile in that direction
      while (nextX > 1 && grid[y][nextX - 1] == 0)
        nextX--;

      // if the next tile in that direction matches the current tile, merge them
      if (grid[y][nextX - 1] == grid[y][x])
      {
        moveTile(grid[y][nextX - 1], grid[y][x], nextX - 1, x);

        // in case there is a merge, there can be a possibility of further merging
        // so set index to one less than the merged tile's index, so we are at
        // the merged tile's index in the next iteration
        if (nextX > 1)
          x = nextX - 2;

        if (!moved)
          moved = true;
      }
      // if the next tile in that direction is empty, merge them
      else if (grid[y][nextX - 1] == 0)
      {
        moveTile(grid[y][nextX - 1], grid[y][x], nextX - 1, x);

        if (!moved)
          moved = true;
      }
      else if (x != nextX) // else, move to the farthest empty tile possible
      {
        moveTile(grid[y][nextX], grid[y][x], nextX, x);
        if (!moved)
          moved = true;
      }
    }
  }
  return moved;
}

/**
 * Moves grid elements right
 * returns true if any tiles were moved, else returns false
 */
bool board::moveRight()
{
  // stores if any tile has moved yet
  bool moved = false;
  for (int y = 0; y < NUM_ROWS; y++)
  {
    for (int x = NUM_COLS - 2; x >= 0; x--) // start from 2, because you can't move the tile at 3 rightwards
    {
      if (grid[y][x] == 0) continue; // if tile is empty, don't do anything

      int nextX = x; // calculate the last empty non-edge tile in that direction
      while (nextX < (NUM_COLS - 2) && grid[y][nextX + 1] == 0)
        nextX++;

      // if the next tile in that direction matches the current tile, merge them
      if (grid[y][nextX + 1] == grid[y][x])
      {
        moveTile(grid[y][nextX + 1], grid[y][x], nextX + 1, x);

        // in case there is a merge, there can be a possibility of further merging
        // so set index to one less than the merged tile's index, so we are at
        // the merged tile's index in the next iteration
        if (nextX < NUM_COLS - 2)
          x = nextX + 2;

        if (!moved)
          moved = true;
      }
      // if the next tile in that direction is empty, move to it
      else if (grid[y][nextX + 1] == 0)
      {
        moveTile(grid[y][nextX + 1], grid[y][x], nextX + 1, x);

        if (!moved)
          moved = true;
      }
      else if (x != nextX) // else, move to the farthest empty tile possible
      {
        moveTile(grid[y][nextX], grid[y][x], nextX, x);
        if (!moved)
          moved = true;
      }
    }
  }
  return moved;
}

/**
 * returns if game is over or not
 */
bool board::isGameOver()
{
  for (int y = 0; y < NUM_ROWS; y++)
  {
    for (int x = 0; x < NUM_COLS; x++)
    {
      // if there's an empty spot on the grid, or there's a pair of matching
      // tiles adjacent to each other, return false
      if ((grid[y][x] == 0) ||
          (y > 0 && grid[y - 1][x] == grid[y][x]) ||
          (y < (NUM_ROWS - 1) && grid[y + 1][x] == grid[y][x]) ||
          (x > 0 && grid[y][x - 1] == grid[y][x]) ||
          (x < (NUM_COLS - 1) && grid[y][x + 1] == grid[y][x]))
        return false;
    }
  }
  return true;
}

/**
 * Helper function
 * returns the number of digits of an int
 */
int getNumDigits(int x)
{
  // return 1 if the number is zero
  if (x == 0)
    return 1;

  int c = 0;
  while (x > 0)
  {
    c++;
    x /= 10; // truncate number by one place
  }
  return c;
}