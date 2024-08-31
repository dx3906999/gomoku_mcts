#ifndef GAME_H
#define GAME_H


#include "global.h"

void print_menu();
void print_chessboard();
void print_title();
void update_chessboard_str();
bool get_move_input(int* i, int* j);
void update_chessboard_data_one_step();

#endif