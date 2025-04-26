#pragma once
#include "GameObject.h"

class ObjectB : public GameObject {
private:
    Trajectory relativeTrajectory;  // B相对于A的相对轨迹
    Trajectory predictedTrajectory;  // 玩家预测的B的实际轨迹

public:
    // 构造函数
    ObjectB(int startRow = 0, int startCol = 0, const std::string& objectColor = "blue");
    
    // 生成B的相对轨迹
    void generateRelativeTrajectory(int steps);
    
    // 根据A的实际轨迹和B的相对轨迹计算B的实际轨迹
    void calculateActualTrajectory(const Trajectory& aTrajectory);
    
    // 设置玩家的预测轨迹
    void setPredictedTrajectory(const Trajectory& predicted);
    
    // 返回玩家的预测轨迹
    const Trajectory& getPredictedTrajectory() const;
    
    // 返回B的相对轨迹
    const Trajectory& getRelativeTrajectory() const;
    
    // 实现基类的纯虚函数，生成B的实际轨迹
    void generateTrajectory(int steps);
    
    // 实现基类的纯虚函数，绘制B及其轨迹
    virtual void draw() override;
}; 