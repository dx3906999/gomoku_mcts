#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>

#include "global.h"
#include "game.h"
#include "mcts.h"
#include "referee.h"

int game_mode;

int i_current=0;
int j_current=0;
int i_black_last=-1;
int j_black_last=-1;
int i_white_last=-1;
int j_white_last=-1;

int i_direction[8]={1,-1,0,0,1,-1,1,-1};
int j_direction[8]={0,0,1,-1,1,-1,-1,1};

player_t current_player=BLACK;

player_t global_chessboard_data[15][15]={[0 ... 14][0 ... 14]=0};

char chessboard_str[]=
    "15 ┌─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┐\n"
    "14 ├─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┤\n"
    "13 ├─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┤\n"
    "12 ├─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┤\n"
    "11 ├─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┤\n"
    "10 ├─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┤\n"
    "9  ├─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┤\n"
    "8  ├─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┤\n"
    "7  ├─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┤\n"
    "6  ├─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┤\n"
    "5  ├─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┤\n"
    "4  ├─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┤\n"
    "3  ├─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┤\n"
    "2  ├─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┤\n"
    "1  └─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┘\n"
    "   A B C D E F G H I J K L M N O\n";

char title[]=
"  ____                       _\n"
" / ___| ___  _ __ ___   ___ | | ___   _\n"
"| |  _ / _ \\| '_ ` _ \\ / _ \\| |/ / | | |\n"
"| |_| | (_) | | | | | | (_) |   <| |_| |\n"
" \\____|\\___/|_| |_| |_|\\___/|_|\\_\\\\__,_|              \x1b[1;36mby dx3qOb\x1b[0m\n";

char BLACK_STR[]="○";
char WHITE_STR[]="●";
char BLACK_LAST_STR[]="△";
char WHITE_LAST_STR[]="▲";

/* 
-------------------------------------------------------------------------
-------------------------------------------------------------------------
-------------------------------fuctions----------------------------------
-------------------------------------------------------------------------
 */


int main(int argc, char const *argv[])
{
    char* chessboard_str_backup=(char*)malloc(sizeof(chessboard_str));
    memcpy(chessboard_str_backup,chessboard_str,sizeof(chessboard_str));
    signal(SIG_SAVE,signal_handle);

    while (1)
    {
        int game_mode_choice;
        char ch;

        print_title();
        printf("1.pvp\n2.pve(black)\n3.pve(white)\n4.train\n5.quit\n");
        scanf("%d",&game_mode_choice);
        while((ch = getchar()) != '\n' && ch != EOF);
        switch (game_mode_choice)
        {
        case 1:
            human_vs_human();
            break;
        case 2:
            human_vs_ai(BLACK);
            break;
        case 3:
            human_vs_ai(WHITE);
            break;
        case 4:
            train();
            break;
        case 5:
            exit(0);
            break;
        default:
            printf("invalid choice.\n");
            break;
        }

        memset(global_chessboard_data,0,sizeof(global_chessboard_data));
        memcpy(chessboard_str,chessboard_str_backup,sizeof(chessboard_str));
        
        // some global_var
        i_current=0;
        j_current=0;
        i_black_last=-1;
        j_black_last=-1;
        i_white_last=-1;
        j_white_last=-1;
        current_player=BLACK;

        visit_num_sum=0;
        global_mcts_player=WHITE;
        mcts_root=NULL;
        mcts_last_choice=NULL;
    }

    free(chessboard_str_backup);

    
    return 0;
}




void print_chessboard(){
    printf("%s",chessboard_str);
}

void print_title(){
    printf("\x1b[1;33m%s\x1b[0m",title);
}

/**
 * @brief 在对局中读取一步的坐标
 * 
 * @param i 读出的i坐标
 * @param j 读出的j坐标
 * @return true 成功读取坐标
 * @return false 退出、保存或读取坐标失败
 */
bool get_move_input(int* i, int* j){
    bool result; // true if input is valid for chessboard, false otherwise
    // char x_char=0;
    int y_int=0;
    char ch;

    scanf("%c", &ch);
    ch=(char)toupper(ch);
    if (ch=='Q')
    {
        printf("quit game.\n");
        exit(0);
    }
    else if (ch=='W')
    {
        printf("save game.\n");
        while((ch = getchar()) != '\n' && ch != EOF);//clear the buffer
        // save
        raise(SIG_SAVE);
        // maybe use signal
        result=false;
    }
    else if (ch>='A'&&ch<='O')
    {
        *j=(int)(ch-'A');
        scanf("%d",&y_int);
        while((ch = getchar()) != '\n' && ch != EOF);//clear the buffer

        if (y_int>0&&y_int<=15)
        {
            *i=(int)(15-y_int);
            result=true;
        }
        else
        {
            printf("The second parameter should be a integer from 1 to 15.\n");
            result=false;
        }
    }
    else
    {
        printf("The first parameter should be a letter from 'a' to 'o' or 'A' to 'O'.\n");
        result=false;
        while((ch = getchar()) != '\n' && ch != EOF);//clear the buffer
    }


    return result;

}

bool check_move_input_is_valid(player_t chessboard_data[15][15],int i,int j){
    if (chessboard_data[i][j]!=EMPTY)
    {
        return false;
    }
    else
    {
        return true;
    }
    
}

/**
 * @brief 根据global_chessboard_data更新chessboard_str
 * 
 */
void update_chessboard_str(){

    // normal , not last , print

    for (size_t i = 0; i < 15; i++)
    {
        for (size_t j = 0; j < 15; j++)
        {
            switch (global_chessboard_data[i][j])
            {
            case BLACK:
                for (size_t k = 0; k < 2; k++)
                {
                    chessboard_str[3*(i+1)+i+i*2*29+4*j+k]=BLACK_STR[k];
                }
                
                break;
            case WHITE:
                for (size_t k = 0; k < 2; k++)
                {
                    chessboard_str[3*(i+1)+i+i*2*29+4*j+k]=WHITE_STR[k];
                }
                
            default:
                break;
            }
        }
        
    }

    // last , print

    if (i_black_last!=-1&&j_black_last!=-1)
    {
        for (size_t k = 0; k < 2; k++)
        {
            chessboard_str[3*(i_black_last+1)+i_black_last+i_black_last*2*29+4*j_black_last+k]=BLACK_LAST_STR[k];
        }
    }
    
    if (i_white_last!=-1&&j_white_last!=-1)
    {
        for (size_t k = 0; k < 2; k++)
        {
            chessboard_str[3*(i_white_last+1)+i_white_last+i_white_last*2*29+4*j_white_last+k]=WHITE_LAST_STR[k];
        }
        
    }
    
    
}

/**
 * @brief 更新一步本棋盘数据
 * 
 * @param chessboard_data 
 * @param i 
 * @param j 
 * @param player 
 */
void update_chessboard_data_one_step(player_t chessboard_data[15][15],int i,int j,player_t player){
    chessboard_data[i][j]=player;
}

void update_global_chessboard_data_one_step(){
    global_chessboard_data[i_current][j_current]=current_player;
}







void signal_handle(int signum){
    switch (signum)
    {
    case SIG_SAVE:
        printf("yes");//something
        break;
    
    default:
        break;
    }
}


void human_vs_human(){
    printf("\x1b[0m\x1b[1;32mHuman vs Human mode.\n\x1b[0m");
    printf("%s is black and %s is white.\n",BLACK_STR,WHITE_STR);
    int round=0;
    bool state=true;
    int is_winner_state=0;
    int i_input;
    int j_input;
    
    while (1)
    {
        round++;
        
        do
        {
            current_player=BLACK;
            print_chessboard();
            printf("Round %d:black's (%s %s) turn.\n",round,BLACK_STR,BLACK_LAST_STR);
            printf("Please input the position of your chess piece, such as 'a1' or 'A1'.\n");
            state=get_move_input(&i_input,&j_input);
            if (state)
            {
                state=check_move_input_is_valid(global_chessboard_data,i_input,j_input);
                if (state)
                {
                    i_black_last=i_input;
                    j_black_last=j_input;
                    i_current=i_input;
                    j_current=j_input;
                    update_global_chessboard_data_one_step();
                    update_chessboard_str();
                    is_winner_state=is_winner(global_chessboard_data,BLACK,i_black_last,j_black_last);
                    if (is_winner_state==2)
                    {
                        printf("Black lost!\n");
                        print_chessboard();
                        return;
                    }
                    else if (is_winner_state==1)
                    {
                        printf("Black win!\n");
                        print_chessboard();
                        return;
                    }
                    else if (chessboard_is_full(global_chessboard_data))
                    {
                        printf("The chessboard is full!\nTie.\n");
                        print_chessboard();
                        return;
                    }
                    
                    
                }
                else
                {
                    printf("please enter a valid input.\n");
                }
                
                
            }
            

        } while (!state);



        do
        {
            current_player=WHITE;
            print_chessboard();
            printf("Round %d:white's (%s %s) turn.\n",round,WHITE_STR,WHITE_LAST_STR);
            printf("Please input the position of your chess piece, such as 'a1' or 'A1'.\n");
            state=get_move_input(&i_input,&j_input);
            if (state)
            {
                state=check_move_input_is_valid(global_chessboard_data,i_input,j_input);
                if (state)
                {
                    i_white_last=i_input;
                    j_white_last=j_input;
                    i_current=i_input;
                    j_current=j_input;
                    update_global_chessboard_data_one_step();
                    update_chessboard_str();
                    if (is_winner(global_chessboard_data,WHITE,i_white_last,j_white_last))
                    {
                        printf("White won!\n");
                        print_chessboard();
                        return;
                    }
                    else if (chessboard_is_full(global_chessboard_data))
                    {
                        printf("The chessboard is full!\nTie.\n");
                        print_chessboard();
                        return;
                    }
                    
                    
                }
                else
                {
                    printf("please enter a valid input.\n");
                }
                
                
            }
            

        } while (!state);
        

        
        
    }
    
}

void human_vs_ai(player_t human_player){
    printf("\x1b[0m\x1b[1;32mHuman vs AI mode.\n\x1b[0m");
    printf("%s is black and %s is white.\n",BLACK_STR,WHITE_STR);
    int round=0;
    bool state=true;
    int is_winner_state=0;
    int i_input;
    int j_input;
    int i_ai;
    int j_ai;

    while (human_player==BLACK)
    {
        round++;
        
        do
        {
            current_player=BLACK;
            print_chessboard();
            printf("Round %d:black's (%s %s) turn.\n",round,BLACK_STR,BLACK_LAST_STR);
            printf("Please input the position of your chess piece, such as 'a1' or 'A1'.\n");
            state=get_move_input(&i_input,&j_input);
            if (state)
            {
                state=check_move_input_is_valid(global_chessboard_data,i_input,j_input);
                if (state)
                {
                    i_black_last=i_input;
                    j_black_last=j_input;
                    i_current=i_input;
                    j_current=j_input;
                    update_global_chessboard_data_one_step();
                    // update_chessboard_str();
                    is_winner_state=is_winner(global_chessboard_data,BLACK,i_black_last,j_black_last);
                    if (is_winner_state==2)
                    {
                        update_chessboard_str();
                        printf("Black lost!\n");
                        print_chessboard();
                        return;
                    }
                    else if (is_winner_state==1)
                    {
                        update_chessboard_str();
                        printf("Black won!\n");
                        print_chessboard();
                        return;
                    }
                    else if (chessboard_is_full(global_chessboard_data))
                    {
                        update_chessboard_str();
                        printf("The chessboard is full!\nTie.\n");
                        print_chessboard();
                        return;
                    }
                    
                    
                }
                else
                {
                    printf("please enter a valid input.\n");
                }
                
                
            }
            

        } while (!state);

        printf("Round %d:white's (%s %s) turn.\n",round,WHITE_STR,WHITE_LAST_STR);
        //printf("Please input the position of your chess piece, such as 'a1' or 'A1'.\n");
        mcts(global_chessboard_data,80000,10000,WHITE,&i_ai,&j_ai);
        current_player=WHITE;
        i_current=i_ai;
        j_current=j_ai;
        i_white_last=i_ai;
        j_white_last=j_ai;
        update_global_chessboard_data_one_step();
        update_chessboard_str();
        if (is_winner(global_chessboard_data,WHITE,i_white_last,j_white_last))
        {
            printf("White won!\n");
            print_chessboard();
            return;
        }
        else if (chessboard_is_full(global_chessboard_data))
        {
            printf("The chessboard is full!\nTie.\n");
            print_chessboard();
            return;
        }

        

    }
    

}
void train(){

}

// i_white_last

