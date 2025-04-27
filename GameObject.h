#pragma once
#include "GridCell.h"
#include "Trajectory.h"
#include <string>

class GameObject {
protected:
    Trajectory actualTrajectory; // 对象的实际移动轨迹 
    Trajectory relativeTrajectory;  // 相对轨迹
    Trajectory predictedTrajectory;  // 玩家预测的轨迹

public:
    // 构造函数
    GameObject(int startRow = 0, int startCol = 0, const std::string& objectColor = "white");
    
    // 析构函数
    virtual ~GameObject() = default;
    
    // 获取当前位置
    const GridCell& getCurrentCell() const;
    
    // 设置当前位置
    void setCurrentCell(const GridCell& cell);
    //设置四个方向的移动
    void removeFourDirection(const GridCell&cell,int direction);
    //设置六个方向的移动
    void removeSixDirection(const GridCell&cell,int direction);
    
    // 生成实际轨迹
    void generateTrajectory(bool difficulty, int steps);
    
    // 生成相对轨迹
    void generateRelativeTrajectory(int steps, bool difficulty);
    
    // 根据参考轨迹和相对轨迹计算实际轨迹
    void calculateActualTrajectory(const Trajectory& referenceTrajectory);
    
    // 设置玩家的预测轨迹
    void setPredictedTrajectory(const Trajectory& predicted);
    
    // 获取实际轨迹
    const Trajectory& getActualTrajectory() const;
    
    // 获取预测轨迹
    const Trajectory& getPredictedTrajectory() const;
    
    // 获取相对轨迹
    const Trajectory& getRelativeTrajectory() const;
    
    // 辅助函数：根据方向添加单元格
    void addCellBasedOnDirection(const GridCell& cell, int direction, bool isSixDirection);
}; 