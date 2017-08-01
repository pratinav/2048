/**
 * Final Project
 * author: Pratinav Bagla
 *
 * main.h
 *
 * main.h and main.cpp build an interface and other features such as
 * a scoring system and a menu around the main board class
 */

#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "board.h"
using namespace std;

void displayMenu();
void displayScores();
void play();
void play(board *b);
void loadScores();
bool saveGame(board *b);
void loadGame();
void bubbleSort(vector<int> &v);
int getIndex(string s, const string ARR[], const int ARR_LEN);
void pushNewScore(int newScore);