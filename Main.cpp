#include "GameManager.h"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>
#if defined(_WIN32)
#include <windows.h>
#endif
using namespace std;

// 函数声明
void runMultiplayerGame(GameManager &gameManager);
void displayTrajectories(const GameObject &objectA, const GameObject &objectB, const Trajectory &predictedPath, bool isComplexMode);

// 用于控制台可视化的网格大小
const int GRID_SIZE = 61;
const char EMPTY_CELL = '.';
const char ACTUAL_PATH = 'A';
const char RELATIVE_PATH = 'R';
const char PREDICTED_PATH = 'P';
const char OVERLAP_AR = 'C';  // 实际和相对轨迹重叠
const char OVERLAP_AP = 'M';  // 实际和预测轨迹重叠
const char OVERLAP_RP = 'O';  // 相对和预测轨迹重叠
const char OVERLAP_ALL = '*'; // 所有轨迹重叠
const int step = 5;
const int MIN_GRID_COORD = -30;
const int MAX_GRID_COORD = 30;
const int MIN_TRAJ_COORD = -15;
const int MAX_TRAJ_COORD = 15;

const vector<GridCell> hex_point = {
    GridCell(0, -2),
    GridCell(0, 2),
    GridCell(-1, 1),
    GridCell(-1, -1),
    GridCell(1, 1),
    GridCell(1, -1),
    GridCell(-1, 0),
    GridCell(1, 0)};
const vector<GridCell> four_directions = {
    GridCell(-1, 0),
    GridCell(0, 1),
    GridCell(0, -1),
    GridCell(1, 0)

};
const vector<GridCell> hex_directions = {
    GridCell(-2, 0),
    GridCell(-1, -3),
    GridCell(1, -3),
    GridCell(2, 0),
    GridCell(1, 3),
    GridCell(-1, 3)};
const int UP = 0;
const int RIGHT = 1;
const int LEFT = 2;
const int DOWN = 3;
const int UP_RIGHT = 5;
const int DOWN_RIGHT = 4;
const int DOWN_LEFT = 2;
const int UP_LEFT = 1;

// 用于显示轨迹的函数
void displayTrajectories(const GameObject &objectA, const Trajectory &predictedPath, bool isComplexMode)
{
    // 创建一个空的网格
    vector<vector<string>> grid(GRID_SIZE, vector<string>(GRID_SIZE, "."));

    // 获取轨迹
    const Trajectory &actualTrajectory = objectA.getActualTrajectory();
    const Trajectory &relativeTrajectory = objectA.getRelativeTrajectory();
    const Trajectory &finalTrajectory = objectA.getfinalTrajectory();

    // 确定网格范围 -30到30
    const int MIN_COORD = MIN_GRID_COORD;
    const int MAX_COORD = MAX_GRID_COORD;
    const int OFFSET = MAX_COORD; // 用于将坐标转换为索引的偏移量

    // 实际上可视化是靠一个二维vector存储得来的

    // 填充网格 - 实际轨迹 (使用 A0, A1, A2, ...)
    for (size_t i = 0; i < actualTrajectory.getLength(); i++)
    {
        const GridCell &cell = actualTrajectory.getCell(i);
        int row = cell.getRow() + OFFSET;
        int col = cell.getCol() + OFFSET;

        // 确保在网格范围内
        if (!isComplexMode)
        {
            if (row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE)
            {
                string marker = "A" + to_string(i % 10); // 使用数字标记顺序
                grid[row][col] = marker;
            }
        }
        else
        {
            if (row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE)
            {
                string marker = "A" + to_string(i % 10); // 使用数字标记顺序
                for (int j = 0; j < 8; j++)
                {
                    int gridX = row + hex_point[j].getRow();
                    int gridY = col + hex_point[j].getCol();
                    if (gridX >= 0 && gridX < GRID_SIZE && gridY >= 0 && gridY < GRID_SIZE)
                    {
                        grid[gridX][gridY] = "#";
                    }
                }
                grid[row][col] = marker;
            }
        }
    }

    // 填充网格 - 相对轨迹 (使用 R0, R1, R2, ...)
    for (size_t i = 0; i < relativeTrajectory.getLength(); i++)
    {
        const GridCell &cell = relativeTrajectory.getCell(i);
        int row = cell.getRow() + OFFSET;
        int col = cell.getCol() + OFFSET;

        // 确保在网格范围内
        if (row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE)
        {
            string marker = "R" + to_string(i % 10);
            // 如果已经有标记，表示重叠
            if (grid[row][col] != "." && grid[row][col] != "+")
            {
                if (grid[row][col][0] == 'A')
                {
                    marker = "C" + to_string(i % 10); // A和R重叠
                }
                else if (grid[row][col][0] == 'P')
                {
                    marker = "O" + to_string(i % 10); // R和P重叠
                }
                else if (grid[row][col][0] == 'C')
                {
                    marker = "*" + to_string(i % 10); // 全部重叠
                }
            }

            if (isComplexMode)
            {
                for (int j = 0; j < 8; j++)
                {
                    int gridX = row + hex_point[j].getRow();
                    int gridY = col + hex_point[j].getCol();
                    if (gridX >= 0 && gridX < GRID_SIZE && gridY >= 0 && gridY < GRID_SIZE)
                    {
                        grid[gridX][gridY] = "&";
                    }
                }
            }
            grid[row][col] = marker;
        }
    }

    // 填充网格 - 预测轨迹 (使用 P0, P1, P2, ...)
    for (size_t i = 0; i < predictedPath.getLength(); i++)
    {
        const GridCell &cell = predictedPath.getCell(i);
        // 添加安全检查，确保i不超过finalTrajectory的长度
        bool canCompareWithWishCell = (i < finalTrajectory.getLength());
        int row = cell.getRow() + OFFSET;
        int col = cell.getCol() + OFFSET;

        // 确保在网格范围内
        if (row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE)
        {
            string marker = "P" + to_string(i % 10);
            // 处理重叠情况
            if (grid[row][col] != ".")
            {
                if (grid[row][col][0] == 'A')
                {
                    marker = "M" + to_string(i % 10); // A和P重叠
                }
                else if (grid[row][col][0] == 'R')
                {
                    marker = "O" + to_string(i % 10); // R和P重叠
                }
                else if (grid[row][col][0] == 'C')
                {
                    marker = "*" + to_string(i % 10); // 全部重叠
                }
            }
            
            // 只有当finalTrajectory有足够长度时才比较
            if (canCompareWithWishCell) {
                const GridCell &wishcell = finalTrajectory.getCell(i);
                if (cell == wishcell)
                    marker = '@';
            }
            
            if (isComplexMode)
            {
                for (int j = 0; j < 8; j++)
                {
                    int gridX = row + hex_point[j].getRow();
                    int gridY = col + hex_point[j].getCol();
                    if (gridX >= 0 && gridX < GRID_SIZE && gridY >= 0 && gridY < GRID_SIZE)
                    {
                        grid[gridX][gridY] = "&";
                    }
                }
            }
            grid[row][col] = marker;
        }
    }

    // 打印网格
    cout << "\n轨迹可视化表示：" << endl;
    cout << "A0-A9 - 实际轨迹，R0-R9 - 相对轨迹，P0-P9 - 预测轨迹" << endl;
    cout << "C - 实际和相对重叠，M - 实际和预测重叠，O - 相对和预测重叠，* - 全部重叠" << endl;

    cout << "数字表示轨迹中点的顺序 (0-9循环)" << endl;

    // 打印坐标轴标签
    cout << "\n坐标范围：X轴和Y轴从 " << MIN_COORD << " 到 " << MAX_COORD << endl;

    // 打印列标题
    cout << "\n    ";
    for (int j = 0; j < GRID_SIZE; j += 5)
    {
        int originalCoord = MIN_GRID_COORD + (j);
        cout << setw(5) << fixed << setprecision(1) << originalCoord;
    }
    cout << endl;

    // 打印网格内容
    for (int i = 0; i < GRID_SIZE; i++)
    {
        if (i % 5 == 0)
        {
            int originalCoord = MIN_GRID_COORD + (i);
            cout << setw(3) << originalCoord << "   ";
        }
        else
        {
            cout << "      ";
        }

        for (int j = 0; j < GRID_SIZE; j++)
        {
            cout << setw(2) << grid[i][j] << " ";
        }
        cout << endl;
    }

    // 打印轨迹点的详细信息
    cout << "\n轨迹点详细信息：" << endl;

    if (actualTrajectory.getLength() > 0)
    {
        cout << "实际轨迹 (" << actualTrajectory.getLength() << "点):" << endl;
        for (size_t i = 0; i < actualTrajectory.getLength(); i++)
        {
            const GridCell &cell = actualTrajectory.getCell(i);
            cout << "  点" << i << ": 原始坐标("
                 << cell.getRow() << "," << cell.getCol() << ")" << endl;
        }
    }

    if (relativeTrajectory.getLength() > 0)
    {
        cout << "相对轨迹 (" << relativeTrajectory.getLength() << "点):" << endl;
        for (size_t i = 0; i < relativeTrajectory.getLength(); i++)
        {
            const GridCell &cell = relativeTrajectory.getCell(i);
            cout << "  点" << i << ": 原始坐标("
                 << cell.getRow() << "," << cell.getCol() << ")" << endl;
        }
    }

    if (finalTrajectory.getLength() > 0)
    {
        cout << "实际轨迹' (" << finalTrajectory.getLength() << "点):" << endl;
        for (size_t i = 0; i < finalTrajectory.getLength(); i++)
        {
            const GridCell &cell = finalTrajectory.getCell(i);
            cout << "  点" << i << ": 原始坐标("
                 << fixed << setprecision(2) << cell.getRow() << "," << cell.getCol() << ")" << endl;
        }
    }
}

// 手动输入预测轨迹
Trajectory inputPrediction(const GameObject &objectA, int steps, bool isComplexMode)
{
    Trajectory prediction;
    const Trajectory &finalTrajectory = objectA.getfinalTrajectory();
    int finalLength = finalTrajectory.getLength();
    
    // 确保steps与finalTrajectory的长度一致
    if (steps > finalLength - 1) {
        std::cout << "警告: 预测轨迹步数已调整为" << (finalLength - 1) << "步，以匹配实际轨迹' 的长度" << std::endl;
        steps = finalLength - 1;
    }
    
    std::cout << "请输入预测轨迹（" << steps << "步）" << std::endl;

    // 预测轨迹的起始点要求和电脑通过actualTrajectory和
    // RelativeTrajectory计算得出的finalTrajectory的起始点一致。
    prediction.addCell(finalTrajectory.getCell(0));
    cout << "起始点行坐标（相对于中心0）：" << prediction.getCurrentCell().getRow() << endl;
    cout << "起始点列坐标（相对于中心0）：" << prediction.getCurrentCell().getCol() << endl;

    // 输入每一步的方向
    for (int i = 0; i < steps; i++)
    {
        int x, y;
        cout << "步骤 " << (i + 1) << endl;
        cout << "请输入下一个位置的行坐标：";
        cin >> x;
        cout << "请输入下一个位置的列坐标：";
        cin >> y;

        GridCell newCell(x, y);
        prediction.addCell(newCell);
    }
    return prediction;
}

// 运行单人游戏
void runSinglePlayerGame(GameManager &gameManager)
{
    bool isComplexMode = gameManager.isComplexMode();
    bool isMultiplayerMode = gameManager.isMultiplayerMode();
    
    // 仅在单人模式下提示设置轨迹步数
    if (!isMultiplayerMode) {
        // 提示玩家输入轨迹步数
        int steps;
        int minSteps = gameManager.getMinSteps();
        int maxSteps = gameManager.getMaxSteps();
        
        cout << "\n您希望生成的轨迹数目 (" << minSteps << "-" << maxSteps-1 << "): ";
        cin >> steps;
        
        // 设置轨迹步数
        gameManager.setGameSteps(steps);
        
        // 生成游戏数据
        gameManager.generateGameData();
    }
    
    cout << "\n=== " << (isComplexMode ? "复杂" : "简单") << "单人游戏开始 ===" << endl;
    cout << "游戏原理：电脑生成一个轨迹A和一个轨迹B相对A的相对路径" << endl;
    cout << "玩家任务是预测轨迹B在实际坐标系中的运动路径\n"
         << endl;

    // 获取游戏对象
    const GameObject &objectA = gameManager.getObjectA();

    // 显示初始轨迹
    Trajectory emptyPrediction; // 创建一个空的预测轨迹用于初始显示
    cout << "\n初始轨迹：" << endl;
    cout << "A - 参考轨迹(蓝色物体的运动)" << endl;
    cout << "R - 相对轨迹(红色物体相对于蓝色物体的运动)" << endl;

    displayTrajectories(objectA, emptyPrediction, isComplexMode);

    // 向玩家解释预测任务
    cout << "\n请预测红色物体在实际坐标系中的运动轨迹" << endl;

    // 让用户输入预测轨迹
    int predictionSteps = gameManager.getGameSteps(); // 使用设置的步数
    Trajectory userPrediction = inputPrediction(objectA, predictionSteps, isComplexMode);

    // 显示包含用户预测的轨迹
    cout << "\n带预测的轨迹：" << endl;
    cout << "A - 参考轨迹，R - 相对轨迹，P - 预测轨迹" << endl;
    displayTrajectories(objectA, userPrediction, isComplexMode);

    // 设置玩家的预测并提交
    Player &currentPlayer = gameManager.getCurrentPlayer();
    currentPlayer.setPrediction(userPrediction);
    gameManager.submitPrediction(currentPlayer.getPlayerID());

    // 显示结果及实际轨迹对比
    cout << "\n游戏结果：" << endl;
    cout << "P - 您的预测轨迹" << endl;
    cout << "A - 物体实际轨迹" << endl;
    cout << "M - 预测和实际重叠部分" << endl;

    // 输出评分
    cout << "得分: " << currentPlayer.getCurrentScore() << endl;
    double similarity = userPrediction.calculateSimilarity(objectA.getfinalTrajectory());
    cout << "相似度: " << similarity * 100 << "%" << endl;
}

int main()
{
#if defined(_WIN32)
    // 切换控制台到 UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    // 创建游戏管理器实例
    GameManager gameManager;

    // 简单的控制台菜单，在实际图形界面中可能会有所不同
    int choice = 0;
    string username;

    cout << "欢迎来到轨迹预测游戏!" << endl;

    while (true)
    {
        cout << "\n=== 主菜单 ===" << endl;
        cout << "1. 注册新用户" << endl;
        cout << "2. 用户登录" << endl;
        cout << "3. 开始游戏" << endl;
        cout << "4. 退出" << endl;
        cout << "请选择: ";
        cin >> choice;

        switch (choice)
        {
        case 1: // 注册新用户
            cout << "请输入用户名: ";
            cin >> username;
            if (gameManager.registerUser(username))
            {
                cout << "注册成功!" << endl;
            }
            else
            {
                cout << "用户名已存在!" << endl;
            }
            break;

        case 2: // 用户登录
            cout << "请输入用户名: ";
            cin >> username;
            if (gameManager.loginUser(username))
            {
                cout << "登录成功!" << endl;
            }
            else
            {
                cout << "用户名不存在!" << endl;
            }
            break;

        case 3: // 开始游戏
            int gameMode;
            cout << "\n=== 选择游戏模式 ===" << endl;
            cout << "1. 简单单人模式" << endl;
            cout << "2. 复杂单人模式" << endl;
            cout << "3. 简单多人模式" << endl;
            cout << "4. 复杂多人模式" << endl;
            cout << "请选择: ";
            cin >> gameMode;

            // 初始化游戏
            switch (gameMode)
            {
            case 1:
                gameManager.initializeGame(GameManager::SIMPLE_SINGLE);
                runSinglePlayerGame(gameManager);
                break;
            case 2:
                gameManager.initializeGame(GameManager::COMPLEX_SINGLE);
                runSinglePlayerGame(gameManager);
                break;
            case 3:
                gameManager.initializeGame(GameManager::SIMPLE_MULTI);
                runMultiplayerGame(gameManager);
                break;
            case 4:
                gameManager.initializeGame(GameManager::COMPLEX_MULTI);
                runMultiplayerGame(gameManager);
                break;
            default:
                cout << "无效选择!" << endl;
                continue;
            }

            break;

        case 4: // 退出
            cout << "谢谢使用，再见!" << endl;
            return 0;

        default:
            cout << "无效选择，请重试!" << endl;
        }
    }

    return 0;
}

// 运行多人游戏
void runMultiplayerGame(GameManager &gameManager)
{
    bool isComplexMode = gameManager.isComplexMode();
    
    // 提示玩家输入轨迹步数
    int steps;
    int minSteps = gameManager.getMinSteps();
    int maxSteps = gameManager.getMaxSteps();
    
    cout << "\n您希望生成的轨迹数目 (" << minSteps << "-" << maxSteps-1 << "): ";
    cin >> steps;
    
    // 设置轨迹步数
    gameManager.setGameSteps(steps);
    
    // 生成游戏数据
    gameManager.generateGameData();
    
    cout << "\n=== " << (isComplexMode ? "复杂" : "简单") << "多人游戏开始 ===" << endl;
    cout << "游戏将进行 " << gameManager.getTotalRounds() << " 回合" << endl;

    while (!gameManager.isGameOver())
    {
        cout << "\n=== 第 " << gameManager.getCurrentRound() << " 回合 ===" << endl;
        cout << "当前玩家: 玩家" << (gameManager.getCurrentPlayer().getPlayerID() + 1) << endl;

        // 由于这里会再次询问轨迹数目，所以需要修改runSinglePlayerGame来处理多人游戏模式的特例
        runSinglePlayerGame(gameManager);

        // 显示当前回合所有玩家的得分
        cout << "\n当前回合得分：" << endl;
        for (const auto &player : gameManager.getPlayers())
        {
            cout << "玩家" << (player.getPlayerID() + 1) << ": " << player.getCurrentScore() << endl;
        }
    }

    // 显示最终结果
    cout << "\n=== 游戏结束 ===" << endl;
    cout << "最终得分：" << endl;
    double maxScore = 0;
    int winnerID = -1;

    for (const auto &player : gameManager.getPlayers())
    {
        double score = player.getCurrentScore();
        cout << "玩家" << (player.getPlayerID() + 1) << ": " << score << endl;

        if (score > maxScore)
        {
            maxScore = score;
            winnerID = player.getPlayerID();
        }
    }

    if (winnerID != -1)
    {
        cout << "\n获胜者: 玩家" << (winnerID + 1) << "!" << endl;
    }
    else
    {
        cout << "\n平局!" << endl;
    }
}
