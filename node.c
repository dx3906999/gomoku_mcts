#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "node.h"
#include "game.h"

Node* new_Node(Node* parent){
    Node* node=(Node*)malloc(sizeof(Node));
    node->parent=parent;
    node->value=0;
    node->children_num=0;
    node->visit_num=0;

    if (parent)
    {
        parent->children_num++;
        parent->children[parent->children_num-1]=node;
        node->children_num_max=parent->children_num_max-1;
        memcpy(node->chessboard_data,parent->chessboard_data,sizeof(node->chessboard_data));
        update_chessboard_data_one_step(node->chessboard_data,parent->i,parent->j,parent->player);
        node->player=0-parent->player;
    }
    else
    {
        node->children_num_max=MAX_CHILDREN;
        memcpy(node->chessboard_data,global_chessboard_data,sizeof(node->chessboard_data));
    }
    
    return node;
}

void change_node_point(Node* node,int i,int j){
    node->i=i;
    node->j=j;
}

void free_Node(Node* node){
    if (node->children_num>0)
    {
        for (size_t i = 0; i < node->children_num; i++)
        {
            free_Node(node->children[i]);
        }
        
    }
    else
    {
        free(node);
        return;
    }
    
}

