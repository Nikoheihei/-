#include "GameObject.h"
#include<cmath>

GameObject::GameObject(int startRow, int startCol, const std::string& objectColor) 
    : currentCell(startRow, startCol), color(objectColor) {
    // 初始化游戏对象，设置起始位置和颜色
    // 将起始位置添加到实际轨迹中
    actualTrajectory.addCell(currentCell);
}

const GridCell& GameObject::getCurrentCell() const {
    // 返回当前位置
    return currentCell;
}

void GameObject::setCurrentCell(const GridCell& cell) {
    // 设置当前位置
    currentCell = cell;
    // 更新轨迹
    actualTrajectory.addCell(cell);
}

void GameObject::removeFourDirection(const GridCell& cell, int direction) {
    double m = cell.getRow();
    double n = cell.getCol();

    switch (direction) {
        case 1:
            GridCell x(m, n + 1);
            actualTrajectory.addCell(x);
            break;
        case 2:
            GridCell x(m - 1, n);
            actualTrajectory.addCell(x);
            break;
        case 3:
            GridCell x(m, n - 1);
            actualTrajectory.addCell(x);
            break;
        case 4:
            GridCell x(m - 1, n);
            actualTrajectory.addCell(x);
            break;
    }
    setCurrentCell(actualTrajectory.getCell(actualTrajectory.getLength()-1));

}

void GameObject::removeSixDirection(const GridCell& cell, int direction) {
    double m = cell.getRow();
    double n = cell.getCol();

    switch (direction) {
        case 1:
            GridCell x(m, n - sqrt(3));
            actualTrajectory.addCell(x);
            break;
        case 2:
            GridCell x(m-sqrt(3)/2, n+1.5);
            actualTrajectory.addCell(x);
            break;
        case 3:
            GridCell x(m+sqrt(3)/2, n +1.5);
            actualTrajectory.addCell(x);
            break;
        case 4:
            GridCell x(m+sqrt(3), n);
            actualTrajectory.addCell(x);
            break;
        case 5:
            GridCell x(m+sqrt(3)/2 , n-1.5);
            actualTrajectory.addCell(x);
            break;
        case 6:
            GridCell x(m - sqrt(3)/2, n-1.5);
            actualTrajectory.addCell(x);
            break;
    }
    setCurrentCell(actualTrajectory.getCell(actualTrajectory.getLength()-1));

}

const Trajectory& GameObject::getActualTrajectory() const {
    // 返回实际轨迹
    return actualTrajectory;
}

const std::string& GameObject::getColor() const {
    // 返回颜色
    return color;
}

void GameObject::setColor(const std::string& newColor) {
    // 设置颜色
    color = newColor;
} 