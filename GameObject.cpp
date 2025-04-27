#include "GameObject.h"
#include <cmath>
#include <ctime>  // 添加ctime头文件用于time函数
#include <cstdlib> // 添加cstdlib头文件用于rand函数
#include <algorithm> // 添加algorithm头文件用于std::min函数

// Define constants for directions
const int RIGHT = 1;
const int UP = 2;
const int LEFT = 3;
const int DOWN = 4;
const int UP_RIGHT = 2;
const int DOWN_RIGHT = 3;
const int DOWN_LEFT = 5;
const int UP_LEFT = 6;

// Precompute constants
const double SQRT_3 = sqrt(3);
const double SQRT_3_OVER_2 = SQRT_3 / 2;

GameObject::GameObject(int startRow, int startCol, const std::string& objectColor) {
    // 初始化游戏对象，设置起始位置和颜色
    // 将起始位置添加到实际轨迹中
    GridCell initialCell(startRow, startCol);
    actualTrajectory.addCell(initialCell);
}

const GridCell& GameObject::getCurrentCell() const {
    // 返回当前位置
    return actualTrajectory.getCurrentCell();
}

void GameObject::setCurrentCell(const GridCell& cell) {
    // 设置当前位置
    actualTrajectory.setCurrentCell(cell);
    // 更新轨迹
    actualTrajectory.addCell(cell);
}

// Ensure addCellBasedOnDirection is a member function of GameObject
void GameObject::addCellBasedOnDirection(const GridCell& cell, int direction, bool isSixDirection) {
    double m = cell.getRow();
    double n = cell.getCol();
    GridCell x;
    
    if (isSixDirection) {
        switch (direction) {
            case RIGHT:
                x = GridCell(m, n - SQRT_3);
                break;
            case UP_RIGHT:
                x = GridCell(m - SQRT_3_OVER_2, n + 1.5);
                break;
            case DOWN_RIGHT:
                x = GridCell(m + SQRT_3_OVER_2, n + 1.5);
                break;
            case DOWN:
                x = GridCell(m + SQRT_3, n);
                break;
            case DOWN_LEFT:
                x = GridCell(m + SQRT_3_OVER_2, n - 1.5);
                break;
            case UP_LEFT:
                x = GridCell(m - SQRT_3_OVER_2, n - 1.5);
                break;
        }
    } else {
        switch (direction) {
            case RIGHT:
                x = GridCell(m, n + 1);
                break;
            case UP:
                x = GridCell(m - 1, n);
                break;
            case LEFT:
                x = GridCell(m, n - 1);
                break;
            case DOWN:
                x = GridCell(m + 1, n);
                break;
        }
    }
    actualTrajectory.addCell(x);
}

void GameObject::removeFourDirection(const GridCell& cell, int direction) {
    addCellBasedOnDirection(cell, direction, false);
}

void GameObject::removeSixDirection(const GridCell& cell, int direction) {
    addCellBasedOnDirection(cell, direction, true);
}

const Trajectory& GameObject::getActualTrajectory() const {
    // 返回实际轨迹
    return actualTrajectory;
}

void GameObject::generateTrajectory(bool difficulty, int steps) {
    // 清空现有轨迹，保留起始位置
    GridCell startCell = actualTrajectory.getCurrentCell();
    actualTrajectory.clear();
    actualTrajectory.addCell(startCell);
    srand(static_cast<unsigned int>(time(0)));
    
    if (difficulty == 0) { 
        for (int i = 0; i < steps; i++) {
            int direction = rand() % 4 + 1;
            removeFourDirection(getCurrentCell(), direction);
        }
    } else if (difficulty == 1) { 
        for (int i = 0; i < steps; i++) {
            int direction = rand() % 6 + 1;
            removeSixDirection(getCurrentCell(), direction);
        }
    }
}

void GameObject::generateRelativeTrajectory(int steps, bool difficulty) {
    // 清空现有相对轨迹
    relativeTrajectory.clear();
    // 保存当前位置
    GridCell startCell = actualTrajectory.getCurrentCell();
    
    srand(static_cast<unsigned int>(time(0)));
    
    if (difficulty == 0) {  
        for (int i = 0; i < steps; i++) {
            int direction = rand() % 6 + 1;
            removeSixDirection(getCurrentCell(), direction);
            relativeTrajectory.addCell(getCurrentCell());
        }
    } else if (difficulty == 1) {
        for (int i = 0; i < steps; i++) {
            int direction = rand() % 4 + 1;
            removeFourDirection(getCurrentCell(), direction);
            relativeTrajectory.addCell(getCurrentCell());
        }
    }
    
    // 恢复当前位置
    actualTrajectory.setCurrentCell(startCell);
}

void GameObject::calculateActualTrajectory(const Trajectory& referenceTrajectory) {
    // 清空现有实际轨迹
    actualTrajectory.clear();
    
    // 根据参考轨迹和相对轨迹计算实际轨迹
    // 对于每个时间点t，计算位置 = 参考位置 + 相对位置
    int minLength = std::min(referenceTrajectory.getLength(), relativeTrajectory.getLength());
    
    for (int i = 0; i < minLength; i++) {
        const GridCell& refCell = referenceTrajectory.getCell(i);
        const GridCell& relCell = relativeTrajectory.getCell(i+1);
        const GridCell& relCell_ = relativeTrajectory.getCell(i);
        
        // 计算新位置 (简单相加作为示例，实际应用可能需要更复杂的计算)
        double newRow = refCell.getRow() + relCell.getRow();
        double newCol = refCell.getCol() + relCell.getCol();
        
        GridCell newCell(newRow, newCol);
        actualTrajectory.addCell(newCell);
    }
}

void GameObject::setPredictedTrajectory(const Trajectory& predicted) {
    // 设置玩家预测的轨迹
    predictedTrajectory = predicted;
}

const Trajectory& GameObject::getPredictedTrajectory() const {
    // 返回玩家预测的轨迹
    return predictedTrajectory;
}

const Trajectory& GameObject::getRelativeTrajectory() const {
    // 返回相对轨迹
    return relativeTrajectory;
}
