#ifndef REFEREE_H
#define REFEREE_H


// #define REFEREE_DEBUG

#include "global.h"
#include "game.h"
#include <stdint.h>

#define OVERLINE_S 1
#define FIVE_S (1<<(3*1))
#define DEAD_S (1<<(3*2))
#define FOUR_HALF_S (1<<(3*3))
#define FOUR_OPEN_S (1<<(3*4))
#define THREE_HALF_S (1<<(3*5))
#define THREE_OPEN_S (1<<(3*6))
#define TRUE_S (1<<(sizeof(chess_shape_t)*8-1))
#define FALSE_S 0
#define GET_SHAPE_S(chess_shape_s,std_shape_s) (((chess_shape_s)&((std_shape_s)|((std_shape_s)<<1)|((std_shape_s)<<2)))/(std_shape_s))
#define GET_TF_S(chess_shape_s) ((chess_shape_s)>>(sizeof(chess_shape_t)*8-1))


#define IS_IN_CHESSBOARD(i,j,k,direction) ((i)+i_direction[(direction)]*(k)>=0 && (i)+i_direction[(direction)]*(k)<15 && (j)+j_direction[(direction)]*(k)>=0 && (j)+j_direction[(direction)]*(k)<15)
#define THE_CHESS(chessboard,i,j,k,direction) ((chessboard)[(i)+i_direction[(direction)]*(k)][(j)+j_direction[(direction)]*(k)])

// typedef struct chess_shape_t
// {
//     int overline;
//     int five;
//     int four_half;
//     int four_open;
//     int three_half;
//     int three_open;
// }chess_shape_t;
 
typedef uint32_t chess_shape_t;

chess_shape_t is_banned(int chessboard[15][15],int i,int j,int h_direction_last);
void scan_chess_state(int chessboard[15][15],int i,int j,int chess_state[8][15],player player);
void analyze_chess_state(int chess_state[8][15],chess_shape_t chess_shape_state[4],int chessboard[15][15],int i,int j,player player);
bool is_five(int chessboard[15][15],player player,int i,int j);
bool chessboard_is_full(int chessboard_data[15][15]);
int is_winner(int chessboard_data[15][15],player player,int i,int j);

#ifdef REFEREE_DEBUG
void referee_debug(int chessboard[15][15],int i,int j,int chess_state[8][15]);
#endif


#endif