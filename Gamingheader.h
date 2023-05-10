// 万能头文件
#pragma once
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <ctime>
#include <unistd.h>
#include <termios.h>
#include <queue>
#include <string.h>
using namespace std;



// 创建一个游戏集合菜单
void menu() 
{
    cout << 
    "**********Classic Gaming!Have Fun!**********\n"
    "Please Select Game:>************************\n"
    "1.Minesweeper*******************************\n"
    "2.Digital Huarong Road**********************\n"
    "3.Random Maze*******************************\n"
    "if you want to get your score, then input: 9\n"
    "if you want to exit system, then input: 0 **\n"   
    "Please input the Index**********************"
    << endl;
}

// 刷新页面，然后重现菜单界面
void flash() 
{
    system("clear");
    menu();
}

// 写一个记录当前游玩分数的结构体
class player
{
public:
    int score = 0; // 初始状态下为0，分数可以为负数
    void get_score(int x) {
        this->score += x;
    }
    void lose_score(int x) {
        this->score -= x;
    }
    void show_score() {
        cout << "Your current score is :> " << this->score << endl;
    }
};

// 调用Linux独有的termios库，只需要输入回车就可以返回输入
char getch()
{
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return (buf);
}


// 寻找文件查找登陆用户
