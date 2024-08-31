#ifndef NODE_H
#define NODE_H

#define MAX_CHILDREN 225


typedef struct Node Node;

struct Node{
    float value;
    int visit_num;
    int children_num;
    Node* parent;
    Node* children[MAX_CHILDREN];
};

Node* new_Node(Node* parent);
void free_Node(Node* node);

#endif