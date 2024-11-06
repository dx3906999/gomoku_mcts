#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "global.h"
#include "node_zero.h"

NodeZero* new_node_zero(NodeZero* parent,float prob,action_t action){
    NodeZero* node=malloc(sizeof(NodeZero));
    node->parent=parent;
    node->action=action;
    node->prob=prob;
    node->n_visited=0;
    // node->player=(parent==NULL)?(player):(OPS_PLAYER(parent->player));
    node->value=0;
    node->children_num=0;
    memset(node->children,0,sizeof(node->children));

    if (parent!=NULL)
    {
        parent->children_num++;
        parent->children_actions[parent->children_num-1]=action;
        parent->children[action]=node;
    }
    

}


void delete_node_zero(NodeZero* node){
    for (size_t i = 0; i < node->children_num; i++)
    {
        if (node->children[node->children_actions[i]]!=NULL)
        {
            delete_node_zero(node->children[node->children_actions[i]]);
        }
        
    }

    free(node);
    
}

/**
 * @brief 选择最好的孩子，但必须有孩子，否则返回NULL
 * 
 * @param node 
 * @param c_puct 
 * @return NodeZero* 
 */
NodeZero* best_uvalue_child(NodeZero* node, float c_puct){
    float best_uvalue=0;
    float temp_uvalue=0;
    action_t best_action=-1;
    for (size_t i = 0; i < node->children_num; i++)
    {
        temp_uvalue=get_uvalue(node->children[node->children_actions[i]],c_puct);
        if (best_uvalue<temp_uvalue)
        {
            best_uvalue=temp_uvalue;
            best_action=node->children_actions[i];
        }
        
    }
    
    return (node->children_num==0)?(NULL):node->children[best_action];
    
}


void expand(NodeZero* node,float prob[CHESSBOARD_SIZE],bool actions_unavailable[CHESSBOARD_SIZE]){
    for (action_t action = 0; action < CHESSBOARD_SIZE; action++)
    {
        if (actions_unavailable[action]==false&&node->children[action]==NULL)
        {
            new_node_zero(node,prob[action],action);
        }
        
    }
    
}


void backup(NodeZero* node,float final_value){
    while (node!=NULL)
    {
        node->value=(final_value+node->value*node->n_visited)/(node->n_visited+1);
        node->n_visited++;
        node=node->parent;
        final_value=-final_value;
    }
    
}


float get_uvalue(NodeZero* node,float c_puct){
    return node->value+c_puct*node->prob*sqrt(node->parent->n_visited)/(node->n_visited+1);
}
