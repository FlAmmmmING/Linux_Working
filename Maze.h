
#include "Gamingheader.h"

const int N = 1e2; // 迷宫地图最大长度

// 广度优先搜索所需要的
int dx_in_Maze[4] = {0, 0, 1, -1};
int dy_in_Maze[4] = {1, -1, 0, 0};
int dis_in_Maze[N][N];
bool vis_in_Maze[N][N];
queue<pair<int, int>> q;

// 预定义
#define down 1
#define right 2
#define left 4
#define up 8
#define WALL -1 // 墙壁
#define ROAD 2	// 通道
struct block	// 定义墙
{
	int row, column, direction;
	block(int r, int c, int d)
	{
		row = r;
		column = c;
		direction = d;
	}
};

struct point // 定义点
{
	int x;
	int y;
};

struct MazeGame
{
	player *play;			  // 玩家信息
	int row, col;			  // 这个地图的长宽
	vector<block> myblock;	  // 动态数组
	int x_num = 1, y_num = 1; // 矿工位置
	int G[N][N];			  // 地图最大的大小
	point start, end;		  // start是迷宫起点位置， end是迷宫出口位置
	bool finish = 0;		  // 判断我是否结束游戏
	int cnt = 0;			  // 记录你目前走了多少步骤

	// 构造函数赋值，加入玩家信息
	MazeGame(int _row, int _col, player *p)
	{
		this->row = _row;
		this->col = _col;
		this->play = p;
	}
	void init() // 初始化状态
	{
		// 将地图全部置为墙
		memset(G, WALL, sizeof(G));
		// 定义起始点
		G[1][1] = ROAD;
		start.x = start.y = 1; // 记录起点位置
	}
	void FindBlock()
	{
		// 找出与当前位置相邻的墙
		if (x_num + 1 <= row && G[x_num + 1][y_num] == WALL)
			myblock.push_back(block(x_num + 1, y_num, down));
		if (y_num + 1 <= col && G[x_num][y_num + 1] == WALL)
			myblock.push_back(block(x_num, y_num + 1, right));
		if (x_num - 1 >= 1 && G[x_num - 1][y_num] == WALL)
			myblock.push_back(block(x_num - 1, y_num, up));
		if (y_num - 1 >= 1 && G[x_num][y_num - 1] == WALL)
			myblock.push_back(block(x_num, y_num - 1, left));
	}

	void playingmaze() // 开始
	{
		init();
		srand((unsigned)time(NULL)); // 随机数种子
		FindBlock();
		// 第一步压入两堵墙（起点右边和起点下面）进入循环
		while (myblock.size()) {
			int BlockSize = myblock.size();
			int randnum = rand() % BlockSize; // 生成随机数
			block SelectBlock = myblock[randnum];
			x_num = SelectBlock.row; // 赋值
			y_num = SelectBlock.column;
			switch (SelectBlock.direction) {
			case down: {
				x_num++;
				break;
			}
			case right: {
				y_num++;
				break;
			}
			case left: {
				y_num--;
				break;
			}
			case up: {
				x_num--;
				break;
			}
			}
			// 目标块如果是墙
			if (G[x_num][y_num] == WALL)
			{
				// 打通墙和目标块
				G[SelectBlock.row][SelectBlock.column] = G[x_num][y_num] = ROAD;
				// 再次找出与矿工当前位置相邻的墙
				FindBlock();
			}
			else
				// 删除动态数组中的这个元素
				myblock.erase(myblock.begin() + randnum);
		}
	}

	bool jud(int x, int y) // 判断这个块是否合理
	{
		if (vis_in_Maze[x][y] || (G[x][y] == WALL))
		{
			return false;
		}
		return true;
	}

	// 通过广度优先搜索，寻找距离初始距离所能到达的最远的点，将这个点设置为出口
	void MazeBFS()
	{
		dis_in_Maze[start.x][start.y] = 0;
		vis_in_Maze[start.x][start.y] = true; // 我遍历了
		q.push({start.x, start.y});
		while (q.size())
		{
			auto t = q.front();
			q.pop();
			vis_in_Maze[t.first][t.second] = 1;
			for (int i = 0; i < 4; i++)
			{
				int xx = t.first + dx_in_Maze[i];
				int yy = t.second + dy_in_Maze[i];
				if (jud(xx, yy))
				{
					q.push({xx, yy});
					dis_in_Maze[xx][yy] = dis_in_Maze[t.first][t.second] + 1;
				}
			}
		}
		end.x = start.x, end.y = start.y;
		for (int i = 1; i <= row; i++)
		{
			for (int j = 1; j <= col; j++)
			{
				if (dis_in_Maze[i][j] > dis_in_Maze[end.x][end.y])
					end.x = i, end.y = j;
			}
		}
	}

	void printMaze() // 不停地打印地图， 以及你目前走过的数量
	{
		system("clear");
		cout << "Reach to the '!' and you will win!" << endl;
		cout << "Current step " << cnt << endl;
		for (int i = 0; i <= row + 1; i++)
		{
			for (int j = 0; j <= col + 1; j++)
			{
				if (i == start.x && j == start.y) // 初始位置
				{
					cout << "Y";
				}
				else if (i == end.x && j == end.y) // 出口位置
				{
					cout << "!";
				}
				else if (G[i][j] == ROAD)
				{
					cout << " ";
				}
				else
				{
					printf("O"); // 生成墙壁
				}
			}
			cout << endl;
		}
		// 打印操作盘
		cout << endl;
		cout << "    W" << endl;
		cout << "  A   D  " << endl;
		cout << "    S" << endl;
	}

	// 以下是移动光标的操作

	void Moving(int x, int y)
	{
		if (G[x][y] == WALL) // 我这里不能移动
		{
			cnt--;
			return;
		}
		// 如果这个格子合法，我就把他赋值到当前的位置上
		// 赋值
		start.x = x;
		start.y = y;
	}

	// 输入操作,迷宫左右移动操作
	void Op(char op)
	{
		switch (op)
		{
		case 'w':
			cnt++;
			Moving(start.x - 1, start.y);
			break;
		case 'a':
			cnt++;
			Moving(start.x, start.y - 1);
			break;
		case 's':
			cnt++;
			Moving(start.x + 1, start.y);
			break;
		case 'd':
			cnt++;
			Moving(start.x, start.y + 1);
			break;
		case 'q':
			exit();
			break;
		default:
			cout << "Invaild Input!" << endl;
		}
		// 每一步之后判断获胜
		canWin();
	}

	// 接下来是判断获胜或者退出的操作
	void canWin()
	{
		if (start.x == end.x && start.y == end.y)
		{
			cout << "You Win!" << endl;
			cout << "You will get " << max(row * col / 5 - cnt, 0) << " score!" << endl;
			this->play->get_score(max(row * col / 5 - cnt, 0));
			finish = 1;
			return;
		}
	}

	void exit()
	{
		cout << "Are you sure? It will deduct your secore!(Y/N)" << endl;
		char jud2;
		cin >> jud2; // 再次确认是否要中途退出
		if (jud2 == 'Y')
		{
			finish = 1;
			cout << "Exit! You will be punished!(Lose  " << row * col << " scores!)" << endl;
			this->play->lose_score(row * col);
			return;
		}
		else
		{
			system("clear");
			return;
		}
	}
};

void playMaze(player *p)
{
	system("clear");

	int n = 0, m = 0;
	cout << "please input the row of Maze (11 ~ 50) :>";
	cin >> n;
	while (n < 11 || n > 50)
	{
		system("clear");
		cout << "Invalid Input! the length of row should be in range of 11 to 50!" << endl;
		cout << "Please input again:>";
		cin >> n;
	}
	cout << "please input the col of Maze (11 ~ 50) :>";
	cin >> m;
	while (m < 11 || m > 50)
	{
		system("clear");
		cout << "Invalid Input! the length of column should be in range of 11 to 50!" << endl;
		cout << "Please input again:>";
		cin >> m;
	}
	if (n % 2 == 0)
		n += 1;
	if (m % 2 == 0)
		m += 1;
	MazeGame mg(n, m, p);
	mg.playingmaze();
	mg.MazeBFS();
	while (!mg.finish)
	{
		mg.printMaze();
		mg.Op(getch());
	}
	sleep(3);
	system("clear");
}
