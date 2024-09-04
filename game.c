#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include "global.h"
#include "game.h"

int game_mode;

int i_current=0;
int j_current=0;
int i_black_last=-1;
int j_black_last=-1;
int i_white_last=-1;
int j_white_last=-1;


int current_player=BLACK;

int global_chessboard_data[15][15]={[0 ... 14][0 ... 14]=0};

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


    while (1)
    {
        int game_mode_choice;
        char ch;
        print_title();
        printf("1.pvp\n2.pve\n3.train\n4.quit\n");
        scanf("%d",&game_mode_choice);
        while((ch = getchar()) != '\n' && ch != EOF);
        switch (game_mode_choice)
        {
        case 1:
            human_vs_human();
            break;
        case 2:
            human_vs_ai();
            break;
        case 3:
            train();
            break;
        case 4:
            exit(0);
            break;
        default:
            printf("invalid choice.\n");
            break;
        }

        memset(global_chessboard_data,0,15*15*sizeof(int));
        memcpy(chessboard_str,chessboard_str_backup,sizeof(chessboard_str));
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
        // save
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

bool check_move_input_is_valid(int chessboard_data[15][15],int i,int j){
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
void update_chessboard_data_one_step(int chessboard_data[15][15],int i,int j,player player){
    chessboard_data[i][j]=player;
}

void update_global_chessboard_data_one_step(){
    global_chessboard_data[i_current][j_current]=current_player;
}

bool chessboard_is_full(int chessboard_data[15][15]){
    for (size_t i = 0; i < 15; i++)
    {
        for (size_t j = 0; j < 15; j++)
        {
            if (chessboard_data[i][j]==EMPTY)
            {
                return false;
            }
            
        }
        
    }

    return true;
    
}

/**
 * @brief 判断当前局面输赢，不包含下满棋盘的情况
 * 
 * @param chessboard_data 
 * @param player 
 * @param i 
 * @param j 
 * @return true 
 * @return false 
 */
bool is_winner(int chessboard_data[15][15],player player,int i,int j){
    bool result;
    if (player==WHITE)  // 白棋无禁手
    {
        result=is_five_in_a_row(chessboard_data,player,i,j);
    }
    else  // 黑棋考虑禁手
    {
        result=(1)&&(is_five_in_a_row(chessboard_data,player,i,j));
    }
    return result;
}

/**
 * @brief 查看本棋盘对于本玩家i,j坐标周围是否构成五子
 * 
 * @param chessboard_data 
 * @param player 
 * @param i 本步坐标i
 * @param j 本步坐标j
 * @return true 
 * @return false 
 */
bool is_five_in_a_row(int chessboard_data[15][15],player player,int i,int j){
    int i_offset=0;
    int j_offset=0;
    int num_in_a_row=1;

    int i_min=MAX(i-4,0);
    int i_max=MIN(i+4,14);
    int j_min=MAX(j-4,0);
    int j_max=MIN(j+4,14);


    /**
     * @brief 上下方向
     * 
     */
    {
        while ((i+i_offset+1)<=i_max)
        {
            i_offset++;
            if (chessboard_data[i+i_offset][j]==player)
            {
                num_in_a_row++;
            }
            else
            {
                i_offset=0;
                break;
            }

        }

        i_offset=0;

        while ((i+i_offset-1)>=i_min)
        {
            i_offset--;
            if (chessboard_data[i+i_offset][j]==player)
            {
                num_in_a_row++;
            }
            else
            {
                i_offset=0;
                break;
            }

        }

        i_offset=0;

        if (num_in_a_row>=5)
        {
            return true;
        }
    
    }

    num_in_a_row=1;

    /**
     * @brief 左右方向
     * 
     */
    {
        while ((j+j_offset+1)<=j_max)
        {
            j_offset++;
            if (chessboard_data[i][j+j_offset]==player)
            {
                num_in_a_row++;
            }
            else
            {
                j_offset=0;
                break;
            }
        }

        j_offset=0;

        while ((j+j_offset-1)>=j_min)
        {
            j_offset--;
            if (chessboard_data[i][j+j_offset]==player)
            {
                num_in_a_row++;
            }
            else
            {
                j_offset=0;
                break;
            }
        }
        
        j_offset=0;

        if (num_in_a_row>=5)
        {
            return true;
        }
        
        
    }

    num_in_a_row=1;    

    /**
     * @brief 斜向1
     * 
     */
    {
        while ((i+i_offset+1)<=i_max&&(j+j_offset+1)<=j_max)
        {
            i_offset++;
            j_offset++;
            if (chessboard_data[i+i_offset][j+j_offset]==player)
            {
                num_in_a_row++;
            }
            else
            {
                i_offset=0;
                j_offset=0;
                break;
            }
        }
        
        j_offset=0;
        i_offset=0;

        while ((i+i_offset-1)>=i_min&&(j+j_offset-1)>=j_min)
        {
            i_offset--;
            j_offset--;
            if (chessboard_data[i+i_offset][j+j_offset]==player)
            {
                num_in_a_row++;
            }
            else
            {
                i_offset=0;
                j_offset=0;
                break;
            }
            
        }

        i_offset=0;
        j_offset=0;

        if (num_in_a_row>=5)
        {
            return true;
        }
        
        
    }

    num_in_a_row=1;
    
    /**
     * @brief 斜向2
     * 
     */
    {
        while ((i+i_offset+1)<=i_max&&(j+j_offset-1)>=j_min)
        {
            i_offset++;
            j_offset--;
            if (chessboard_data[i+i_offset][j+j_offset]==player)
            {
                num_in_a_row++;
            }
            else
            {
                i_offset=0;
                j_offset=0;
                break;
            }    
        }

        i_offset=0;
        j_offset=0;

        while ((i+i_offset-1)>=i_min&&(j+j_offset+1)<=j_max)
        {
            i_offset--;
            j_offset++;
            if (chessboard_data[i+i_offset][j+j_offset]==player)
            {
                num_in_a_row++;
            }
            else
            {
                i_offset=0;
                j_offset=0;
                break;
            }
            
        }

        i_offset=0;
        j_offset=0;

        if (num_in_a_row>=5)
        {
            return true;
        }
        
    }

    num_in_a_row=1;

    return false;
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
    int i_input;
    int j_input;
    
    while (1)
    {
        round++;
        
        do
        {
            current_player=BLACK;
            print_chessboard();
            printf("Round %d:black player's (%s %s) turn.\n",round,BLACK_STR,BLACK_LAST_STR);
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
                    if (is_winner(global_chessboard_data,BLACK,i_black_last,j_black_last))
                    {
                        printf("Black player win!\n");
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
            printf("Round %d:white player's (%s %s) turn.\n",round,WHITE_STR,WHITE_LAST_STR);
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
                    if (is_winner(global_chessboard_data,WHITE,i_black_last,j_black_last))
                    {
                        printf("Black player win!\n");
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

void human_vs_ai(){

}
void train(){

}

