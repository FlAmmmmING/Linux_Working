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
    player* play = new player; // 游玩角色，需要用指针类型
    while (choice) {
        cin >> choice;
        switch (choice){
            case 1: 
                playMinesweeper(play);
                flash();
            break;
            case 2: 
                playHuarong(play);
                flash();
            break;
            case 3:
                playMaze(play);
                flash();
            break;
            case 9:
                play->show_score();
                sleep(3);
                system("clear");
                menu();
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
