#include "Gamingheader.h"

using namespace std;
// 随机
#define RANDROM(x) rand() % (x) // 随机种子

// 显示图像的预定义
#define Is_Mine '*'     // 这个地方是地雷
#define Is_Secret '"'   // 这个地方还没有被触发
#define Is_NULL '0'     // 周围都没有
#define Is_Selected '_' // 这个地方已经被光标选中了

// 定义广度优先搜索时候的dx, dy
int dx[8] = {1, -1, 0, 0, 1, -1, 1, -1};
int dy[8] = {0, 0, 1, -1, -1, 1, 1, -1};

// 预定义——排查是否访问过该格子
// 广度优先搜索
#define BFS(x, y, queue)                       \
    {                                          \
        Point *pMap = GetPoint(x, y);          \
        if (NULL != pMap)                      \
        {                                      \
            if (false == pMap->vis)            \
            {                                  \
                pMap->vis = true;              \
                if (!pMap->IsMine())           \
                    pMap->ShowReal();          \
                if (Is_NULL == pMap->RealShow) \
                    queue.push(pMap);          \
            }                                  \
        }                                      \
    }


// 每一个格子类
class Point
{
public:
    char CurrentShow;             // 这个格子目前的状态
    char RealShow;                // 这个格子真实的状态
    unsigned short nearMineCount; // 这个格子附近有多少地雷
    bool vis;                     // 有无被BFS过
    int x_point, y_point;         // 这个格子在Map里的坐标定位

public:
    Point()
    {
        CurrentShow = Is_Secret;
        RealShow = Is_NULL;
        vis = false;
        nearMineCount = 0;
        x_point = 0;
        y_point = 0;
    }
    ~Point()
    {
    }

public:
    void SetMine() // 设置这个点是地雷
    {
        RealShow = Is_Mine;
    }
    void SetCurrentShow(char s) // 设置这个点的当前状态
    {
        CurrentShow = s;
    }
    bool IsMine() // 这个点如果是雷，则返回真，反之返回假
    {
        return RealShow == Is_Mine;
    }
    void ShowReal() // 将这个点揭示
    {
        CurrentShow = RealShow;
    }
};

// 地图管理
class GamingMap
{
public:
    vector<vector<Point>> Map; // 地图
    int mapMaxX;               // 长
    int mapMaxY;               // 宽

    int current_x; // 当前位置X
    int current_y; // 当前位置Y

    int MapMineSum; // 地雷数

    player *P; // 玩家

    bool finish = 0; // 判断游戏是否完成
    
public:
    GamingMap(player* play) // 初始化地图+导入玩家
    {
        this->P = play; // 导入玩家身份
        Map.clear();
        mapMaxY = 0;
        mapMaxX = 0;
        current_x = 0;
        current_y = 0;
        MapMineSum = 0;
    }
    ~GamingMap()
    {
    }

public:
    enum show // 枚举类型
    {
        secret, // 还没有被揭示
        real    // 已经被揭示
    };
    // 打印地图
    void PrintMap(int prt)
    {
        system("clear");
        auto it = Map.begin();
        cout << endl;
        for (int y = 0; y < Map.size(); y++)
        {
            vector<Point> &Printing = Map[y];
            for (int x = 0; x < Printing.size(); x++)
            {
                if (secret == prt)
                {
                    if (x == current_x && y == current_y)
                    {
                        cout << Is_Selected;
                    }
                    else
                    {
                        cout << Printing[x].CurrentShow;
                    }
                }
                else if (real == prt)
                {
                    cout << Printing[x].RealShow;
                }
            }
            cout << endl;
        }

        cout << endl;
        cout << "    W" << endl;
        cout << "  A   D  Enter" << endl;
        cout << "    S" << endl;
    }

    // 特定格子附近的地雷数+1
    void AddGridMineCount(int x, int y)
    {
        if (x >= mapMaxX || x < 0)
            return;
        if (y >= mapMaxY || y < 0)
            return;

        Map[y][x].nearMineCount++; // 附近的地雷数量加1
        if (Map[y][x].RealShow != Is_Mine)
            Map[y][x].RealShow = 48 + Map[y][x].nearMineCount;
    }

    // 设置地雷
    void SetMine(int Index)
    { // 我当前设置的这个地雷不能超过地图
        if (Index >= mapMaxX * mapMaxY || Index < 0)
        {
            cout << "Error! The Mine is out of Map!" << endl;
            return;
        }

        int iTargetY = Index / mapMaxX;
        int iTargetX = Index % mapMaxX;

        Map[iTargetY][iTargetX].SetMine();

        // 以当前位置为中心 
        // 开始遍历
        for (int i = 0; i < 8; i++)
            AddGridMineCount(iTargetX + dx[i], iTargetY + dy[i]);
    }

    // 生成地图 iMineCount为生成地雷数
    void GenerateMap(int Mine_sum, int iMaxX, int iMaxY)
    {
        if (iMaxX < 2)
            return;
        if (iMaxY < 2)
            return;
        mapMaxX = iMaxX;
        mapMaxY = iMaxY;

        // 初始化空地图
        vector<Point> vecGrid(iMaxX);
        Map.clear();
        Map.resize(iMaxY, vecGrid);
        for (int y = 0; y < Map.size(); y++)
        {
            for (int x = 0; x < Map[y].size(); x++)
            {
                Map[y][x].x_point = x;
                Map[y][x].y_point = y;
            }
        }

        int Sum_MapCount = mapMaxX * mapMaxY;
        MapMineSum = Mine_sum;

        // 随机算法 生成地雷
        vector<int> vecTmp(Sum_MapCount);
        for (int i = 0; i < Sum_MapCount; i++)
        {
            vecTmp[i] = i; // 赋予初始值
        }

        // 核心算法！ 核心算法！ 核心算法！
        srand((int)time(0));
        int len = Sum_MapCount;
        for (int i = 0; i < Mine_sum; i++)
        {
            // 将抽取到的坐标和最后的坐标交换，然后让vector长度减去1,这样就能做到随机数不重复
            int MineIndex = RANDROM(len); // 随机数
            int Tep = vecTmp[MineIndex];
            vecTmp[MineIndex] = vecTmp[len - 1];
            vecTmp[len - 1] = Tep;
            SetMine(Tep);
            len--;
        }
    }

    void Lose()
    {
        PrintMap(real);
        cout << "game over!!! You lose!!!" << endl;
        cout << "You will lose " << max(MapMineSum * (mapMaxX * mapMaxY) / 10, 5) << " score!" << endl;
        P->lose_score(max(MapMineSum * (mapMaxX * mapMaxY) / 100, 5));
        finish = 1;
        return;
    }
    bool Win()
    {
        // 算出剩余未扫的格子数
        int iDefaultCount = 0;
        for (auto Y : Map) {
            for (auto X : Y) {
                if (X.CurrentShow == Is_Secret)
                    iDefaultCount++;
            }
        }
        return MapMineSum == iDefaultCount; // 地雷的数量 == 没有扫过的数量，就说明剩下的就全部是地雷，就说明扫雷获胜
    }

    void GameWin()
    {
        PrintMap(real);
        cout << "game Win!!!" << endl;
        cout << "You will get " << max(MapMineSum * (mapMaxX * mapMaxY) / 10, 5) << " score!" << endl;
        P->get_score(max(MapMineSum * (mapMaxX * mapMaxY) / 100, 5));
        finish = 1;
        return;
    }

    Point *GetPoint(int x, int y)
    {
        if (x >= mapMaxX || x < 0)
            return NULL;
        if (y >= mapMaxY || y < 0)
            return NULL;
        // 如果这个格子合法，我就把他return
        // 否则我return一个空回去
        return &Map[y][x];
    }

    // 移动距离，判断自己有没有出界
    void Moving(int x, int y)
    {
        if (x >= mapMaxX || x < 0)
            return;
        if (y >= mapMaxY || y < 0)
            return;
        // 如果这个格子合法，我就把他赋值到当前的位置上
        current_x = x;
        current_y = y;
    }

    // 触发扫雷
    void Trigger()
    {
        Point &restPoint = Map[current_y][current_x];
        if (restPoint.CurrentShow == restPoint.RealShow) // 我已经遍历过这个点了
            return;

        // 如果说这个点没有被揭开，并且他是一颗雷
        if (restPoint.IsMine())
        {
            Lose();
            return;
        }

        // 我没有踩雷，那我就需要排查周围有多少雷(3 x 3)
        queue<Point *> quePoint;
        BFS(restPoint.x_point, restPoint.y_point, quePoint);
        while (!quePoint.empty())
        {
            Point *CenterPoint = quePoint.front();
            quePoint.pop();
            for (int i = 0; i < 8; i++)
                BFS(CenterPoint->x_point + dx[i], CenterPoint->y_point + dy[i], quePoint);
        }

        // 打印最新地图
        PrintMap(secret);
        if (Win())
        {
            // 赢了
            GameWin();
            return;
        }
    }

    // 输入操作
    void Op(char op)
    {
        switch (op)
        {
        case 'w':
            Moving(current_x, current_y - 1);
            break;
        case 'a':
            Moving(current_x - 1, current_y);
            break;
        case 's':
            Moving(current_x, current_y + 1);
            break;
        case 'd':
            Moving(current_x + 1, current_y);
            break;
        case '\n':
            Trigger();
            break;
        default:
            cout << "Invaild Input!" << endl;
        }
    }
};

void playMinesweeper(player *p)
{
    system("clear");

    int row = 0, column = 0, mine;
    // 输入行数
    cout << "please input rows: 2~99" << endl;
    cin >> row;
    while (row < 2 || row > 99) {
        cout << "Invalid Input: the row should be in range if (2 - 99)" << endl;
        cout << "Input again :>";
        cin >> row;
    }
    // 输入列数
    cout << "please input columns: 2~99" << endl;
    cin >> column;
    while (column < 2 || column > 99) {
        cout << "Invalid Input: the column should be in range if (2 - 99)" << endl;
        cout << "Input again :>";
        cin >> column;
    }
    // 输入地雷数
    cout << "please input mine count:" << endl;
    cin >> mine;
    if (mine < 0)
        mine = 0;
    while (mine > column * row) {
        cout << "Invalid Input: The number of mines greater than Map!" << endl;
        cout << "Input again :>";
        cin >> mine;
        system("clear");
    }
    GamingMap MinesweeperGame(p);
    
    // 生成地图
    MinesweeperGame.GenerateMap(mine, column, row);

    // 循环
    while (!MinesweeperGame.finish)
    {
        MinesweeperGame.PrintMap(GamingMap::secret);
        MinesweeperGame.Op(getch());
    }
    sleep(3);
}
