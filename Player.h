#pragma once
#include "Trajectory.h"
#include <chrono>

class Player {
private:
    int playerID;
    double currentScore;
    double highScore;
    double timeTaken;  // 玩家做出预测所用的时间（秒）
    Trajectory prediction;
    
    // 计时器相关变量
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    bool timerRunning;

public:
    // 构造函数
    Player(int id = 0);
    
    // 获取玩家ID
    int getPlayerID() const;
    
    // 开始预测计时
    void startTimer();
    
    // 结束预测计时并返回用时（秒）
    double endTimer();
    
    // 记录预测轨迹
    void setPrediction(const Trajectory& trajectory);
    
    // 获取预测轨迹
    const Trajectory& getPrediction() const;
    
    // 计算得分（基于预测轨迹和实际轨迹的相似度）
    void calculateScore(const Trajectory& actualTrajectory);
    
    // 获取当前得分
    double getCurrentScore() const;
    
    // 获取最高得分
    double getHighScore() const;
    
    // 获取用时
    double getTimeTaken() const;
    
    // 更新最高分（如果当前分数高于最高分）
    bool updateHighScore();
    
    // 重置玩家数据（用于新一轮游戏）
    void reset();
}; 