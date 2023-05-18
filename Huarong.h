// 遍历的方法简单实现华容道
#pragma once
#include "Gamingheader.h"

struct Huarong
{
private:
	int order; // 阶数
	int *arr;
	int cnt = 0;  // 用于记录走了多少步
	bool jud = 0; // 用于判断用户是否选择中途退出游戏
	player *p;	  // 当前游戏玩家

public:
	Huarong(int n, player *player) // 构造函数中传进游玩的玩家信息
	{
		order = n;
		arr = new int[order * order];
		for (int i = 0; i < order * order; i++)
		{
			arr[i] = i;
		}
		for (int i = 0; i < order * order; i++)
		{
			int x = arr[i];
			int pos = rand() % (n * n);
			arr[i] = arr[pos];
			arr[pos] = x;
		}
		this->p = player;
	}
	void print()
	{
		system("clear"); // 代码用于清除cmd
		cout << "If you want to exit game, please press q" << endl;
		cout << "current step:" << cnt << endl; // 用于记录目前你走了几步
		for (int i = 0; i < order * order; i++)
		{
			if (arr[i] == 0)
			{
				cout.width(3);
				cout << " ";
			}
			else
			{
				cout.width(3);
				cout << arr[i];
			}
			if (i % order == order - 1)
			{ // 换行操作
				cout << endl;
			}
		}
		// 打印操作盘
		cout << endl;
		cout << "    W" << endl;
		cout << "  A   D  " << endl;
		cout << "    S" << endl;
	}
	bool Finish()
	{
		for (int i = 0; i < order * order - 1; i++)
		{ // 判断是否成功
			if (arr[i] != i + 1)
			{
				return false;
			}
		}
		return true;
	}
	void play()
	{
		print();
		if (jud)
		{
			cout << "exit game!" << endl;
			cout << "You lose " << 10 * (order - 1) << " scores!" << endl;
			p->lose_score(10 * (order - 1));
		}
		else if (Finish())
		{
			cout << "You Win!" << endl;
			cout << "You get " << max(10 * (order - 1) - cnt, 10 * (order - 2)) << " scores!" << endl;
			p->get_score(max(10 * (order - 1) - cnt, 10 * (order - 2)));
		}
	}
	bool Exit()
	{
		return jud;
	}
	void move(char a)
	{
		if (a == 'q')
		{
			cout << "Are you sure? It will deduct your secore!(Y/N)" << endl;
			char jud2;
			cin >> jud2; // 再次确认是否要中途退出
			if (jud2 == 'Y')
			{
				jud = 1;
				cnt = max(0, cnt - 1);
				return;
			}
			else
			{
				system("clear");
				cnt = max(0, cnt - 1);
				print();
			}
		}
		cnt++;
		if (a == 'w' && (FindBlank() + order) < order * order)
		{
			int pos = FindBlank();
			int x = arr[pos];
			arr[pos] = arr[pos + order];
			arr[pos + order] = x;
		}
		else if (a == 's' && (FindBlank() - order) >= 0)
		{
			int pos = FindBlank();
			int x = arr[pos];
			arr[pos] = arr[pos - order];
			arr[pos - order] = x;
		}
		else if (a == 'a' && (FindBlank() + 1) < order * order && (FindBlank() + 1) % order != 0)
		{
			int pos = FindBlank();
			int x = arr[pos];
			arr[pos] = arr[pos + 1];
			arr[pos + 1] = x;
		}
		else if (a == 'd' && (FindBlank() - 1) >= 0 && FindBlank() % order != 0)
		{
			int pos = FindBlank();
			int x = arr[pos];
			arr[pos] = arr[pos - 1];
			arr[pos - 1] = x;
		}
		else
		{
			move(getch());
			cnt = max(0, cnt - 1);
		}
	}
	int FindBlank() // 判断哪里是空的
	{
		for (int i = 0; i < order * order; i++)
		{ // 找华容道为空的格子
			if (arr[i] == 0)
			{
				return i;
			}
		}
		return 0;
	}
};

void playHuarong(player *play)
{
	system("clear");

	cout << "Please input the order of Huarong Road(2 ~ 10):>";
	int order;
	cin >> order;
	while (order < 2 || order > 10)
	{
		cout << "Invalid input! The length of order should be in range of 2 to 10" << endl;
		cout << "Please input again:>";
		cin >> order;
	}
	Huarong hr(order, play);
	while (true)
	{
		hr.play();
		hr.move(getch());
		if (hr.Exit() || hr.Finish())
		{
			hr.play();
			break;
		}
	}
	sleep(3);
}
