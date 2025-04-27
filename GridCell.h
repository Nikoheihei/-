#pragma once

class GridCell {
private:
    double row;
    double col;//double因为每个位置图像可能是一个正六边形

public:
    // 构造函数
    GridCell(double r = 0, double c = 0);
    
    // 获取行列坐标
    double getRow() const;
    double getCol() const;
    
    // 比较两个网格单元是否相等
    bool operator==(const GridCell& other) const;
    bool operator!=(const GridCell& other) const;
}; 