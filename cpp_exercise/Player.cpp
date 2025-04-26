#include "Player.h"

Player::Player(int id) 
    : playerID(id), currentScore(0.0), highScore(0.0), timeTaken(0.0), timerRunning(false) {
    // 初始化玩家对象
}

int Player::getPlayerID() const {
    // 返回玩家ID
    return playerID;
}

void Player::startTimer() {
    // 开始计时
    // 记录开始时间并设置计时器状态为运行中
    startTime = std::chrono::steady_clock::now();
    timerRunning = true;
}

double Player::endTimer() {
    // 结束计时并计算用时
    // 如果计时器没有启动，返回0
    if (!timerRunning) {
        return 0.0;
    }
    
    // 计算用时（秒）
    auto endTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;
    timeTaken = elapsed.count();
    timerRunning = false;
    
    return timeTaken;
}

void Player::setPrediction(const Trajectory& trajectory) {
    // 设置玩家的预测轨迹
    prediction = trajectory;
}

const Trajectory& Player::getPrediction() const {
    // 返回玩家的预测轨迹
    return prediction;
}

void Player::calculateScore(const Trajectory& actualTrajectory) {
    // 基于预测轨迹和实际轨迹的相似度计算得分
    // 可以使用轨迹的相似度方法，并应用一些额外的评分标准
    // 例如：考虑预测时间、预测难度等
    double similarity = prediction.calculateSimilarity(actualTrajectory);
    
    // 示例评分公式：得分 = 相似度 * 100 * (1 / (1 + timeTaken * 0.1))
    // 这将奖励高相似度和快速预测
    currentScore = similarity * 100.0 * (1.0 / (1.0 + timeTaken * 0.1));
}

double Player::getCurrentScore() const {
    // 返回当前得分
    return currentScore;
}

double Player::getHighScore() const {
    // 返回最高得分
    return highScore;
}

double Player::getTimeTaken() const {
    // 返回预测用时
    return timeTaken;
}

bool Player::updateHighScore() {
    // 如果当前得分高于最高分，更新最高分并返回true
    if (currentScore > highScore) {
        highScore = currentScore;
        return true;
    }
    return false;
}

void Player::reset() {
    // 重置玩家数据（用于新一轮游戏）
    // 保留playerID和highScore
    currentScore = 0.0;
    timeTaken = 0.0;
    timerRunning = false;
    prediction.clear();
} 