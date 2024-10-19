#include <stdlib.h>
#include "referee.h"
#include "global.h"
#include "game.h"

// typedef enum chess_shape_state_t{
//     TOOLONG=1,
//     FIVE,
//     FOUR_FAKE,
//     FOUR_REAL,
//     THREE_FAKE,
//     THREE_REAL
// }chess_shape_state_t;


/**
 * @brief 禁手程序，可用于递归
 * 
 * @param chessboard 
 * @param i 
 * @param j 
 * @param h_direction_last last half direction 为-1时为纯禁手判断
 * @return chess_shape_t 
 */
chess_shape_t is_banned(int chessboard[15][15],int i,int j,int h_direction_last){
    int chess_state[8][15]={0};
    chess_shape_t chess_shape_state[4]={0};
    int chess_shape_sum=0;
    scan_chess_state(chessboard,i,j,chess_state,BLACK);
    analyze_chess_state(chess_state,chess_shape_state,chessboard,i,j,BLACK);

    for (size_t h_direction = 0; h_direction < 4; h_direction++)
    {
        chess_shape_sum+=chess_shape_state[h_direction];
    }
    
    /* if (GET_SHAPE_S(chess_shape_sum,FIVE_S))// 得和一般的区分？
    {
        return false;
    }
    else if (GET_SHAPE_S(chess_shape_sum,OVERLINE_S))
    {
        return (h_direction_last==-1)?(true):chess_shape_state[h_direction_last];
    }
    else if (GET_SHAPE_S(chess_shape_sum,FOUR_HALF_S)+GET_SHAPE_S(chess_shape_sum,FOUR_OPEN_S)>=2)
    {
        return (h_direction_last==-1)?(true):chess_shape_state[h_direction_last];
    }
    else if (GET_SHAPE_S(chess_shape_sum,THREE_OPEN_S)>=2)
    {
        return (h_direction_last==-1)?(true):chess_shape_state[h_direction_last];
    }
    else
    {
        return false;
    } */
    
    if (h_direction_last==-1)
    {
        if (GET_SHAPE_S(chess_shape_sum,FIVE_S))// 得和一般的区分？
        {
            return false;
        }
        else if (GET_SHAPE_S(chess_shape_sum,OVERLINE_S))
        {
            return true;
        }
        else if (GET_SHAPE_S(chess_shape_sum,FOUR_HALF_S)+GET_SHAPE_S(chess_shape_sum,FOUR_OPEN_S)>=2)
        {
            return true;
        }
        else if (GET_SHAPE_S(chess_shape_sum,THREE_OPEN_S)>=2)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if (GET_SHAPE_S(chess_shape_sum,FIVE_S))// 得和一般的区分？
        {
            return chess_shape_state[h_direction_last];
        }
        else if (GET_SHAPE_S(chess_shape_sum,OVERLINE_S))
        {
            return chess_shape_state[h_direction_last]|TRUE_S;
        }
        else if (GET_SHAPE_S(chess_shape_sum,FOUR_HALF_S)+GET_SHAPE_S(chess_shape_sum,FOUR_OPEN_S)>=2)
        {
            return chess_shape_state[h_direction_last]|TRUE_S;
        }
        else if (GET_SHAPE_S(chess_shape_sum,THREE_OPEN_S)>=2)
        {
            return chess_shape_state[h_direction_last]|TRUE_S;
        }
        else
        {
            return chess_shape_state[h_direction_last];
        }
    }
    
    
    
}


// bool is_blocked(int chessboard[15][15],int i,int j){

// }

void scan_chess_state(int chessboard[15][15],int i,int j,int chess_state[8][15],player player){
    int empty_index=1;
    int connected_index=0;
    // int block_num=0;
    // int connection_state=1;
    int chess_state_index=0;

    for (size_t direction = 0; direction < 8; direction++)
    {
        empty_index=1;
        connected_index=0;
        chess_state_index=0;
        // block_num=0;
        // connection_state=1;
        // chess_state_index=0;
        for (size_t k = 1; k < 15 && IS_IN_CHESSBOARD(i,j,k,direction); k++)
        {
            if (chessboard[i+i_direction[direction]*k][j+j_direction[direction]*k]==player)
            {
                chess_state[direction][connected_index]++;
                if (IS_IN_CHESSBOARD(i,j,k+1,direction)&&chessboard[i+i_direction[direction]*(k+1)][j+j_direction[direction]*(k+1)]==EMPTY)
                {
                    connected_index+=2;
                    chess_state_index++;
                }
                else if (!IS_IN_CHESSBOARD(i,j,k+1,direction)||(IS_IN_CHESSBOARD(i,j,k+1,direction)&&chessboard[i+i_direction[direction]*(k+1)][j+j_direction[direction]*(k+1)]==-player))
                {
                    // chess_state_index++;
                    chess_state[direction][connected_index+1]=-1;
                    break;
                }
                
            }
            else if (chessboard[i+i_direction[direction]*k][j+j_direction[direction]*k]==EMPTY)
            {
                chess_state[direction][empty_index]++;
                if (IS_IN_CHESSBOARD(i,j,k+1,direction)&&chessboard[i+i_direction[direction]*(k+1)][j+j_direction[direction]*(k+1)]==player)
                {
                    empty_index+=2;
                    chess_state_index++;
                }
                else if (!IS_IN_CHESSBOARD(i,j,k+1,direction)||(IS_IN_CHESSBOARD(i,j,k+1,direction)&&chessboard[i+i_direction[direction]*(k+1)][j+j_direction[direction]*(k+1)]==-player))
                {
                    chess_state[direction][empty_index+1]=-1;
                    break;
                }
                
                
            }
            else
            {
                chess_state[direction][chess_state_index]=-1;
                break;
            }
            
            
        }
        
    }
    
}


void analyze_chess_state(int chess_state[8][15],chess_shape_t chess_shape_state[4],int chessboard[15][15],int i,int j,player player){

    
    for (size_t h_direction = 0; h_direction < 4; h_direction++)
    {
        if (chess_state[h_direction*2][0]>=0&&chess_state[h_direction*2+1][0]>=0)// 两边都没有block
        {
            /* if (chess_state[h_direction*2][0]+chess_state[h_direction*2+1][0]+1>5)//得加上自己
            {
                chess_shape_state[h_direction]|=OVERLINE_S;
                break;
            }
            else if (chess_state[h_direction*2][0]+chess_state[h_direction*2+1][0]+1==5)
            {
                chess_shape_state[h_direction]|=FIVE_S;
                break;
            }
            else if (chess_state[h_direction*2][0]+chess_state[h_direction*2+1][0]+1==4)
            {
                
            }
            else if (chess_state[h_direction*2][0]+chess_state[h_direction*2+1][0]+1==3)
            {
                
            }
            else if (chess_state[h_direction*2][0]+chess_state[h_direction*2+1][0]+1==2)
            {
                
            } */

            switch (chess_state[h_direction*2][0]+chess_state[h_direction*2+1][0]+1)
            {
            case 5:
                chess_shape_state[h_direction]|=FIVE_S;
                return;
                break;
            case 9:
                if (chess_state[h_direction*2][0]==4)
                {
                    chess_shape_state[h_direction]|=FIVE_S;
                }
                else
                {
                    chess_shape_state[h_direction]|=OVERLINE_S;
                }

                return;
                break;

            case 4:// 如果单纯禁手的话，只需要不是死四就行
                int unblocked_num=0;

                chessboard[i][j]=player;

                if (chess_state[h_direction*2][1]>0&&(!is_banned(chessboard,i+i_direction[h_direction*2]*(chess_state[h_direction*2][0]+1),j+j_direction[h_direction*2]*(chess_state[h_direction*2][0]+1),-1)))
                {
                    unblocked_num++;
                }

                if (chess_state[h_direction*2+1][1]>0&&(!is_banned(chessboard,i+i_direction[h_direction*2+1]*(chess_state[h_direction*2+1][0]+1),j+j_direction[h_direction*2+1]*(chess_state[h_direction*2+1][0]+1),-1)))
                {
                    unblocked_num++;
                }

                chessboard[i][j]=EMPTY;// 一定要改回来

                switch (unblocked_num)
                {
                case 2:
                    chess_shape_state[h_direction]+=FOUR_OPEN_S;
                    break;
                case 1:
                    chess_shape_state[h_direction]+=FOUR_HALF_S;
                    break;
                default:
                    break;
                }
                
                
                break;

            case 3:
                int four_check_num=0;

                chessboard[i][j]=player;

                if (chess_state[h_direction*2][1]==1&&chess_state[h_direction*2][2]==1&&(!is_banned(chessboard,i+i_direction[h_direction*2]*(chess_state[h_direction*2][0]+1),j+j_direction[h_direction*2]*(chess_state[h_direction*2][0]+1),-1)))
                {
                    four_check_num++;
                }
                if (chess_state[h_direction*2+1][1]==1&&chess_state[h_direction*2+1][2]==1&&(!is_banned(chessboard,i+i_direction[h_direction*2+1]*(chess_state[h_direction*2+1][0]+1),j+j_direction[h_direction*2+1]*(chess_state[h_direction*2+1][0]+1),-1)))
                {
                    four_check_num++;
                }

                chessboard[i][j]=EMPTY;

                switch (four_check_num)
                {
                case 2:
                    chess_shape_state[h_direction]+=FOUR_HALF_S*2;// 这其实是禁手 oeOOOeo
                    goto case_3_end;
                    break;
                case 1:
                    chess_shape_state[h_direction]+=FOUR_HALF_S;
                    goto case_3_end;
                    break;
                default:// TODO: 三连的活三判断
                    break;
                }

                chessboard[i][j]=player;// 由于活四只能是 eOOOOe ，则只需要判断临近空位落子后是否为活四
                if (chess_state[h_direction*2][1]>=1&&chess_state[h_direction*2+1][1]>=1)
                {
                    chess_shape_t advanced_chess_shape=0;
                    /* for (size_t empty_num = 0; empty_num < chess_state[h_direction*2][1]; empty_num++)
                    {
                        advanced_chess_shape=is_banned(chessboard,i+i_direction[h_direction*2]*(chess_state[h_direction*2][0]+empty_num+1),j+j_direction[h_direction*2]*(chess_state[h_direction*2][0]+empty_num+1),h_direction);
                        if (GET_SHAPE_S(advanced_chess_shape,FOUR_OPEN_S)&&(!GET_TF_S(advanced_chess_shape)))
                        {
                            chess_shape_state[h_direction]+=THREE_OPEN_S;
                            goto case_3_end;
                        }
                        
                    }
                    for (size_t empty_num = 0; empty_num < chess_state[h_direction*2+1][1]; empty_num++)
                    {
                        advanced_chess_shape=is_banned(chessboard,i+i_direction[h_direction*2+1]*(chess_state[h_direction*2+1][0]+empty_num+1),j+j_direction[h_direction*2+1]*(chess_state[h_direction*2+1][0]+empty_num+1),h_direction);
                        if (GET_SHAPE_S(advanced_chess_shape,FOUR_OPEN_S)&&(!GET_TF_S(advanced_chess_shape)))
                        {
                            chess_shape_state[h_direction]+=THREE_OPEN_S;
                            goto case_3_end;
                        }
                        
                    } */
                    
                    advanced_chess_shape=is_banned(chessboard,i+i_direction[h_direction*2]*(chess_state[h_direction*2][0]+1),j+j_direction[h_direction*2]*(chess_state[h_direction*2][0]+1),h_direction);
                    if (GET_SHAPE_S(advanced_chess_shape,FOUR_OPEN_S)&&(!GET_TF_S(advanced_chess_shape)))
                    {
                        chess_shape_state[h_direction]+=THREE_OPEN_S;
                        goto case_3_end;
                    }
                    advanced_chess_shape=is_banned(chessboard,i+i_direction[h_direction*2+1]*(chess_state[h_direction*2+1][0]+1),j+j_direction[h_direction*2+1]*(chess_state[h_direction*2+1][0]+1),h_direction);
                    if (GET_SHAPE_S(advanced_chess_shape,FOUR_OPEN_S)&&(!GET_TF_S(advanced_chess_shape)))
                    {
                        chess_shape_state[h_direction]+=THREE_OPEN_S;
                        goto case_3_end;
                    }
                    
                    
                }
                
                
                case_3_end:
                chessboard[i][j]=EMPTY;
                break;
                
            case 2:// ^ 不可能形成一个冲四一个跳活三，只可能一个冲四一个眠三，那样的话也不用判断眠三，因其与禁手无关
                int four_check_num=0;

                chessboard[i][j]=player;

                if (
                    chess_state[h_direction*2][1]==1 &&
                    chess_state[h_direction*2][2]==2 &&
                    (!is_banned(chessboard,i+i_direction[h_direction*2]*(chess_state[h_direction*2][0]+1),j+j_direction[h_direction*2]*(chess_state[h_direction*2][0]+1),-1))
                )
                {
                    four_check_num++;
                }
                if (
                    chess_state[h_direction*2+1][1]==1 &&
                    chess_state[h_direction*2+1][2]==2 &&
                    (!is_banned(chessboard,i+i_direction[h_direction*2+1]*(chess_state[h_direction*2+1][0]+1),j+j_direction[h_direction*2+1]*(chess_state[h_direction*2+1][0]+1),-1))
                )
                {
                    four_check_num++;
                }

                chessboard[i][j]=EMPTY;
                
                switch (four_check_num)
                {
                case 2:
                    chess_shape_state[h_direction]+=FOUR_HALF_S*2;// ooeOOeoo 是禁手，但 ooeooeOO 暂时不是禁手，因为有一个4是之前形成的
                    goto case_2_end;
                    break;
                case 1:
                    chess_shape_state[h_direction]+=FOUR_HALF_S;
                    goto case_2_end;
                    break;
                default:
                    break;
                }

                //TODO: 活三判断，即是否为跳活三
                chessboard[i][j]=player;
                chess_shape_t advanced_chess_shape=0;
                if (chess_state[h_direction*2][1]==1&&chess_state[h_direction*2][2]==1)
                {
                    advanced_chess_shape=is_banned(chessboard,i+i_direction[h_direction*2]*(chess_state[h_direction*2][0]+1),j+j_direction[h_direction*2]*(chess_state[h_direction*2][0]+1),h_direction);
                    if (GET_SHAPE_S(advanced_chess_shape,FOUR_OPEN_S)&&(!GET_TF_S(advanced_chess_shape)))
                    {
                        chess_shape_state[h_direction]+=THREE_OPEN_S;
                        goto case_2_end;
                    }
                    
                }
                if (chess_state[h_direction*2+1][1]==1&&chess_state[h_direction*2+1][2]==1)
                {
                    advanced_chess_shape=is_banned(chessboard,i+i_direction[h_direction*2+1]*(chess_state[h_direction*2+1][0]+1),j+j_direction[h_direction*2+1]*(chess_state[h_direction*2+1][0]+1),h_direction);
                    if (GET_SHAPE_S(advanced_chess_shape,FOUR_OPEN_S)&&(!GET_TF_S(advanced_chess_shape)))
                    {
                        chess_shape_state[h_direction]+=THREE_OPEN_S;
                        goto case_2_end;
                    }
                    
                }
                

                case_2_end:
                chessboard[i][j]=EMPTY;
                break;

            case 1:// ^ 同理，也不可能形成一个冲四一个跳活三，那样只能是眠三（因为长连）
                int four_check_num=0;

                chessboard[i][j]=player;

                if (
                    chess_state[h_direction*2][1]==1 &&
                    chess_state[h_direction*2][2]==3 &&
                    (!is_banned(chessboard,i+i_direction[h_direction*2]*(chess_state[h_direction*2][0]+1),j+j_direction[h_direction*2]*(chess_state[h_direction*2][0]+1),-1))
                )
                {
                    four_check_num++;
                }
                if (
                    chess_state[h_direction*2+1][1]==1 &&
                    chess_state[h_direction*2+1][2]==3 &&
                    (!is_banned(chessboard,i+i_direction[h_direction*2+1]*(chess_state[h_direction*2+1][0]+1),j+j_direction[h_direction*2+1]*(chess_state[h_direction*2+1][0]+1),-1))
                )
                {
                    four_check_num++;
                }

                chessboard[i][j]=EMPTY;
                
                switch (four_check_num)
                {
                case 2:
                    chess_shape_state[h_direction]+=FOUR_HALF_S*2;// oooeOeooo 是禁手，但 oooeoeOOO 暂时不是禁手，因为有一个4是之前形成的
                    goto case_2_end;
                    break;
                case 1:
                    chess_shape_state[h_direction]+=FOUR_HALF_S;
                    goto case_2_end;
                    break;
                default:
                    break;
                }

                //TODO: 跳活三判断

                chessboard[i][j]=player;
                chess_shape_t advanced_chess_shape=0;

                if (chess_state[h_direction*2][1]==1&&chess_state[h_direction*2][2]==2)
                {
                    advanced_chess_shape=is_banned(chessboard,i+i_direction[h_direction*2]*(1),j+j_direction[h_direction*2]*(1),h_direction);
                    if (GET_SHAPE_S(advanced_chess_shape,FOUR_OPEN_S)&&(!GET_TF_S(advanced_chess_shape)))
                    {
                        chess_shape_state[h_direction]+=THREE_OPEN_S;
                        goto case_1_end;
                    }
                    
                }
                if (chess_state[h_direction*2+1][1]==1&&chess_state[h_direction*2+1][2]==2)
                {
                    advanced_chess_shape=is_banned(chessboard,i+i_direction[h_direction*2+1]*(1),j+j_direction[h_direction*2+1]*(1),h_direction);
                    if (GET_SHAPE_S(advanced_chess_shape,FOUR_OPEN_S)&&(!GET_TF_S(advanced_chess_shape)))
                    {
                        chess_shape_state[h_direction]+=THREE_OPEN_S;
                        goto case_1_end;
                    }
                    
                }

                case_1_end:
                chessboard[i][j]=EMPTY;
                break;
            
            default:
                chess_shape_state[h_direction]|=OVERLINE_S;
                return;
                break;
            }
            
            
            
        }
        else if (chess_state[h_direction*2][0]==-1&&chess_state[h_direction*2+1][0]==-1)
        {
            continue;
        }
        else// 有一边有block
        {
            // int (*chess_state_unblocked)[15];
            // chess_state_unblocked=(chess_state[h_direction*2][0]==-1)?(&(chess_state[h_direction*2+1])):(&(chess_state[h_direction*2]));
            int direction_unblocked=0;
            direction_unblocked=(chess_state[h_direction*2][0]==-1)?(h_direction*2+1):(h_direction*2);

            if (chess_state[direction_unblocked][0]+1>5)
            {
                chess_shape_state[h_direction]|=OVERLINE_S;
                break;
            }
            else if (chess_state[direction_unblocked][0]+1==5)
            {
                chess_shape_state[h_direction]|=FIVE_S;
                break;
            }
            else if (chess_state[direction_unblocked][0]+1==4)
            {
                chessboard[i][j]=player;
                if (chess_state[direction_unblocked][1]>0||(!is_banned(chessboard,i+i_direction[direction_unblocked]*4,j+j_direction[direction_unblocked]*4,-1)))
                {
                    chess_shape_state[h_direction]+=FOUR_HALF_S;
                }
                chessboard[i][j]=EMPTY;
                
            }
            
        }
        
        
        
    }
    

}

