#ifndef MCTS_H
#define MCTS_H

#include "global.h"
#include "node.h"
#define AI_STATE 0
#define MAX_SIMULATION 150


void mcts(int chessboard[15][15],int mcts_count,int max_simulation,player mcts_player,int* i_result,int* j_result);
float UCB(Node* node);
Node* select_mcts(Node* node);
Node* best_ucb_child(Node* node);
Node* best_avg_value_child(Node* node);
void get_random_move(int chessboard_data[15][15],int* i_result, int* j_result);
Node* expand_random(Node* node);
void backup(Node* node,int value);
int simulation(Node* node,int max_count);

#endif