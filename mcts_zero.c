#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "global.h"
#include "mcts_zero.h"
#include "node_zero.h"
#include "referee.h"


MctsZero* init_mcts_zero(float c_puct,int n_playout){
    MctsZero* mcts=malloc(sizeof(MctsZero));
    mcts->c_puct=c_puct;
    mcts->n_playout=n_playout;
    // mcts->mcts_player=mcts_player;
    mcts->root=new_node_zero(NULL,1.0,NULL);// action 对根节点没用
}

void playout(MctsZero* mcts,const State* state){
    NodeZero* node=mcts->root;

    float prob_predicted[CHESSBOARD_SIZE];
    float value_predicted;
    int winner_state;

    State* state_temp=malloc(sizeof(State));
    memcpy(state_temp,state,sizeof(State));

    while (node->children_num>0)
    {
        node=best_uvalue_child(node,mcts->c_puct);
        do_action(state_temp,node->action);
    }

    // policy_value_analyzer(state_temp,prob_predicted,&value_predicted);
    if (state_temp->history_actions_num>0)
    {
        winner_state=is_winner(
            state_temp->chessboard,
            OPS_PLAYER(state_temp->current_player),
            state_temp->history_actions[state_temp->history_actions_num-1]/CHESSBOARD_LEN,
            state_temp->history_actions[state_temp->history_actions_num-1]%CHESSBOARD_LEN
        );
    }
    else
    {
        winner_state=0;
    }
    
    

    if (winner_state==0&&state_temp->history_actions_num<CHESSBOARD_SIZE)
    {
        policy_value_analyzer(state_temp,prob_predicted,&value_predicted);
        expand(node,prob_predicted,state->actions_unavailable);
    }
    else
    {
        if (winner_state==0)
        {
            value_predicted=0.0;
        }
        else if (winner_state==1)// 此时获胜的只能是最后一步棋
        {
            value_predicted=1.0;
        }
        else// 此时最后一手一定是黑棋，即最后一手为输方
        {
            value_predicted=-1.0;
        }
        
    }
    
    backup(node,value_predicted);

    free(state_temp);
}


void update_root(MctsZero* mcts,action_t last_action){
    if (last_action>=0&&last_action<CHESSBOARD_SIZE&&mcts->root->children[last_action]!=NULL)
    {
        mcts->root=mcts->root->children[last_action];
        mcts->root->parent->children[last_action]=NULL;
        // mcts->root->parent->children_num--;
        delete_node_zero(mcts->root->parent);// 置为NULL即可
        mcts->root->parent=NULL;
    }
    else
    {
        delete_node_zero(mcts->root);
        mcts->root=new_node_zero(NULL,1.0,NULL);
    }
    
}


void get_actions_prob_log(MctsZero* mcts,const State* state,float temperature,float porb_output[CHESSBOARD_SIZE]){
    float porb_temp=0;

    for (size_t i = 0; i < mcts->n_playout; i++)
    {
        playout(mcts,state);
    }

    for (size_t action = 0; action < CHESSBOARD_SIZE; action++)
    {
        if (mcts->root->children[action]!=NULL)
        {
            porb_temp=1.0/temperature*log(mcts->root->children[action]->n_visited+1e-10);
            porb_output[action]=porb_temp;

            log_softmax_mcts(porb_output,CHESSBOARD_SIZE);
        
        }
        
    }
    
    
}

void get_actions_prob(MctsZero* mcts,const State* state,float temperature,float porb_output[CHESSBOARD_SIZE]){
    float porb_temp=0;

    for (size_t i = 0; i < mcts->n_playout; i++)
    {
        playout(mcts,state);
    }

    for (size_t action = 0; action < CHESSBOARD_SIZE; action++)
    {
        if (mcts->root->children[action]!=NULL)
        {
            porb_temp=1.0/temperature*log(mcts->root->children[action]->n_visited+1e-10);
            porb_output[action]=porb_temp;

            softmax_mcts(porb_output,CHESSBOARD_SIZE);
        
        }
        
    }
    
    
}


void log_softmax_mcts(float x[],int n){
    float temp=0.0;
    float max=0.0;
    // int max_idx=0;
    float sum=0.0;
    for (size_t i = 0; i < n; i++)
    {
        temp=x[i];
        if (temp>max)
        {
            max=temp;
            // max_idx=i;
        }
        
    }

    for (size_t i = 0; i < n; i++)
    {
        sum+=exp(x[i]-max);
    }

    for (size_t i = 0; i < n; i++)
    {
        x[i]=x[i]-max-log(sum);
    }
    
    
    
}


void softmax_mcts(float x[],int n){
    float temp=0.0;
    float max=0.0;
    // int max_idx=0;
    float sum=0.0;
    for (size_t i = 0; i < n; i++)
    {
        temp=x[i];
        if (temp>max)
        {
            max=temp;
            // max_idx=i;
        }
        
    }

    for (size_t i = 0; i < n; i++)
    {
        sum+=exp(x[i]-max);
    }

    for (size_t i = 0; i < n; i++)
    {
        x[i]=exp(x[i]-max)/sum;
    }
}


