#pragma once
#include "GameObject.h"

class ObjectA : public GameObject {
public:
    // 构造函数
    ObjectA(int startRow = 0, int startCol = 0, const std::string& objectColor = "red");
    
    // 实现基类的纯虚函数，生成A的实际轨迹
    void generateTrajectory(bool difficulty,int steps);
    
    // 实现基类的纯虚函数，绘制A及其轨迹
    virtual void draw() override;
}; 