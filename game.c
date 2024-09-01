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

int chessboard_data[15][15]={[0 ... 14][0 ... 14]=0};

char chessboard_str[]=
    "15 �����Щ��Щ��Щ��Щ��Щ��Щ��Щ��Щ��Щ��Щ��Щ��Щ��Щ���\n"
    "14 �����੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤��\n"
    "13 �����੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤��\n"
    "12 �����੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤��\n"
    "11 �����੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤��\n"
    "10 �����੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤��\n"
    "9  �����੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤��\n"
    "8  �����੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤��\n"
    "7  �����੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤��\n"
    "6  �����੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤��\n"
    "5  �����੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤��\n"
    "4  �����੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤��\n"
    "3  �����੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤��\n"
    "2  �����੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤�੤��\n"
    "1  �����ة��ة��ة��ة��ة��ة��ة��ة��ة��ة��ة��ة��ة���\n"
    "   A B C D E F G H I J K L M N O\n";

char title[]=
"  ____                       _\n"
" / ___| ___  _ __ ___   ___ | | ___   _\n"
"| |  _ / _ \\| '_ ` _ \\ / _ \\| |/ / | | |\n"
"| |_| | (_) | | | | | | (_) |   <| |_| |\n"
" \\____|\\___/|_| |_| |_|\\___/|_|\\_\\\\__,_|              \x1b[1;36mby dx3qOb\x1b[0m\n";

char BLACK_STR[]="��";
char WHITE_STR[]="��";
char BLACK_LAST_STR[]="��";
char WHITE_LAST_STR[]="��";

/* 
-------------------------------------------------------------------------
-------------------------------------------------------------------------
-------------------------------fuctions----------------------------------
-------------------------------------------------------------------------
 */


int main(int argc, char const *argv[])
{
    // ���߼�ûд
    print_title();
    // print_chessboard();
    i_current=1;
    j_current=10;
    update_chessboard_data_one_step();
    update_chessboard_str();
    
    print_chessboard();
    get_move_input(&i_current,&j_current);
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
 * @brief �ڶԾ��ж�ȡһ��������
 * 
 * @param i 
 * @param j 
 * @return true �ɹ���ȡ����
 * @return false �˳���������ȡ����ʧ��
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
 * @brief ����chessboard_data����chessboard_str
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
        chessboard_str[3*(i_black_last+1)+i_black_last+i_black_last*2*29+4*j_black_last+k]=BLACK_LAST_STR[k];
        chessboard_str[3*(i_white_last+1)+i_white_last+i_white_last*2*29+4*j_white_last+k]=WHITE_LAST_STR[k];
    }
    
    
}

/**
 * @brief ���±�����������
 * 
 */
void update_chessboard_data_one_step(){
    chessboard_data[i_current][j_current]=current_player;
}

