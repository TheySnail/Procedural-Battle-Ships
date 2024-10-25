#pragma once
#ifndef _BATTLE_SHIP_H_
#define _BATTLE_SHIP_H_

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <Windows.h>

int Menu(void);
void playGame(bool _debugMap, bool _debugAmmo);
void instructions(void);
void debugMode(bool& _debugMap, bool& _debugAmmo);
void initaliseMap(char _map[][10], char _displaymap[][10]);
void initaliseShip(char _map[][10]);
void printMap(char _map[][10], char _displayMap[10][10], int _guesses, bool _debugAmmo, bool _debugMap);
bool hasHit(char _map[][10], char _displayMap[10][10], int _xGuess, int _yGuess, int& hitA, int& hitB, int& hitD, int& hitS, int& hitP);
bool gameWon(int& _A, int& _B, int& _D, int& _S, int& _P, int& _shipsDestroyed);

#endif // !_BATTLE_SHIP_H_