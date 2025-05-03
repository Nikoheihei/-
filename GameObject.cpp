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
    // 清空现有轨迹
    actualTrajectory.clear();
    
    // 生成随机初始坐标（范围-10到10）
    srand(static_cast<unsigned int>(time(0)));
    double startRow = (rand() % 21) - 10;
    double startCol = (rand() % 21) - 10;
    GridCell startCell(startRow, startCol);
    actualTrajectory.addCell(startCell);
    
    // 记录上一次的方向，确保轨迹连续性
    int lastDirection = 0;
    
    // 根据难度选择移动方式
    if (difficulty) {
        // 复杂模式：六方向移动
        for (int i = 0; i < steps; i++) {
            int direction;
            
            // 如果不是第一步，则确保方向与上一步相连
            if (lastDirection > 0) {
                // 生成与上一步相邻的方向，避免生成相反方向
                do {
                    direction = rand() % 6 + 1;
                } while ((direction == 3 && lastDirection == 1) || 
                         (direction == 1 && lastDirection == 3) ||
                         (direction == 2 && lastDirection == 4) ||
                         (direction == 4 && lastDirection == 2) ||
                         (direction == 5 && lastDirection == 6) ||
                         (direction == 6 && lastDirection == 5));
            } else {
                direction = rand() % 6 + 1;
            }
            
            lastDirection = direction;
            removeSixDirection(getCurrentCell(), direction);
        }
    } else {
        // 简单模式：四方向移动
        for (int i = 0; i < steps; i++) {
            int direction;
            
            if (lastDirection > 0) {
                do {
                    direction = rand() % 4 + 1;
                } while ((direction == 3 && lastDirection == 1) || 
                         (direction == 1 && lastDirection == 3) ||
                         (direction == 2 && lastDirection == 4) ||
                         (direction == 4 && lastDirection == 2));
            } else {
                direction = rand() % 4 + 1;
            }
            
            lastDirection = direction;
            removeFourDirection(getCurrentCell(), direction);
        }
    }
}

void GameObject::generateRelativeTrajectory(int steps, bool difficulty) {
    // 清空现有相对轨迹
    relativeTrajectory.clear();
    
    // 生成随机初始坐标（范围-10到10）
    srand(static_cast<unsigned int>(time(0) + 100));
    double startRow = (rand() % 21) - 10;
    double startCol = (rand() % 21) - 10;
    GridCell startCell(startRow, startCol);
    relativeTrajectory.addCell(startCell);
    
    // 保存当前位置用于轨迹生成
    actualTrajectory.clear();
    actualTrajectory.addCell(startCell);
    
    // 记录上一次的方向，确保轨迹连续性
    int lastDirection = 0;
    
    // 根据难度选择移动方式
    if (difficulty) {
        // 复杂模式：六方向移动
        for (int i = 0; i < steps - 1; i++) {
            int direction;
            
            if (lastDirection > 0) {
                do {
                    direction = rand() % 6 + 1;
                } while ((direction == 3 && lastDirection == 1) || 
                         (direction == 1 && lastDirection == 3) ||
                         (direction == 2 && lastDirection == 4) ||
                         (direction == 4 && lastDirection == 2) ||
                         (direction == 5 && lastDirection == 6) ||
                         (direction == 6 && lastDirection == 5));
            } else {
                direction = rand() % 6 + 1;
            }
            
            lastDirection = direction;
            removeSixDirection(getCurrentCell(), direction);
            relativeTrajectory.addCell(getCurrentCell());
        }
    } else {
        // 简单模式：四方向移动
        for (int i = 0; i < steps - 1; i++) {
            int direction;
            
            if (lastDirection > 0) {
                do {
                    direction = rand() % 4 + 1;
                } while ((direction == 3 && lastDirection == 1) || 
                         (direction == 1 && lastDirection == 3) ||
                         (direction == 2 && lastDirection == 4) ||
                         (direction == 4 && lastDirection == 2));
            } else {
                direction = rand() % 4 + 1;
            }
            
            lastDirection = direction;
            removeFourDirection(getCurrentCell(), direction);
            relativeTrajectory.addCell(getCurrentCell());
        }
    }
}

void GameObject::calculateActualTrajectory(const Trajectory& referenceTrajectory) {
    // 清空现有实际轨迹
    actualTrajectory.clear();
    
    // 先获取两个轨迹的长度
    int refLength = referenceTrajectory.getLength();
    int relLength = relativeTrajectory.getLength();
    
    // 确保至少有起始点
    if (refLength < 1 || relLength < 1) {
        return;
    }
    
    // 获取起始点并添加到实际轨迹
    const GridCell& refStart = referenceTrajectory.getCell(0);
    const GridCell& relStart = relativeTrajectory.getCell(0);
    
    // 随机生成实际轨迹的起始点（范围-10到10）
    srand(static_cast<unsigned int>(time(0) + 200)); // 再次使用不同的随机种子
    double startRow = (rand() % 21) - 10;
    double startCol = (rand() % 21) - 10;
    actualTrajectory.addCell(GridCell(startRow, startCol));
    
    // 使用四方向移动生成实际轨迹，确保每次只移动1个单位
    int lastDirection = 0;
    int minLength = std::min(refLength, relLength);
    
    for (int i = 1; i < minLength; i++) {
        const GridCell& lastCell = actualTrajectory.getCurrentCell();
        double lastRow = lastCell.getRow();
        double lastCol = lastCell.getCol();
        
        // 随机选择一个方向，但避免与上一步相反的方向
        int direction;
        if (lastDirection > 0) {
            do {
                direction = rand() % 4 + 1;
            } while ((direction == 3 && lastDirection == 1) || 
                     (direction == 1 && lastDirection == 3) ||
                     (direction == 2 && lastDirection == 4) ||
                     (direction == 4 && lastDirection == 2));
        } else {
            direction = rand() % 4 + 1;
        }
        
        lastDirection = direction;
        
        // 根据方向更新位置
        GridCell newCell(lastRow, lastCol);
        switch (direction) {
            case RIGHT: // 右
                newCell = GridCell(lastRow, lastCol + 1);
                break;
            case UP: // 上
                newCell = GridCell(lastRow - 1, lastCol);
                break;
            case LEFT: // 左
                newCell = GridCell(lastRow, lastCol - 1);
                break;
            case DOWN: // 下
                newCell = GridCell(lastRow + 1, lastCol);
                break;
        }
        
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
