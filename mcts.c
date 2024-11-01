#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>

#include "mcts.h"
#include "node.h"
#include "global.h"
#include "game.h"



float UCB_C=1.0;
int visit_num_sum=0;
player global_mcts_player=WHITE;
Node* mcts_root=NULL;
Node* mcts_last_choice=NULL;




/**
 * @brief 对当前棋局进行count次mcts次模拟，得到最有价值的节点
 * 
 * @param chessboard_data 
 * @param count 
 * @param i_result 
 * @param j_result 
 */
// void mcts(int chessboard_data[15][15],int mcts_count,player mcts_player,int* i_result,int* j_result){
//     global_mcts_player=mcts_player;
//     Node* root=(mcts_root==NULL)?(new_Node(NULL)):(mcts_root);
//     root->player=mcts_player;
//     Node* selected_node=NULL;
//     int value;

//     if (mcts_root==NULL)
//     {
//         root->i=i_current+1;
//         root->j=j_current+1;
//     }
    

//     for (size_t i = 0; i < mcts_count; i++)
//     {
//         selected_node=select_mcts(root);
//         value=simulation(selected_node,MAX_SIMULATION);
//         backup(selected_node,value);
//     }

//     selected_node=best_avg_value_child(root);

//     *i_result=selected_node->i;
//     *j_result=selected_node->j;
    
    
// }


void mcts(int chessboard[15][15],int mcts_count,int max_simulation,player mcts_player,int* i_result,int* j_result){
    Node* root=NULL;
    Node* select_node=NULL;
    bool human_step_in_tree=false;
    int value=0;
    global_mcts_player=mcts_player;

    if (mcts_last_choice)
    {
        for (size_t i = 0; i < mcts_last_choice->children_num; i++)
        {
            if ((i_current==mcts_last_choice->children[i]->i)&&(j_current==mcts_last_choice->children[i]->j))
            {
                root=mcts_last_choice->children[i];
                mcts_root=root;
                human_step_in_tree=true;
                
                // free other nodes
                for (size_t i = 0; i < mcts_last_choice->children_num; i++)
                {
                    if ((i_current!=mcts_last_choice->children[i]->i)||(j_current!=mcts_last_choice->children[i]->j))
                    {
                        free_Node(mcts_last_choice->children[i]);
                    }
                    
                }

                free(mcts_last_choice);
                mcts_last_choice=NULL;
                visit_num_sum=root->visit_num;
                root->parent=NULL;
                
                break;
                
            }
            
            
        }

        if (!human_step_in_tree)
        {
            
            visit_num_sum=0;
            root=new_Node(NULL);
            root->children_num_max=mcts_last_choice->children_num_max-2;
            change_node_point(root,i_current,j_current);
            root->player=0-mcts_player;
            free_Node(mcts_last_choice);
            mcts_last_choice=NULL;
        }
        
        
    }
    else
    {
        root=new_Node(NULL);
        change_node_point(root,i_current,j_current);
        root->player=0-mcts_player;
    }

    for (size_t i = 0; i < mcts_count; i++)
    {
        select_node=select_mcts(root);
        value=simulation(select_node,225);
        backup(select_node,value);
    }
    
    // select_node=best_avg_value_child(root);
    select_node=most_visited_child(root);
    *i_result=select_node->i;
    *j_result=select_node->j;
    
    for (size_t i = 0; i < root->children_num; i++)
    {
        if (root->children[i]!=select_node)
        {
            free_Node(root->children[i]);
        }
        
    }

    free(root);
    mcts_last_choice=select_node;
    mcts_last_choice->parent=NULL;
    



    
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
/* Node* select_mcts(Node* node){
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

        return select_mcts(node->children[max_child]);
        
    }
    
    
} */

Node* select_mcts(Node* node){
    while (node->children_num_max!=0)
    {
        if (node->children_num!=node->children_num_max)
        {
            return expand_random(node);
        }
        else
        {
            node=best_ucb_child(node);
        }
        
    }

    return node;
    
}


Node* best_ucb_child(Node* node){
    float ucb_max=0;
    float ucb_temp=0;
    int best_ucb_child=0;
    for (size_t i = 0; i < node->children_num; i++)
    {
        ucb_temp=UCB(node->children[i]);
        if (ucb_temp>ucb_max)
        {
            ucb_max=ucb_temp;
            best_ucb_child=i;
        }
        
    }

    return node->children[best_ucb_child];
    
}

Node* best_avg_value_child(Node* node){
    float avg_value_max=0;
    float avg_value_temp=0;
    int best_value_child=0;
    for (size_t i = 0; i < node->children_num; i++)
    {
        avg_value_temp=((float)node->children[i]->value)/((float)node->children[i]->visit_num);
        if (avg_value_temp>avg_value_max)
        {
            avg_value_max=avg_value_temp;
            best_value_child=i;
        }
        
        
    }

    return (node->children_num==0)?(node):(node->children[best_value_child]);
    
}

Node* most_visited_child(Node* node){
    float visit_max=0;
    float visit_tmp=0;
    int most_visited_child=0;
    for (size_t i = 0; i < node->children_num; i++)
    {
        visit_tmp=node->children[i]->visit_num;
        if (visit_tmp>visit_max)
        {
            visit_max=visit_tmp;
            most_visited_child=i;
        }
        
    }

    return (node->children_num==0)?(node):(node->children[most_visited_child]);
    
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
    int i_random=-1;
    int j_random=-1;
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

void backup(Node* node,int value){
    while (node!=NULL)
    {
        node->visit_num++;
        node->value+=value;
        node=node->parent;
    }
    visit_num_sum++;
    
}


int simulation(Node* node,int max_count){
    int chessboard_data[15][15];
    int i_random;
    int j_random;
    int value;
    int now_ai_player=0-node->player;
    int is_winner_state=0;
    memcpy(chessboard_data,node->chessboard_data,sizeof(chessboard_data));
    update_chessboard_data_one_step(chessboard_data,node->i,node->j,node->player);

    // 先判断本节点下完之后是否已经获胜
    is_winner_state=is_winner(chessboard_data,node->player,node->i,node->j);
    if (is_winner_state==1)
    {
        if (node->player==global_mcts_player) 
        {
            value=1;
            return value;
        }
        else
        {
            value=-1;
            return value;
        }
        
    }
    else if (is_winner_state==2)
    {
        if (global_mcts_player==BLACK)
        {
            value=-1;
            return value;
        }
        else
        {
            value=1;
            return value;
        }
        
        
    }
    
    

    for (size_t i = 0; i < max_count; i++)
    {
        if (!chessboard_is_full(chessboard_data))
        {
            get_random_move(chessboard_data,&i_random,&j_random);
            update_chessboard_data_one_step(chessboard_data,i_random,j_random,now_ai_player);
            is_winner_state=is_winner(chessboard_data,now_ai_player,i_random,j_random);
            if (is_winner_state==1)
            {
                value=(now_ai_player==global_mcts_player)?(1):(-1);
                return value;
                
            }
            else if (is_winner_state==2)
            {
                value=(global_mcts_player==WHITE)?(1):(-1);
                return value;
            }
            
            now_ai_player=0-now_ai_player;
            
        }
        else
        {
            value=0;
            return value;
        }
        

    }

    value=0;
    return value;
    

}

