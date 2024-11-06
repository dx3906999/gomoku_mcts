#ifndef NODE_H
#define NODE_H

#include "global.h"

#define MAX_CHILDREN 224


typedef struct Node Node;

struct Node{
    float value;
    int visit_num;
    int children_num;
    int children_num_max;
    int i;
    int j;
    player_t player;
    Node* parent;
    player_t chessboard_data[15][15];        // 实际上是父节点下完后的棋盘
    Node* children[MAX_CHILDREN];
};

Node* new_Node(Node* parent);
void change_node_point(Node* node,int i,int j);
void free_Node(Node* node);

#endif