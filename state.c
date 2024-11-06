#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "state.h"

State* init_state(){
    State* state=malloc(sizeof(State));
    memset(state->chessboard,0,sizeof(state->chessboard));
    memset(state->actions_unavailable,0,sizeof(state->actions_unavailable));
    state->history_actions_num=0;
    state->current_player=BLACK;
}


void do_action(State* state,action_t action){
    state->chessboard[action/CHESSBOARD_LEN][action%CHESSBOARD_LEN]=state->current_player;
    state->actions_unavailable[action]=true;
    state->history_actions_num++;
    state->history_actions[state->history_actions_num-1]=action;
    state->current_player=OPS_PLAYER(state->current_player);
}


