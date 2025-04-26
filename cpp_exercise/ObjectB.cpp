#include "ObjectB.h"
#include <cstdlib>
#include <ctime>

ObjectB::ObjectB(int startRow, int startCol, const std::string& objectColor)
    : GameObject(startRow, startCol, objectColor) {
    // 初始化ObjectB对象
}

void ObjectB::generateRelativeTrajectory(int steps) {
    // 清空现有相对轨迹
    relativeTrajectory.clear();
    relativeTrajectory.addCell(GridCell(0, 0)); // 相对轨迹的起点是(0,0)
    
    // 初始化随机数生成器
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // 创建一个简单的相对移动模式
    // 例如：随机行走，但有一定概率向特定方向移动
    for (int i = 1; i < steps; i++) {
        // 获取上一个位置
        GridCell lastCell = relativeTrajectory.getCell(relativeTrajectory.getLength() - 1);
        double row = lastCell.getRow();
        double col = lastCell.getCol();
        
        // 随机选择移动方向
        int direction = std::rand() % 4; // 0=上, 1=右, 2=下, 3=左
        
        // 根据方向计算新位置
        switch (direction) {
            case 0: // 上
                row -= 0.5;
                break;
            case 1: // 右
                col += 0.5;
                break;
            case 2: // 下
                row += 0.5;
                break;
            case 3: // 左
                col -= 0.5;
                break;
        }
        
        // 添加新的相对位置到轨迹中
        relativeTrajectory.addCell(GridCell(row, col));
    }
}

void ObjectB::calculateActualTrajectory(const Trajectory& aTrajectory) {
    // 清空现有实际轨迹
    actualTrajectory.clear();
    
    // 根据A的实际轨迹和B的相对轨迹计算B的实际轨迹
    // 对于每个时间点t，B的位置 = A的位置 + B的相对位置
    
    // 确保轨迹长度相匹配
    size_t length = std::min(aTrajectory.getLength(), relativeTrajectory.getLength());
    
    for (size_t i = 0; i < length; i++) {
        // 获取A在时间点i的位置
        GridCell aPosition = aTrajectory.getCell(i);
        // 获取B相对于A在时间点i的相对位置
        GridCell bRelativePosition = relativeTrajectory.getCell(i);
        
        // 计算B的实际位置: B = A + 相对位置
        GridCell bActualPosition(
            aPosition.getRow() + bRelativePosition.getRow(),
            aPosition.getCol() + bRelativePosition.getCol()
        );
        
        // 将B的实际位置添加到轨迹中
        actualTrajectory.addCell(bActualPosition);
    }
    
    // 更新当前位置为轨迹中的最后一个位置
    if (actualTrajectory.getLength() > 0)
        setCurrentCell(actualTrajectory.getCell(actualTrajectory.getLength() - 1));
}

void ObjectB::setPredictedTrajectory(const Trajectory& predicted) {
    // 设置玩家预测的B的轨迹
    predictedTrajectory = predicted;
}

const Trajectory& ObjectB::getPredictedTrajectory() const {
    // 返回玩家预测的轨迹
    return predictedTrajectory;
}

const Trajectory& ObjectB::getRelativeTrajectory() const {
    // 返回B的相对轨迹
    return relativeTrajectory;
}

void ObjectB::generateTrajectory(int steps) {
    // 此方法可能不会直接使用，因为B的轨迹是通过calculateActualTrajectory计算得到的
    // 但为了满足基类的纯虚函数要求，我们需要提供实现
    // 可以选择调用generateRelativeTrajectory方法
    generateRelativeTrajectory(steps);
}

void ObjectB::draw() {
    // 在此处实现绘制B及其轨迹的代码
    // 可以使用不同的颜色和样式来区分:
    // 1. B对象本身
    // 2. B的实际轨迹
    // 3. 玩家预测的B的轨迹
    // 4. B的相对轨迹（如果需要显示）
} 