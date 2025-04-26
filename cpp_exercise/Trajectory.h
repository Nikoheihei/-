#pragma once
#include <vector>
#include "GridCell.h"

class Trajectory {
private:
    std::vector<GridCell> cells;  // 存储构成轨迹的一系列网格单元

public:
    // 构造函数
    Trajectory();
    
    // 向轨迹中添加一个网格单元
    void addCell(const GridCell& cell);
    
    // 返回包含所有网格单元的向量
    const std::vector<GridCell>& getCells() const;
    
    // 返回轨迹中网格单元的数量
    size_t getLength() const;
    
    // 返回指定索引处的网格单元
    GridCell getCell(size_t index) const;
    
    // 计算两条轨迹的相似度（例如，重合的单元格比例）
    double calculateSimilarity(const Trajectory& other) const;
    
    // 清空轨迹
    void clear();
}; 