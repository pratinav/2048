/**
 * Final Project
 * author: Pratinav Bagla
 *
 * main.cpp
 *
 * main.h and main.cpp build an interface and other features such as
 * a scoring system and a menu around the main board class
 */

#include "main.h"

const int NUM_MENU_COMMANDS = 6;
const string MENU_COMMANDS[NUM_MENU_COMMANDS] = { "play", "load", "scores", "quit", "q", "help" };
const int NUM_GAME_COMMANDS = 14;
const string GAME_COMMANDS[] = { "up", "u", "down", "d", "left", "l", "right", "r", "save", "s", "quit", "q", "reset", "help"};
vector<int> scores;
ofstream scoreFile;

int main()
{
  string input;
  bool flag;
  int index;
  loadScores();

  // open scoreFile in append mode for writing scores
  scoreFile.open("scores.txt", ofstream::app);
  if (scoreFile.fail())
    cout << "Could not open score file. Scores for this session will not be saved." << endl;

  displayMenu();
  while (true)
  {
    flag = false;
    do
    {
      if (flag)
        cout << "Invalid command!" << endl;
      else
        flag = true;

      cout << "\n> ";
      cin >> input;
      index = getIndex(input, MENU_COMMANDS, NUM_MENU_COMMANDS);
    } while (index == -1);

    switch (index)
    {
      case 0: // play
        play();
        break;

      case 1: // load
        loadGame();
        break;

      case 2: // scores
        displayScores();
        break;  

      case 3: // quit, q
      case 4:
        return 0;

      case 5: // help
        cout << endl;
        displayMenu();
        break;

      default: // empty default just in case
        ;
    }
  }

  return 0;
}

/**
 * plays with a new board
 */
void play()
{
  board *b = new board;

  if (!b)
  {
    cout << "Error allocating memory for board" << endl;
    return;
  }

  play(b);
}

/**
 * provides an interface to play with the passed in board
 */
void play(board *b)
{
  string input;
  bool flag;
  do
  {
    // display board and score
    cout << endl << *b;

    // get move from user
    int index;
    flag = false;
    do
    {
      if (flag)
        cout << "Invalid command!" << endl;
      else
        flag = true;

      cout << "> ";
      cin >> input;
      index = getIndex(input, GAME_COMMANDS, NUM_GAME_COMMANDS);
    } while (index == -1);

    flag = false;
    switch (index)
    {
      case 0: // up, u
      case 1:
        flag = b -> moveUp();
        break;

      case 2: // down, d
      case 3:
        flag = b -> moveDown();
        break;

      case 4: // left, l
      case 5:
        flag = b -> moveLeft();
        break;

      case 6: // right, r
      case 7:
        flag = b -> moveRight();
        break;

      case 8: // save, s
      case 9:
        saveGame(b);
        break;

      case 10: // quit, q
      case 11:
        // ask the user if they want to save the game before quitting
        cout << "Save game? (y/n): ";
        cin >> input;

        // if there was an error saving the game, confirm if the user
        // still wants to quit
        if (input == "y" && !saveGame(b))
        {
          cout << "Continue to quit? (y/n): ";
          cin >> input;

          if (input != "y")
            break;
        }
        return;

      case 12: // reset
        b -> reset();
        break;

      case 13: // help
        cout << endl;
        displayMenu();
        break;

      default:
        ; // empty default just in case
    }

    if (flag) // if any tiles were moved
      b -> populate(); // populate a new spot

  } while (!b -> isGameOver()); // exit once game is over

  // print out the final score and baord
  cout << endl << *b << "Game Over!" << endl;

  // tell the user if it was a new highscore
  if (scores.size() > 0 && scores.back() < b -> getScore())
    cout << "New highscore!" << endl;

  // add the score to the scores vector, also display the current highscore
  if (b -> getScore() > 0)
    pushNewScore(b -> getScore());
  cout << "Highscore: " << scores.back() << endl;

  delete b;
}

/**
 * load scores from scores.txt if it exists
 */
void loadScores()
{
  ifstream file("scores.txt");
  if (file.fail())
  {
    cout << "Could not load scores" << endl;
    return;
  }

  string line;
  while (getline(file, line))
    if (line != "") // in case a stray empty line is left out
      scores.push_back(stoi(line));

  bubbleSort(scores); // sort the scores vector
}

/**
 * Saves the current game
 */
bool saveGame(board *b)
{
  // get save file path from user
  string path;
  cin.ignore();
  cout << "Enter save file name/path (without extension): "; // a .txt extension will be added regardless
  getline(cin, path);
  path += ".txt";

  // if the save was successful, tell the user and return true
  if (b -> save(path))
  {
    cout << "The game was saved at " << path << endl;
    return true;
  }

  // else return false
  return false;
}

/**
 * loads pre-existing game
 */
void loadGame()
{
  // get save file path from user
  string path;
  cin.ignore();
  cout << "Enter save file path (with extension): ";
  getline(cin, path);

  // allocate a new board, loading it from the given path
  board *b = new board(path);
  if (!b)
  {
    cout << "Error allocating memory for board" << endl;
    return;
  }

  // if the board was loaded successfully, play it
  if (!b -> fail())
    play(b);
}

/**
 * bubble sorts the vector<int> passed to it by reference
 */
void bubbleSort(vector<int> &v)
{
  // if the vector has less than 2 ints, we don't need to sort it
  if (v.size() < 2)
    return;

  int *end = v.data() + v.size() - 1, temp;
  bool flag = false;

  do
  {
    flag = false;
    for (int *x = v.data(); x < end; x++)
    {
      // if x > (x + 1), swap them
      if (*x > *(x + 1))
      {
        flag = true;
        temp = *(x + 1);
        *(x + 1) = *x;
        *x = temp;
      }
    }
    end--; // keep deducting one from the end as we bubble the largest int up
  } while (flag); // if there are no swaps in the entire iteration, it must be sorted
}

/**
 * pushes new score to vector in the correct index
 */
void pushNewScore(int newScore)
{
  if (newScore <= 0)
    return;
  
  // write score to score file, if it's open
  if (scoreFile.is_open())
    scoreFile << newScore << endl;

  // if the scores vector is empty, or the new score being pushed is greater than
  // or equal to the last score in the vector, insert it at the end
  int lastIndex = scores.size() - 1;
  if (lastIndex == -1 || newScore >= scores.back())
  {
    scores.push_back(newScore);
    return;
  }

  // if newScore is smaller or equal to the smallest score, insert the newscore
  // before it
  if (newScore <= scores.front())
  {
    scores.insert(scores.begin(), newScore);
    return;
  }

  // else, find the right index to insert the new score into
  int index = 0;
  for ( ; index < lastIndex; index++)
  {
    if (newScore >= scores[index] && newScore < scores[index + 1])
      break;
  }

  // insert score at the index
  scores.insert(scores.begin() + index + 1, newScore);
}

/**
 * displays menu/help prompt
 */
void displayMenu()
{
  cout << "2048\n\n"
       << "Enter 'play' to start a new game\n"
       << "Enter 'load' to load a game\n"
       << "Enter 'scores' to view the score table\n"
       << "Enter 'quit' or 'q' to quit the program\n"
       << "Enter 'help' to print this again\n\n"
       << "How to play:\n"
       << "Enter 'up' or 'u' to move the tiles up\n"
       << "Enter 'down' or 'd' to move the tiles down\n"
       << "Enter 'left' or 'l' to move the tiles left\n"
       << "Enter 'right' or 'r' to move the tiles right\n\n"
       << "When two matching tiles touch, they merge!\n"
       << "Try to get the highest score possible\n\n"
       << "Enter 'save' or 's' to save the game\n"
       << "Enter 'quit' or 'q' to quit to the menu\n"
       << "Enter 'reset' to reset the game\n";
}

/**
 * displays score table
 */
void displayScores()
{
  int len = scores.size();
  if (len == 0) // if there are no scores in the vector, don't display any
  {
    cout << "No scores yet" << endl;
    return;
  }

  cout << endl << "Score table" << endl;
  // iterate through the vector of scores and print them out in descending order
  for (int *begin = scores.data(), *x = begin + len - 1, c = 1; x >= begin; x--, c++)
    cout << c << ". " << *x << endl;
}

/**
 * gets index of string s from array of possible moves
 */
int getIndex(string s, const string ARR[], const int ARR_LEN)
{
  for (int x = 0; x < ARR_LEN; x++)
  {
    if (s == ARR[x])
      return x;
  }

  return -1; // if s was not found in ARR, return -1
}