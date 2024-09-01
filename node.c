#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "node.h"

Node* new_Node(Node* parent){
    Node* node=(Node*)malloc(sizeof(Node));
    node->parent=parent;
    node->value=0;
    node->children_num=0;
    node->visit_num=0;
    if (!parent)
    {
        parent->children_num++;
        parent->children[parent->children_num-1]=node;
    }
    
    return node;
}

void change_point(Node* node,int i,int j){
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
    }
    
}

