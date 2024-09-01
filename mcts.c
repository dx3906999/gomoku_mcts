#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#include "mcts.h"
#include "node.h"

float UCB_C=1.0;
int visit_num_sum=0;

float UCB(Node* node){
    return (node->visit_num!=0)?(node->value/node->visit_num+UCB_C*sqrt((2*log(visit_num_sum))/node->visit_num)):__FLT32_NORM_MAX__;
}


/**
 * @brief 选择UCB最大的叶节点
 * 
 * @param node 
 * @return Node* 
 */
Node* select(Node* node){
    if (node->children_num==0)
    {
        return node;
    }
    else
    {
        float ucb_max=0;
        int max_child=0;
        float ucb;
        for (size_t i = 0; i < node->children_num; i++)
        {
            ucb=UCB(node->children[i]);
            if (ucb>ucb_max)
            {
                max_child=i;
                ucb_max=ucb;
            }
            
        }

        return select(node->children[max_child]);
        
    }
    
    
}



