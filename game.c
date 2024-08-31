#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "global.h"
#include "game.h"

int game_mode;

int current_i=0;
int current_j=0;
int black_last_i=0;
int black_last_j=0;
int white_last_i=0;
int white_last_j=0;


int current_player=BLACK;

int chessboard_data[15][15]={[0 ... 14][0 ... 14]=0};

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
    // 主逻辑没写
    print_title();
    print_chessboard();
    get_move_input(&current_i,&current_j);
    return 0;
}


void print_menu(){

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
 * @param i 
 * @param j 
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
 * @brief 根据chessboard_data更新chessboard_str
 * 
 */
void update_chessboard_str(){

    // normal , not last , print

    for (size_t i = 0; i < 15; i++)
    {
        for (size_t j = 0; j < 15; j++)
        {
            switch (chessboard_data[i][j])
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
        chessboard_str[3*(black_last_i+1)+black_last_i+black_last_i*2*29+4*black_last_j+k]=BLACK_LAST_STR[k];
        chessboard_str[3*(white_last_i+1)+white_last_i+white_last_i*2*29+4*white_last_j+k]=WHITE_LAST_STR[k];
    }
    
    
}

/**
 * @brief 更新本步棋盘数据
 * 
 */
void update_chessboard_data_one_step(){
    chessboard_data[current_i][current_j]=current_player;
}

