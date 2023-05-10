#pragma once
#include "Gamingheader.h"
#include "Huarong.h"
#include "Maze.h"
#include "Minesweeper.h"
// 执行
void Begin()
{
    flash();
    int choice = 100;
    User* U = new User; //开一个空间给可能需要登陆的玩家 
    player* play = new player; // 游玩角色，需要用指针类型
    while (choice) {
        cin >> choice;
        switch (choice){
            case 1: 
                playMinesweeper(play);
                is_formal ? flash(U) : flash();
            break;
            case 2: 
                playHuarong(play);
                is_formal ? flash(U) : flash();
            break;
            case 3:
                playMaze(play);
                is_formal ? flash(U) : flash();
            break;
            case 8:
                if(is_formal == 0) // 我目前没有登陆
                {
                    U->FindUser();
                    play->inputplayer(U); // 导入
                }
                else {
                    is_formal = 0; // 否则注销
                    cout << "Log out!" << endl;
                    play->clearscore();
                    sleep(3);
                }
                is_formal ? flash(U) : flash();
            break;
            case 9:
                play->show_score();
                sleep(3);
                system("clear");
                is_formal ? flash(U) : flash();
            break;
            case 0:
                cout << "Exit!" << endl;
                sleep(3);
                system("clear");
            break;
            default:
                cout << "Invaild Input! Please input again!" << endl;
                choice = 1;
            break;
        }
        
    }
}
