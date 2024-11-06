#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdint.h>
#define BLACK 1
#define WHITE -1
#define EMPTY 0
#define true 1
#define false 0
#define SIG_SAVE 114514
#define CHESSBOARD_LEN 15
#define CHESSBOARD_SIZE 225
#define MIN(x,y) (((x)>(y))?(y):(x))
#define MAX(x,y) (((x)>(y))?(x):(y))
#define OPS_PLAYER(player) (-(player))
// typedef int player;
typedef char player_t;
typedef int bool;
typedef uint8_t action_t;
/**game_mode
 * 0: pvp
 * 1: pve
 * 2: eve
 */



#endif