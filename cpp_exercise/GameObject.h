#pragma once
#include "GridCell.h"
#include "Trajectory.h"
#include <string>

class GameObject {
protected:
    GridCell currentCell;        // 对象的当前网格单元位置
    Trajectory actualTrajectory; // 对象的实际移动轨迹
    std::string color;           // 对象的颜色

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
    
    // 获取实际轨迹
    const Trajectory& getActualTrajectory() const;
    
    // 获取颜色
    const std::string& getColor() const;
    
    // 设置颜色
    void setColor(const std::string& newColor);
    
    // 纯虚函数，绘制对象及其轨迹（由派生类实现）
    virtual void draw() = 0;
}; 