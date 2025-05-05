#include "GameManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>
const int step=5;

GameManager::GameManager() 
    : currentPlayerIndex(0), currentGameMode(SIMPLE_SINGLE), 
      gameSteps(step), gameRunning(false), roundComplete(false),
      currentRound(1), totalRounds(5) {
    // 初始化GameManager对象
    // 加载用户数据
    loadUserData();
}

void GameManager::setGameSteps(int steps) {
    // 根据游戏模式确定步数范围
    int minSteps = getMinSteps();
    int maxSteps = getMaxSteps();
    
    // 确保步数在有效范围内
    if (steps < minSteps) {
        gameSteps = minSteps;
    } else if (steps > maxSteps) {
        gameSteps = maxSteps;
    } else {
        gameSteps = steps;
    }
}

void GameManager::initializeGame(GameMode mode) {
    // 初始化游戏并设置游戏模式
    currentGameMode = mode;
    currentPlayerIndex = 0;
    gameRunning = true;
    roundComplete = false;
    currentRound = 1;
    
    // 根据游戏模式设置玩家数量
    players.clear();
    if (mode == SIMPLE_SINGLE || mode == COMPLEX_SINGLE) {
        // 单人模式只有一个玩家
        players.push_back(Player(0));
    } else {
        // 多人模式有两个玩家
        players.push_back(Player(0));
        players.push_back(Player(1));
    }
    
    // 重置gameSteps为默认值
    gameSteps = getMinSteps(); // 使用模式对应的最小步数作为默认值
    
    // 生成游戏数据
    generateGameData();
}

void GameManager::loadUserData() {
    // 从文件加载用户信息
    // 包括用户名和对应的Player对象（主要是保存最高分）
    // 使用文件I/O操作读取用户数据库
}

void GameManager::generateGameData() {
    // 生成A和B的轨迹
    // 1. 首先生成A的实际轨迹
    bool isDifficult = (currentGameMode == COMPLEX_SINGLE || currentGameMode == COMPLEX_MULTI);
    objectA.generateTrajectory(isDifficult, gameSteps);
    
    // 2. 生成B的相对轨迹
    objectA.generateRelativeTrajectory(gameSteps, isDifficult);
    
    // 3. 根据A的实际轨迹和B的相对轨迹计算B的实际轨迹
    objectA.calculateActualTrajectory();
}

void GameManager::handleUserInput(/* 参数 */) {
    // 处理用户输入
    // 例如：鼠标点击、键盘输入等
    // 根据输入更新玩家的预测轨迹等
}

void GameManager::updateGame() {
    // 更新游戏状态
    // 例如：检查游戏是否结束，更新对象位置等
}

void GameManager::renderGame() {
   
   
}

void GameManager::startNewRound() {
    if (isMultiplayerMode()) {
        // 切换玩家
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    }
    
    // 重置回合完成标志
    roundComplete = false;
    
    // 生成新的游戏数据
    generateGameData();
    
    // 增加回合数
    currentRound++;
}

void GameManager::submitPrediction(int playerID) {
    // 提交玩家的预测
    // 1. 结束计时
    players[playerID].endTimer();
    
    // 2. 获取玩家的预测轨迹并设置给B
    //objectB.predictedTrajectory = players[playerID].getPrediction();
    
    // 3. 计算结果
    calculateResults();
    
    // 4. 设置回合完成标志
    roundComplete = true;
    
    // 5. 如果所有玩家都完成了当前回合，开始新回合
    if (isMultiplayerMode()) {
        bool allPlayersComplete = true;
        for (const auto& player : players) {
            if (!roundComplete) {
                allPlayersComplete = false;
                break;
            }
        }
        
        if (allPlayersComplete && !isGameOver()) {
            startNewRound();
        }
    }
}

void GameManager::calculateResults() {
    // 计算游戏结果
    // 1. 计算当前玩家的得分
    players[currentPlayerIndex].calculateScore(objectB.getActualTrajectory());
    
    // 2. 更新并保存最高分
    updateAndSaveHighScore(currentPlayerIndex);
}

void GameManager::switchPlayer() {
    // 切换当前玩家（多人模式）
    // 检查是否是多人模式，如果是则切换玩家
    if (currentGameMode == SIMPLE_MULTI || currentGameMode == COMPLEX_MULTI) {
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    }
    
    // 重置回合完成标志
    roundComplete = false;
}

void GameManager::saveUserData() {
    // 将用户信息保存到文件
    // 包括用户名和对应的最高分
    // 使用文件I/O操作写入用户数据库
}

bool GameManager::registerUser(const std::string& username) {
    // 注册新用户
    // 检查用户名是否已存在
    if (userDatabase.find(username) != userDatabase.end()) {
        return false; // 用户名已存在
    }
    
    // 创建新用户并添加到数据库
    userDatabase[username] = Player();
    loggedInUsername = username;
    saveUserData();
    return true;
}

bool GameManager::loginUser(const std::string& username) {
    // 用户登录
    // 检查用户名是否存在
    if (userDatabase.find(username) == userDatabase.end()) {
        return false; // 用户名不存在
    }
    
    // 设置当前登录用户
    loggedInUsername = username;
    return true;
}

void GameManager::updateAndSaveHighScore(int playerID) {
    // 更新并保存最高分
    // 检查是否有登录用户
    if (!loggedInUsername.empty()) {
        // 更新用户数据库中的最高分
        Player& currentPlayer = players[playerID];
        Player& databasePlayer = userDatabase[loggedInUsername];
        
        if (currentPlayer.getCurrentScore() > databasePlayer.getHighScore()) {
            databasePlayer.setPrediction(currentPlayer.getPrediction());
            databasePlayer.calculateScore(objectB.getActualTrajectory());
            databasePlayer.updateHighScore();
            saveUserData();
        }
    }
}

GameManager::GameMode GameManager::getCurrentGameMode() const {
    // 返回当前游戏模式
    return currentGameMode;
}

Player& GameManager::getCurrentPlayer() {
    // 返回当前玩家
    return players[currentPlayerIndex];
}

const GameObject& GameManager::getObjectA() const {
    // 返回ObjectA
    return objectA;
}

const GameObject& GameManager::getObjectB() const {
    // 返回ObjectB
    return objectB;
}

bool GameManager::isGameRunning() const {
    // 返回游戏运行状态
    return gameRunning;
}

bool GameManager::isRoundComplete() const {
    // 返回当前回合是否完成
    return roundComplete;
} 