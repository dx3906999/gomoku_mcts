#ifndef NODE_H
#define NODE_H

#include "global.h"

#define MAX_CHILDREN 225


typedef struct Node Node;

struct Node{
    float value;
    int visit_num;
    int children_num;
    int i;
    int j;
    player player;
    Node* parent;
    Node* children[MAX_CHILDREN];
};

Node* new_Node(Node* parent);
void change_node_point(Node* node,int i,int j);
void free_Node(Node* node);

#endif