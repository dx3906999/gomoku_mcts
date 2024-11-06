#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "global.h"
#include "mcts_player.h"
#include "mcts_zero.h"




MctsPlayer* new_mcts_player(player_t player,bool is_training,float c_puct,int n_playout){
    MctsPlayer* mcts_player=malloc(sizeof(MctsPlayer));
    mcts_player->is_training=is_training;
    mcts_player->player=player;
    mcts_player->mcts=init_mcts_zero(c_puct,n_playout);
    srand(time(0));
}


action_t get_action(MctsPlayer* mcts_player,const State* state,float temperature,float porb_output[CHESSBOARD_SIZE]){
    action_t ret_action;
    if (state->history_actions_num<CHESSBOARD_SIZE)
    {
        get_actions_prob(mcts_player->mcts,state,temperature,porb_output);
        if (mcts_player->is_training)
        {
            ret_action=action_random_choose_with_d(porb_output,true,0.3);
            update_root(mcts_player->mcts,ret_action);
        }
        else
        {
            ret_action=action_random_choose_with_d(porb_output,false,0.1);
            update_root(mcts_player->mcts,-1);
        }
        
        
    }
    return ret_action;
}


action_t action_random_choose_with_d(const float prob[CHESSBOARD_SIZE],bool d,float alpha){
    float prob_temp[CHESSBOARD_SIZE];
    memcpy(prob_temp,prob,sizeof(prob_temp));
    int random_num=rand()%10001;
    int noise=rand()%MAX(((int)(10001*alpha)),1);
    for (size_t i = 1; i < CHESSBOARD_SIZE; i++)
    {
        prob_temp[i]+=prob_temp[i-1];
        prob_temp[i]*=10000;
        if (d==true)
        {
            prob_temp[i]+=rand()%MAX(((int)(10001*alpha)),1)-10001*alpha/2;
        }
        
    }

    for (size_t i = 0; i < CHESSBOARD_SIZE; i++)
    {
        if (random_num<prob_temp[i])
        {
            return i;
        }
        
    }
    
    return CHESSBOARD_SIZE-1;// 由于精度问题所以补救
    
}



