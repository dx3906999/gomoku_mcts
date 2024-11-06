#ifndef MCTS_PLAYER_H
#define MCTS_PLAYER_H

#include "global.h"
#include "mcts_zero.h"

typedef struct MctsPlayer MctsPlayer;

struct MctsPlayer
{
    MctsZero* mcts;
    bool is_training;
    player_t player;
};

MctsPlayer* new_mcts_player(player_t player,bool is_training,float c_puct,int n_playout);
action_t get_action(MctsPlayer* mcts_player,const State* state,float temperature,float porb_output[CHESSBOARD_SIZE]);
action_t action_random_choose_with_d(const float prob[CHESSBOARD_SIZE],bool d,float alpha);

#endif