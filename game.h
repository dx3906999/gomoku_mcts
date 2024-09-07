#ifndef GAME_H
#define GAME_H


#include "global.h"

extern int game_mode;
extern int i_current;
extern int j_current;
extern int i_black_last;
extern int j_black_last;
extern int i_white_last;
extern int j_white_last;
extern int current_player;
extern int global_chessboard_data[15][15];

// void print_menu();
void print_chessboard();
void print_title();
void update_chessboard_str();
bool get_move_input(int* i, int* j);
void update_chessboard_data_one_step(int chessboard_data[15][15], int i, int j, player player);
void update_global_chessboard_data_one_step();
bool chessboard_is_full(int chessboard[15][15]);
bool is_winner(int chessboard_data[15][15],player player,int i,int j);
bool is_five_in_a_row(int chessboard_data[15][15],player player,int i,int j);
void human_vs_human();
void human_vs_ai(player human_player);
void train();

#endif