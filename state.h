#ifndef STATE_H
#define STATE_H

#include "global.h"

typedef struct State State;

struct State
{
    player_t chessboard[CHESSBOARD_LEN][CHESSBOARD_LEN];
    player_t current_player;
    action_t history_actions[CHESSBOARD_SIZE];
    int history_actions_num;
    bool actions_unavailable[CHESSBOARD_SIZE];
};

State* init_state();
void do_action(State* state,action_t action);


#endif