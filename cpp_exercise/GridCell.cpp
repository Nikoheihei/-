#include "GridCell.h"
//还不确定要不要用SFML库

GridCell::GridCell(double r, double c) : row(r), col(c) {
    // 初始化网格单元格的行列坐标
}

double GridCell::getRow() const {
    // 返回行坐标
    return row;
}

double GridCell::getCol() const {
    // 返回列坐标
    return col;
}

bool GridCell::operator==(const GridCell& other) const {
    // 比较两个网格单元是否相等（行列坐标都相同）
    return row == other.row && col == other.col;
}

bool GridCell::operator!=(const GridCell& other) const {
    // 比较两个网格单元是否不相等
    return !(*this == other);
} 