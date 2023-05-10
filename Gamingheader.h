#pragma once
#include <bits/stdc++.h> // 万能头
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <unistd.h>
#include <termios.h>
#include <string.h>
using namespace std;

// 实现将字符串变成整数的函数
int to_INT(string s)
{
    int n = s.size();
    int res = 0;
    int k = 1;       // 位数
    if (s[0] == '-') // 负数
    {
        for (int i = n - 1; i >= 1; i--)
        {
            res += k * (s[i] - '0');
            k *= 10;
        }
        res = -res;
    }
    else
    {
        for (int i = n - 1; i >= 0; i--)
        {
            res += k * (s[i] - '0');
            k *= 10;
        }
    }
    return res;
}

bool is_formal = 0; // 全局变量，用来表示是否有用户登陆
// 寻找文件查找登陆用户
// 如果没有找到，就当作注册
struct User
{
private:
    string Username;
    string Password;
    int score;

public:
    // 注册和登陆的实现
    void FindUser()
    {
        string i_Username; // 用户名
        string i_Password; // 密码
        string i_score;    // 当前的分数
        bool jud = 0;      // 是否有这个用户
        string line;       // 读取这一行字
        cout << "Your Username:>";
        cin >> i_Username;
        // 先查找有没有这个用户
        ifstream infile;
        infile.open("User.txt");
        getline(infile, line);
        while (infile)
        {
            if (line == i_Username)
            {
                jud = 1; // 表示我搜索到了一个用户
                break;
            }
            // 空开三行
            getline(infile, line);
            getline(infile, line);
            getline(infile, line);
        }
        if (jud)
        {
            cout << "Welcome back! " << i_Username << "! Enter your PassWord:>";
            cin >> i_Password;
            getline(infile, line);
            if (line == i_Password)
            {
                cout << "Accepted! Let's enter the game!" << endl;
                // 读取score
                getline(infile, line);
                i_score = line;
                is_formal = 1;
                Username = i_Username;
                Password = i_Password;
                score = to_INT(i_score);
                infile.close();
                sleep(3);
                return;
            }
            else
            {
                cout << "Wrong Password! Back to the menu!" << endl;
                infile.close();
                sleep(3);
                return;
            }
        }
        else
        {
            cout << "Welcome! Newbie! Let's set your Password :>";
            cin >> i_Password;
            ofstream outfile;
            outfile.open("User.txt", ios::app);
            string s[6] = {"\n", i_Username, "\n", i_Password, "\n", "0"};
            for (int i = 0; i < 6; i++)
            {
                outfile.write(s[i].data(), s[i].size());
            }
            cout << "Well Done! Let's enter the game!" << endl;
            // 赋值
            Username = i_Username;
            Password = i_Password;
            score = 0;
            outfile.close();
        }
        infile.close();

        sleep(3);
    }

    // 接口
    string getUser()
    {
        return this->Username;
    }
    string getPassWord()
    {
        return this->Password;
    }
    int getscore()
    {
        return this->score;
    }

    // 将新的分数替换原本的文件分数
    void exchangescore(int score)
    {
        // 先定位到我要修改的地方
        ifstream infile;
        string line;
        string strFileData = "";
        int cnt = 0;        // 只有当cnt % 3 == 0 的时候，才开始判断我这个数据需要修改么
        bool yes = 0;       // 我已经搜索到了需要修改的数据
        int modifyline = 0; // 这一行数据需要修改
        infile.open("User.txt");
        while (infile)
        {
            if (line == "\n")
                break;
            getline(infile, line);
            if (cnt % 3 == 0)
            {
                if (line == Username)
                {
                    yes = 1;
                    modifyline = cnt + 2;
                }
            }
            if (yes)
            {
                if (cnt == modifyline)
                {
                    strFileData += (to_string(score) + "\n");
                    yes = 0;
                }
                else
                    strFileData += line + "\n";
            }
            else
                strFileData += line + "\n";
            cnt++;
        }
        strFileData.pop_back(); // 把最后一个回车给扔了
        // 由于C++读入最后一行会读两边，去掉最后重复的行
        while (strFileData.back() != '\n')
            strFileData.pop_back();
        strFileData.pop_back(); // 再扔掉一个回车
        infile.close();
        ofstream outfile;
        outfile.open("User.txt");
        outfile.clear();
        outfile << strFileData;
        outfile.close();
    }
};

// 创建一个游戏集合菜单
void menu()
{
    cout << "**********Classic Gaming!Have Fun!**********\n"
            "Please Select Game:>************************\n"
            "1.Minesweeper*******************************\n"
            "2.Digital Huarong Road**********************\n"
            "3.Random Maze*******************************\n"
            "If you want to get your score, then input: 9\n"
            "If you want to exit system, then input: 0 **\n"
            "If you want to login/register, than input: 8\n"
            "Please input the Index :>";
}
// 登陆用户的游戏菜单
void menu(User *U)
{
    cout << "**********Classic Gaming!Have Fun!**********\n"
            "Please Select Game:>************************\n"
            "1.Minesweeper*******************************\n"
            "2.Digital Huarong Road**********************\n"
            "3.Random Maze*******************************\n"
            "If you want to get your score, then input: 9\n"
            "If you want to exit system, then input: 0 **\n"
            "Current Player: "
         << U->getUser() << ". Press 8 if you want to log out\n"
                            "Please input the Index :>";
}
// 刷新页面，然后重现菜单界面
void flash()
{
    system("clear");
    menu();
}
void flash(User *U)
{
    system("clear");
    menu(U);
}

// 写一个记录当前游玩分数的结构体
struct player
{
public:
    int score = 0; // 初始状态下为0，分数可以为负数
    player() {}
    void inputplayer(User *U) { this->score = U->getscore(); }
    void get_score(int x)
    {
        this->score += x;
    }
    void lose_score(int x)
    {
        this->score -= x;
    }
    void show_score()
    {
        cout << "Your current score is :> " << this->score << endl;
    }
    int return_score()
    {
        return this->score;
    }
    void clearscore()
    {
        score = 0; // 清空分数
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
