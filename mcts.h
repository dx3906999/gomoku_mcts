#ifndef MCTS_H
#define MCTS_H

#include "global.h"
#include "node.h"
#include "referee.h"
#define AI_STATE 0
#define MAX_SIMULATION 150


extern int visit_num_sum;
extern player_t global_mcts_player;
extern Node* mcts_root;
extern Node* mcts_last_choice;

void mcts(player_t chessboard[15][15],int mcts_count,int max_simulation,player_t mcts_player,int* i_result,int* j_result);
float UCB(Node* node);
Node* select_mcts(Node* node);
Node* best_ucb_child(Node* node);
Node* best_avg_value_child(Node* node);
Node* most_visited_child(Node* node);
void get_random_move(player_t chessboard_data[15][15],int* i_result, int* j_result);
Node* expand_random(Node* node);
void backup(Node* node,int value);
int simulation(Node* node,int max_count);

#endif