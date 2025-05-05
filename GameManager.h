#pragma once
#include "GameObject.h"
#include "Player.h"
#include <vector>
#include <map>
#include <string>

class GameManager {
public:
    // 游戏模式枚举
    enum GameMode { 
        SIMPLE_SINGLE,  // 简单单人模式
        COMPLEX_SINGLE, // 复杂单人模式
        SIMPLE_MULTI,   // 简单多人模式
        COMPLEX_MULTI   // 复杂多人模式
    };

private:
    GameObject objectA;
    GameObject objectB;
    std::vector<Player> players;
    int currentPlayerIndex;
    GameMode currentGameMode;
    std::map<std::string, Player> userDatabase;
    std::string loggedInUsername;
    
    // 游戏配置
    int gameSteps;      // 轨迹的步数
    bool gameRunning;   // 游戏运行状态
    bool roundComplete; // 当前回合是否完成
    int currentRound;   // 当前回合数
    int totalRounds;    // 总回合数

    // 轨迹步数范围配置
    static const int MIN_SIMPLE_STEPS = 6;   // 简单模式最小步数
    static const int MAX_SIMPLE_STEPS = 25;  // 简单模式最大步数
    static const int MIN_COMPLEX_STEPS = 4;  // 复杂模式最小步数
    static const int MAX_COMPLEX_STEPS = 15;  // 复杂模式最大步数

public:
    // 构造函数
    GameManager();
    
    // 初始化游戏
    void initializeGame(GameMode mode);
    
    // 从文件加载用户信息
    void loadUserData();
    
    // 生成游戏数据（A和B的轨迹等）
    void generateGameData();

    // 设置轨迹步数
    void setGameSteps(int steps);
    
    // 获取轨迹步数
    int getGameSteps() const {
        return gameSteps;
    }
    
    // 获取轨迹步数范围
    int getMinSteps() const {
        return isComplexMode() ? MIN_COMPLEX_STEPS : MIN_SIMPLE_STEPS;
    }
    
    int getMaxSteps() const {
        return isComplexMode() ? MAX_COMPLEX_STEPS : MAX_SIMPLE_STEPS;
    }
    
    // 处理用户输入
    void handleUserInput(/* 可以根据实际情况添加参数 */);
    
    // 更新游戏状态
    void updateGame();
    
    // 渲染游戏
    void renderGame();
    
    // 提交玩家预测
    void submitPrediction(int playerID);
    
    // 计算游戏结果
    void calculateResults();
    
    // 切换当前玩家（多人模式）
    void switchPlayer();
    
    // 将用户信息保存到文件
    void saveUserData();
    
    // 注册新用户
    bool registerUser(const std::string& username);
    
    // 用户登录
    bool loginUser(const std::string& username);
    
    // 更新并保存最高分
    void updateAndSaveHighScore(int playerID);
    
    // 获取当前游戏模式
    GameMode getCurrentGameMode() const;
    
    // 获取当前玩家
    Player& getCurrentPlayer();
    
    // 获取ObjectA
    const GameObject& getObjectA() const;
    
    // 获取ObjectB
    const GameObject& getObjectB() const;
    
    // 检查游戏是否在运行
    bool isGameRunning() const;
    
    // 检查当前回合是否完成
    bool isRoundComplete() const;
    
    // 判断是否为复杂模式
    bool isComplexMode() const {
        return currentGameMode == COMPLEX_SINGLE || currentGameMode == COMPLEX_MULTI;
    }
    
    // 判断是否为多人模式
    bool isMultiplayerMode() const {
        return currentGameMode == SIMPLE_MULTI || currentGameMode == COMPLEX_MULTI;
    }
    
    // 获取当前回合数
    int getCurrentRound() const {
        return currentRound;
    }
    
    // 获取总回合数
    int getTotalRounds() const {
        return totalRounds;
    }
    
    // 检查游戏是否结束
    bool isGameOver() const {
        return currentRound > totalRounds;
    }
    
    // 开始新回合
    void startNewRound();
    
    // 获取所有玩家
    const std::vector<Player>& getPlayers() const {
        return players;
    }
}; 