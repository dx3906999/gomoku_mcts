#ifndef NODE_ZERO_H
#define NODE_ZERO_H

#include "global.h"

typedef struct NodeZero NodeZero;

struct NodeZero
{
    NodeZero* parent;
    float value;
    int n_visited;
    float prob;
    int children_num;//待补充
    action_t action;
    // player_t player;
    action_t children_actions[CHESSBOARD_SIZE];
    NodeZero* children[CHESSBOARD_SIZE];
};

NodeZero* new_node_zero(NodeZero* parent,float prob,action_t action);
void delete_node_zero(NodeZero* node);
NodeZero* best_uvalue_child(NodeZero* node, float c_puct);
void expand(NodeZero* node,float prob[CHESSBOARD_SIZE],bool actions_unavailable[CHESSBOARD_SIZE]);
float get_uvalue(NodeZero* node,float c_puct);
void backup(NodeZero* node,float final_value);

#endif 