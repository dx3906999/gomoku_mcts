#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "global.h"
#include "game.h"

int game_mode;

int i_current=0;
int j_current=0;
int i_black_last=0;
int j_black_last=0;
int i_white_last=0;
int j_white_last=0;


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
            printf("invalid choice.");
            break;
        }
    }
    

    // print_title();
    // print_chessboard();
    // i_current=1;
    // j_current=10;
    // update_global_chessboard_data_one_step();
    // update_chessboard_str();
    
    // print_chessboard();
    // get_move_input(&i_current,&j_current);
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

    for (size_t k = 0; k < 2; k++)
    {
        chessboard_str[3*(i_black_last+1)+i_black_last+i_black_last*2*29+4*j_black_last+k]=BLACK_LAST_STR[k];
        chessboard_str[3*(i_white_last+1)+i_white_last+i_white_last*2*29+4*j_white_last+k]=WHITE_LAST_STR[k];
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
void update_chessboard_data_one_step(int** chessboard_data,int i,int j,player player){
    chessboard_data[i][j]=player;
}

void update_global_chessboard_data_one_step(){
    global_chessboard_data[i_current][j_current]=current_player;
}

bool chessboard_is_full(int** chessboard_data){
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
 * @return true 
 * @return false 
 */
bool is_win(int** chessboard_data,player player){
    bool result;
    if (player==WHITE)  // 白棋无禁手
    {
        result=is_five_in_a_row();
    }
    else  // 黑棋考虑禁手
    {
        result=(!is_move_forbidden())&&(is_five_in_a_row());
    }
    return result;
}

void human_vs_human(){
    
}

void human_vs_ai(){

}
void train(){

}

