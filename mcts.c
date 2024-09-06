#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>

#include "mcts.h"
#include "node.h"
#include "global.h"


float UCB_C=1.0;
int visit_num_sum=0;


void init_mcts(){
    Node* root=new_Node(NULL);
    
}


/**
 * @brief 对当前棋局进行count次mcts次模拟，得到最有价值的节点
 * 
 * @param chessboard_data 
 * @param count 
 * @param i_result 
 * @param j_result 
 */
void mcts(int chessboard_data[15][15],int count,int* i_result,int* j_result){
    Node* select_node;
    
}







float UCB(Node* node){
    return (node->visit_num!=0)?(node->value/node->visit_num+UCB_C*sqrt((2*log(visit_num_sum))/node->visit_num)):__FLT32_NORM_MAX__;
}


/**
 * @brief 选择UCB最大的叶节点
 * 
 * @param node 
 * @return Node* 
 */
/* Node* select(Node* node){
    if (node->children_num!=node->children_num_max)
    {
        return expand(node);
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
    
    
} */

Node* select(Node* node){
    while (node->children_num!=0)
    {
        if (node->children_num!=node->children_num_max)
        {
            return expand_random(node);
        }
        else
        {
            node=best_child(node);
        }
        
    }

    return node;
    
}


Node* best_child(Node* node){
    float ucb_max=0;
    float ucb_temp;
    int best_child;
    for (size_t i = 0; i < node->children_num; i++)
    {
        ucb_temp=UCB(node->children[i]);
        if (ucb_temp>ucb_max)
        {
            ucb_max=ucb_temp;
            best_child=i;
        }
        
    }

    return node->children[best_child];
    
}



/**
 * @brief 得到棋盘上随机空位置，要在之前检验棋盘是否已满
 * 
 * @param chessboard_data 
 * @param i_result 
 * @param j_result 
 */
void get_random_move(int chessboard_data[15][15],int* i_result, int* j_result){
    int i_random;
    int j_random;
    srand(time(NULL));
    do
    {
        i_random=rand()%15;
        j_random=rand()%15;
    } while (chessboard_data[i_random][j_random]!=EMPTY);
    
    *i_result=i_random;
    *j_result=j_random;
}


Node* expand_random(Node* node){
    Node* new_node=NULL;
    int i_random;
    int j_random;
    bool is_tried=false;
    srand(time(NULL));

    do
    {
        is_tried=false;
        i_random=rand()%15;
        j_random=rand()%15;
        if ((node->chessboard_data[i_random][j_random]!=EMPTY)||(i_random==node->i&&j_random==node->j))
        {
            is_tried=true;
            
        }
        else
        {
            for (size_t i = 0; i < node->children_num; i++)
            {
                if (i_random==node->children[i]->i&&j_random==node->children[i]->j)
                {
                    is_tried=true;
                    break;
                }

            }
        }
        

        
    } while (is_tried);

    new_node=new_Node(node);
    change_node_point(new_node,i_random,j_random);
    return new_node;
    
}


Node* expand(Node* node){
    Node* new_node=new_Node(node);// has problem
    int i_result;
    int j_result;
    int try_state=false;
    do
    {
        try_state=false;
        //get_random_move(chessboard_data,&i_result,&j_result);
        for (size_t i = 0; i < node->children_num; i++)
        {
            if (i_result==node->children[i]->i&&j_result==node->children[i]->j)
            {
                try_state=true;
                break;
            }
            
        }
        
    } while (try_state);
    
    change_node_point(new_node,i_result,j_result);
    return new_node;
    
}

void backup(Node* node,player player,int value){
    while (node!=NULL)
    {
        node->visit_num++;
        node->value+=value;
        node=node->parent;
    }
    
}

