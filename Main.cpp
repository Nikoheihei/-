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
void runMultiplayerGame(GameManager& gameManager);
void displayTrajectories(const GameObject& objectA, const GameObject& objectB, const Trajectory& predictedPath, bool isComplexMode);

// 用于控制台可视化的网格大小
const int GRID_SIZE = 20;
const char EMPTY_CELL = '.';
const char ACTUAL_PATH = 'A';
const char RELATIVE_PATH = 'R';
const char PREDICTED_PATH = 'P';
const char OVERLAP_AR = 'C';  // 实际和相对轨迹重叠
const char OVERLAP_AP = 'M';  // 实际和预测轨迹重叠
const char OVERLAP_RP = 'O';  // 相对和预测轨迹重叠
const char OVERLAP_ALL = '*'; // 所有轨迹重叠

// 用于显示轨迹的函数
void displayTrajectories(const GameObject& objectA, const GameObject& objectB, const Trajectory& predictedPath, bool isComplexMode) {
    // 创建一个空的网格
    const int DISPLAY_GRID_SIZE = 40; // 使用更高分辨率的网格
    vector<vector<string>> grid(DISPLAY_GRID_SIZE, vector<string>(DISPLAY_GRID_SIZE, "."));
    
    // 获取轨迹
    const Trajectory& actualTrajectory = objectB.getActualTrajectory();
    const Trajectory& relativeTrajectory = objectB.getRelativeTrajectory();
    
    // 确定坐标范围
    // 坐标范围固定为 -20 到 20
    const double MIN_COORD = -20.0;
    const double MAX_COORD = 20.0;
    const double RANGE = MAX_COORD - MIN_COORD;
    
    // 计算缩放因子，将原始坐标映射到显示网格
    double scale = (DISPLAY_GRID_SIZE - 1) / RANGE;
    
    // 映射函数：将原始坐标映射到显示网格索引
    auto mapToGridIndex = [&](double coord, bool isCol = false) -> int {
        if (isComplexMode && isCol) {
            // 在复杂模式下，列坐标需要考虑六边形网格的特性
            return static_cast<int>((coord * sqrt(3) / 2 - MIN_COORD) * scale);
        }
        return static_cast<int>((coord - MIN_COORD) * scale);
    };
    
    // 在复杂模式下绘制六边形网格参考线
    if (isComplexMode) {
        for (int i = 0; i < DISPLAY_GRID_SIZE; i++) {
            for (int j = 0; j < DISPLAY_GRID_SIZE; j++) {
                // 计算原始坐标
                double row = MIN_COORD + (i / scale);
                double col = MIN_COORD + (j / scale);
                
                // 检查是否在六边形网格的顶点上
                bool isVertex = false;
                if (fmod(abs(row), 1.0) < 0.1 && fmod(abs(col * 2 / sqrt(3)), 1.0) < 0.1) {
                    isVertex = true;
                }
                
                if (isVertex) {
                    grid[i][j] = "+";
                }
            }
        }
    }
    
    // 填充网格 - 实际轨迹 (使用 A0, A1, A2, ...)
    for (size_t i = 0; i < actualTrajectory.getLength(); i++) {
        const GridCell& cell = actualTrajectory.getCell(i);
        int row = mapToGridIndex(cell.getRow());
        int col = mapToGridIndex(cell.getCol(), true);
        
        // 确保在网格范围内
        if (row >= 0 && row < DISPLAY_GRID_SIZE && col >= 0 && col < DISPLAY_GRID_SIZE) {
            string marker = "A" + to_string(i % 10); // 使用数字标记顺序
            grid[row][col] = marker;
        }
    }
    
    // 填充网格 - 相对轨迹 (使用 R0, R1, R2, ...)
    for (size_t i = 0; i < relativeTrajectory.getLength(); i++) {
        const GridCell& cell = relativeTrajectory.getCell(i);
        int row = mapToGridIndex(cell.getRow());
        int col = mapToGridIndex(cell.getCol(), true);
        
        // 确保在网格范围内
        if (row >= 0 && row < DISPLAY_GRID_SIZE && col >= 0 && col < DISPLAY_GRID_SIZE) {
            string marker = "R" + to_string(i % 10);
            // 如果已经有标记，表示重叠
            if (grid[row][col] != "." && grid[row][col] != "+") {
                if (grid[row][col][0] == 'A') {
                    marker = "C" + to_string(i % 10); // A和R重叠
                } else if (grid[row][col][0] == 'P') {
                    marker = "O" + to_string(i % 10); // R和P重叠
                } else if (grid[row][col][0] == 'C') {
                    marker = "*" + to_string(i % 10); // 全部重叠
                }
            }
            grid[row][col] = marker;
        }
    }
    
    // 填充网格 - 预测轨迹 (使用 P0, P1, P2, ...)
    for (size_t i = 0; i < predictedPath.getLength(); i++) {
        const GridCell& cell = predictedPath.getCell(i);
        int row = mapToGridIndex(cell.getRow());
        int col = mapToGridIndex(cell.getCol(), true);
        
        // 确保在网格范围内
        if (row >= 0 && row < DISPLAY_GRID_SIZE && col >= 0 && col < DISPLAY_GRID_SIZE) {
            string marker = "P" + to_string(i % 10);
            // 处理重叠情况
            if (grid[row][col] != "." && grid[row][col] != "+") {
                if (grid[row][col][0] == 'A') {
                    marker = "M" + to_string(i % 10); // A和P重叠
                } else if (grid[row][col][0] == 'R') {
                    marker = "O" + to_string(i % 10); // R和P重叠
                } else if (grid[row][col][0] == 'C') {
                    marker = "*" + to_string(i % 10); // 全部重叠
                }
            }
            grid[row][col] = marker;
        }
    }
    
    // 打印网格
    cout << "\n轨迹可视化表示：" << endl;
    cout << "A0-A9 - 实际轨迹，R0-R9 - 相对轨迹，P0-P9 - 预测轨迹" << endl;
    cout << "C - 实际和相对重叠，M - 实际和预测重叠，O - 相对和预测重叠，* - 全部重叠" << endl;
    if (isComplexMode) {
        cout << "+ - 六边形网格顶点" << endl;
    }
    cout << "数字表示轨迹中点的顺序 (0-9循环)" << endl;
    
    // 打印坐标轴标签
    cout << "\n坐标范围：X轴和Y轴从 " << MIN_COORD << " 到 " << MAX_COORD << endl;
    
    // 打印列标题
    cout << "\n    ";
    for (int j = 0; j < DISPLAY_GRID_SIZE; j += 5) {
        double originalCoord = MIN_COORD + (j / scale);
        cout << setw(5) << fixed << setprecision(1) << originalCoord;
    }
    cout << endl;
    
    // 打印网格内容
    for (int i = 0; i < DISPLAY_GRID_SIZE; i++) {
        if (i % 5 == 0) {
            double originalCoord = MIN_COORD + (i / scale);
            cout << setw(3) << fixed << setprecision(1) << originalCoord << " ";
        } else {
            cout << "    ";
        }
        
        for (int j = 0; j < DISPLAY_GRID_SIZE; j++) {
            cout << setw(2) << grid[i][j] << " ";
        }
        cout << endl;
    }
    
    // 打印轨迹点的详细信息
    cout << "\n轨迹点详细信息：" << endl;
    
    if (actualTrajectory.getLength() > 0) {
        cout << "实际轨迹 (" << actualTrajectory.getLength() << "点):" << endl;
        for (size_t i = 0; i < actualTrajectory.getLength(); i++) {
            const GridCell& cell = actualTrajectory.getCell(i);
            cout << "  点" << i << ": 原始坐标(" 
                 << fixed << setprecision(2) << cell.getRow() << "," << cell.getCol() << ")" << endl;
        }
    }
    
    if (relativeTrajectory.getLength() > 0) {
        cout << "相对轨迹 (" << relativeTrajectory.getLength() << "点):" << endl;
        for (size_t i = 0; i < relativeTrajectory.getLength(); i++) {
            const GridCell& cell = relativeTrajectory.getCell(i);
            cout << "  点" << i << ": 原始坐标(" 
                 << fixed << setprecision(2) << cell.getRow() << "," << cell.getCol() << ")" << endl;
        }
    }
    
    if (predictedPath.getLength() > 0) {
        cout << "预测轨迹 (" << predictedPath.getLength() << "点):" << endl;
        for (size_t i = 0; i < predictedPath.getLength(); i++) {
            const GridCell& cell = predictedPath.getCell(i);
            cout << "  点" << i << ": 原始坐标(" 
                 << fixed << setprecision(2) << cell.getRow() << "," << cell.getCol() << ")" << endl;
        }
    }
    
    // 打印连续性分析
    cout << "\n轨迹连续性分析：" << endl;
    
    auto analyzeContinuity = [isComplexMode](const Trajectory& traj, const string& name) {
        if (traj.getLength() < 2) {
            cout << name << "轨迹点数不足，无法分析连续性。" << endl;
            return;
        }
        
        cout << name << "轨迹连续性：" << endl;
        int discontinuities = 0;
        
        for (size_t i = 1; i < traj.getLength(); i++) {
            const GridCell& prev = traj.getCell(i-1);
            const GridCell& curr = traj.getCell(i);
            
            double rowDiff = abs(curr.getRow() - prev.getRow());
            double colDiff = abs(curr.getCol() - prev.getCol());
            
            // 在复杂模式下使用六边形网格的距离计算
            if (isComplexMode) {
                double hexDistance = sqrt(pow(rowDiff, 2) + pow(colDiff * sqrt(3) / 2, 2));
                if (hexDistance > 1.1) {
                    cout << "  发现不连续点：点" << (i-1) << "与点" << i 
                         << " 相差 (行=" << rowDiff << ", 列=" << colDiff << ")" << endl;
                    discontinuities++;
                }
            } else {
                if (rowDiff > 1.1 || colDiff > 1.1) {
                    cout << "  发现不连续点：点" << (i-1) << "与点" << i 
                         << " 相差 (行=" << rowDiff << ", 列=" << colDiff << ")" << endl;
                    discontinuities++;
                }
            }
        }
        
        if (discontinuities == 0) {
            cout << "  轨迹完全连续" << endl;
        } else {
            cout << "  发现 " << discontinuities << " 处不连续点" << endl;
        }
    };
    
    analyzeContinuity(actualTrajectory, "实际");
    analyzeContinuity(relativeTrajectory, "相对");
    if (predictedPath.getLength() > 0) {
        analyzeContinuity(predictedPath, "预测");
    }
}

// 手动输入预测轨迹
Trajectory inputPrediction(int steps, bool isComplexMode) {
    Trajectory prediction;
    cout << "请输入预测轨迹（" << steps << "步）" << endl;
    
    // 输入起始点
    double startRow, startCol;
    cout << "请输入起始点行坐标（相对于中心0）: ";
    cin >> startRow;
    cout << "请输入起始点列坐标（相对于中心0）: ";
    cin >> startCol;
    
    GridCell startCell(startRow, startCol);
    prediction.addCell(startCell);
    
    // 输入每一步的方向
    for (int i = 0; i < steps - 1; i++) {
        int direction;
        if (isComplexMode) {
            cout << "步骤 " << (i+1) << " 的方向（1-右, 2-右上, 3-左上, 4-左, 5-左下, 6-右下）: ";
            cin >> direction;
            
            // 获取上一个单元格
            const GridCell& lastCell = prediction.getCurrentCell();
            double row = lastCell.getRow();
            double col = lastCell.getCol();
            
            GridCell newCell(row, col);
            
            // 根据方向更新位置
            switch (direction) {
                case 1: // 右
                    newCell = GridCell(row, col + 1);
                    break;
                case 2: // 右上
                    newCell = GridCell(row - 0.5, col + 0.866);
                    break;
                case 3: // 左上
                    newCell = GridCell(row - 0.5, col - 0.866);
                    break;
                case 4: // 左
                    newCell = GridCell(row, col - 1);
                    break;
                case 5: // 左下
                    newCell = GridCell(row + 0.5, col - 0.866);
                    break;
                case 6: // 右下
                    newCell = GridCell(row + 0.5, col + 0.866);
                    break;
                default:
                    cout << "无效方向，默认向右移动" << endl;
                    newCell = GridCell(row, col + 1);
                    break;
            }
            prediction.addCell(newCell);
        } else {
            cout << "步骤 " << (i+1) << " 的方向（1-右, 2-上, 3-左, 4-下）: ";
            cin >> direction;
            
            // 获取上一个单元格
            const GridCell& lastCell = prediction.getCurrentCell();
            double row = lastCell.getRow();
            double col = lastCell.getCol();
            
            GridCell newCell(row, col);
            
            // 根据方向更新位置
            switch (direction) {
                case 1: // 右
                    newCell = GridCell(row, col + 1);
                    break;
                case 2: // 上
                    newCell = GridCell(row - 1, col);
                    break;
                case 3: // 左
                    newCell = GridCell(row, col - 1);
                    break;
                case 4: // 下
                    newCell = GridCell(row + 1, col);
                    break;
                default:
                    cout << "无效方向，默认向右移动" << endl;
                    newCell = GridCell(row, col + 1);
                    break;
            }
            prediction.addCell(newCell);
        }
    }
    
    return prediction;
}

// 运行单人游戏
void runSinglePlayerGame(GameManager& gameManager) {
    bool isComplexMode = gameManager.isComplexMode();
    cout << "\n=== " << (isComplexMode ? "复杂" : "简单") << "单人游戏开始 ===" << endl;
    cout << "游戏原理：电脑生成一个轨迹A和一个轨迹B相对A的相对路径" << endl;
    cout << "玩家任务是预测轨迹B在实际坐标系中的运动路径\n" << endl;
    
    // 获取游戏对象
    const GameObject& objectA = gameManager.getObjectA();
    const GameObject& objectB = gameManager.getObjectB();
    
    // 显示初始轨迹
    Trajectory emptyPrediction; // 创建一个空的预测轨迹用于初始显示
    cout << "\n初始轨迹：" << endl;
    cout << "A - 参考轨迹(蓝色物体的运动)" << endl;
    cout << "R - 相对轨迹(红色物体相对于蓝色物体的运动)" << endl;
    displayTrajectories(objectA, objectB, emptyPrediction, isComplexMode);
    
    // 向玩家解释预测任务
    cout << "\n请预测红色物体在实际坐标系中的运动轨迹" << endl;
    cout << "应用公式：t+1时刻红色物体位置 = t+1时刻蓝色物体位置 + (t时刻红色物体相对位置 - t+1时刻红色物体相对位置)" << endl;
    
    // 让用户输入预测轨迹
    int steps = objectB.getActualTrajectory().getLength(); // 使用实际轨迹的长度
    if (steps <= 0) steps = 10; // 默认步数
    
    Trajectory userPrediction = inputPrediction(steps, isComplexMode);
    
    // 显示包含用户预测的轨迹
    cout << "\n带预测的轨迹：" << endl;
    cout << "A - 参考轨迹，R - 相对轨迹，P - 预测轨迹" << endl;
    displayTrajectories(objectA, objectB, userPrediction, isComplexMode);
    
    // 设置玩家的预测并提交
    Player& currentPlayer = gameManager.getCurrentPlayer();
    currentPlayer.setPrediction(userPrediction);
    gameManager.submitPrediction(currentPlayer.getPlayerID());
    
    // 显示结果及实际轨迹对比
    cout << "\n游戏结果：" << endl;
    cout << "P - 您的预测轨迹" << endl;
    cout << "A - 物体实际轨迹" << endl;
    cout << "M - 预测和实际重叠部分" << endl;
    
    // 输出评分
    cout << "得分: " << currentPlayer.getCurrentScore() << endl;
    double similarity = userPrediction.calculateSimilarity(objectB.getActualTrajectory());
    cout << "相似度: " << similarity * 100 << "%" << endl;
}

int main() {
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
    
    while (true) {
        cout << "\n=== 主菜单 ===" << endl;
        cout << "1. 注册新用户" << endl;
        cout << "2. 用户登录" << endl;
        cout << "3. 开始游戏" << endl;
        cout << "4. 退出" << endl;
        cout << "请选择: ";
        cin >> choice;
        
        switch (choice) {
            case 1: // 注册新用户
                cout << "请输入用户名: ";
                cin >> username;
                if (gameManager.registerUser(username)) {
                    cout << "注册成功!" << endl;
                } else {
                    cout << "用户名已存在!" << endl;
                }
                break;
                
            case 2: // 用户登录
                cout << "请输入用户名: ";
                cin >> username;
                if (gameManager.loginUser(username)) {
                    cout << "登录成功!" << endl;
                } else {
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
                switch (gameMode) {
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
void runMultiplayerGame(GameManager& gameManager) {
    bool isComplexMode = gameManager.isComplexMode();
    cout << "\n=== " << (isComplexMode ? "复杂" : "简单") << "多人游戏开始 ===" << endl;
    cout << "游戏将进行 " << gameManager.getTotalRounds() << " 回合" << endl;
    
    while (!gameManager.isGameOver()) {
        cout << "\n=== 第 " << gameManager.getCurrentRound() << " 回合 ===" << endl;
        cout << "当前玩家: 玩家" << (gameManager.getCurrentPlayer().getPlayerID() + 1) << endl;
        
        runSinglePlayerGame(gameManager);
        
        // 显示当前回合所有玩家的得分
        cout << "\n当前回合得分：" << endl;
        for (const auto& player : gameManager.getPlayers()) {
            cout << "玩家" << (player.getPlayerID() + 1) << ": " << player.getCurrentScore() << endl;
        }
    }
    
    // 显示最终结果
    cout << "\n=== 游戏结束 ===" << endl;
    cout << "最终得分：" << endl;
    double maxScore = 0;
    int winnerID = -1;
    
    for (const auto& player : gameManager.getPlayers()) {
        double score = player.getCurrentScore();
        cout << "玩家" << (player.getPlayerID() + 1) << ": " << score << endl;
        
        if (score > maxScore) {
            maxScore = score;
            winnerID = player.getPlayerID();
        }
    }
    
    if (winnerID != -1) {
        cout << "\n获胜者: 玩家" << (winnerID + 1) << "!" << endl;
    } else {
        cout << "\n平局!" << endl;
    }
} 