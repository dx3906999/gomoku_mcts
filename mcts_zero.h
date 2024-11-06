#ifndef MCTS_ZERO_H
#define MCTS_ZERO_H

#include "global.h"
#include "state.h"
#include "node_zero.h"

typedef struct MctsZero MctsZero;

struct MctsZero
{
    NodeZero* root;
    float c_puct;
    int n_playout;
    // player_t mcts_player;
};

MctsZero* init_mcts_zero(float c_puct,int n_playout);
void policy_value_analyzer(State* state,float prob_output[CHESSBOARD_SIZE],float* value);
void playout(MctsZero* mcts,State* state);
void update_root(MctsZero* mcts,action_t last_action);
void get_actions_prob_log(MctsZero* mcts,const State* state,float temperature,float porb_output[CHESSBOARD_SIZE]);
void get_actions_prob(MctsZero* mcts,const State* state,float temperature,float porb_output[CHESSBOARD_SIZE]);
void log_softmax_mcts(float x[],int n);
void softmax_mcts(float x[],int n);

#endif 