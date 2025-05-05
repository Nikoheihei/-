#include "GameObject.h"
#include <cmath>
#include <ctime>  // 添加ctime头文件用于time函数
#include <cstdlib> // 添加cstdlib头文件用于rand函数
#include <algorithm> // 添加algorithm头文件用于std::min函数
#include <vector>

using namespace std;

// 坐标范围常量
const int MIN_TRAJ_COORD = -15;
const int MAX_TRAJ_COORD = 15;

const vector<GridCell>hex_point={
    GridCell(0, -2),
    GridCell(0, 2),
    GridCell(-1, 1),
    GridCell(-1, -1),
    GridCell(1, 1),
    GridCell(1, -1),
    GridCell(-1,0),
    GridCell(1,0)
};
const vector<GridCell>four_directions={
    GridCell(-1, 0),
    GridCell(0, 1),
    GridCell(0, -1),
    GridCell(1, 0)
    
};
const vector<GridCell>hex_directions={
    GridCell(-2,0),
    GridCell(-1,-3),
    GridCell(1, -3),
    GridCell(2, 0),
    GridCell(1, 3),
    GridCell(-1, 3)
};
const int UP = 0;
const int RIGHT = 1;
const int LEFT = 2;
const int DOWN = 3;
const int UP_RIGHT = 5;
const int DOWN_RIGHT = 4;
const int DOWN_LEFT = 2;
const int UP_LEFT = 1;


GameObject::GameObject(int startRow, int startCol, const std::string& objectColor) {
    // 初始化游戏对象，设置起始位置和颜色
    // 将起始位置添加到实际轨迹中
    GridCell initialCell(startRow, startCol);
    actualTrajectory.addCell(initialCell);
}

const GridCell& GameObject::getCurrentCell(Trajectory&trajectory) const {
    // 返回当前位置
    return trajectory.getCurrentCell();
}

// 添加一个辅助函数来检查移动是否会超出边界
bool GameObject::wouldExceedBounds(const GridCell& cell, int direction, bool isSixDirection) {
    GridCell newCell;
    
    if (isSixDirection) {
        switch (direction) {
            case UP_RIGHT:
                newCell = cell+hex_directions[UP_RIGHT];
                break;
            case DOWN_RIGHT:
                newCell = cell+hex_directions[DOWN_RIGHT];
                break;
            case DOWN:
                newCell = cell+hex_directions[DOWN];
                break;
            case DOWN_LEFT:
                newCell = cell+hex_directions[DOWN_LEFT];
                break;
            case UP_LEFT:
                newCell = cell+hex_directions[UP_LEFT];
                break;
            case UP:
                newCell = cell+hex_directions[UP];
                break;  
            default:
                return true; // 无效方向
        }
    } else {
        switch (direction) {
            case RIGHT:
                newCell = cell+four_directions[RIGHT];
                break;
            case UP:
                newCell = cell+four_directions[UP];
                break;
            case LEFT:
                newCell = cell+four_directions[LEFT];
                break;
            case DOWN:
                newCell = cell+four_directions[DOWN];
                break;
            default:
                return true; // 无效方向
        }
    }
    
    // 检查新坐标是否在范围内，确保坐标在-15到15之间
    return (newCell.getRow() > MAX_TRAJ_COORD || newCell.getRow() < MIN_TRAJ_COORD || 
            newCell.getCol() > MAX_TRAJ_COORD || newCell.getCol() < MIN_TRAJ_COORD);
}

void GameObject::addCellBasedOnDirection(Trajectory& trajectory,const GridCell& cell, int direction, bool isSixDirection) {
    GridCell x;
    
    if (isSixDirection) {
        switch (direction) {
            case UP_RIGHT:
                x = cell+hex_directions[UP_RIGHT];
                break;
            case DOWN_RIGHT:
                x = cell+hex_directions[DOWN_RIGHT];
                break;
            case DOWN:
                x = cell+hex_directions[DOWN];
                break;
            case DOWN_LEFT:
                x = cell+hex_directions[DOWN_LEFT];
                break;
            case UP_LEFT:
                x = cell+hex_directions[UP_LEFT];
                break;
            case UP:
                x = cell+hex_directions[UP];
                break;  
        }
    } else {
        switch (direction) {
            case RIGHT:
                x = cell+four_directions[RIGHT];
                break;
            case UP:
                x = cell+four_directions[UP];
                break;
            case LEFT:
                x = cell+four_directions[LEFT];
                break;
            case DOWN:
                x = cell+four_directions[DOWN];
                break;
        }
    }
    
    trajectory.addCell(x);
}

void GameObject::removeFourDirection(Trajectory& trajectory,const GridCell& cell, int direction) {
    addCellBasedOnDirection(trajectory,cell, direction, false);
}

void GameObject::removeSixDirection(Trajectory& trajectory,const GridCell& cell, int direction) {
    addCellBasedOnDirection(trajectory,cell, direction, true);
}

bool checkIfExist(const Trajectory&trajectory,GridCell newgrid,const int direction,bool ISCOMPLEX){
    int i=trajectory.getLength();
    int value=0;
    if(ISCOMPLEX){
        for(int j=0;j<i;j++){
        if(trajectory.getCell(j)==(newgrid+hex_directions[direction])) value=1;}
    }
    else{
    for(int j=0;j<i;j++){
        if(trajectory.getCell(j)==(newgrid+four_directions[direction])) value=1;
    }
    }
    return value;
}

void GameObject::generateTrajectory(bool difficulty, int steps) {
    // 清空现有轨迹
    actualTrajectory.clear();
    
    // 生成随机初始坐标（范围-15到15）
    srand(static_cast<unsigned int>(time(0)));
    int startRow = (rand() % (MAX_TRAJ_COORD - MIN_TRAJ_COORD + 1)) + MIN_TRAJ_COORD;
    int startCol = (rand() % (MAX_TRAJ_COORD - MIN_TRAJ_COORD + 1)) + MIN_TRAJ_COORD;
    GridCell startCell(startRow, startCol);
    actualTrajectory.addCell(startCell);
    
    // 记录上一次的方向，确保轨迹连续性
    int lastDirection = -1;
    
    // 根据难度选择移动方式
    if (difficulty) {
        // 复杂模式：六方向移动
        for (int i = 0; i < steps; i++) {
            int direction;
            
            // 如果不是第一步，则确保方向与上一步相连
            if (lastDirection >=0) {
                // 生成与上一步相邻的方向，避免生成相反方向
                do {
                    direction = rand() % 6;
                } while ((abs(direction-lastDirection)==3)||
                    checkIfExist(actualTrajectory,actualTrajectory.getCurrentCell(),direction,difficulty)); 
                    //||wouldExceedBounds(actualTrajectory.getCurrentCell(), direction, true));
            } else {
                //do {
                    direction = rand() % 6;
                //} while (wouldExceedBounds(actualTrajectory.getCurrentCell(), direction, true));
            }
            
            lastDirection = direction;
            removeSixDirection(actualTrajectory,getCurrentCell(actualTrajectory), direction);
        }
    } else {
        // 简单模式：四方向移动
        for (int i = 0; i < steps; i++) {
            int direction;
            
            if (lastDirection >= 0) {
                do {
                    direction = rand() % 4;
                } while (((direction==0&&lastDirection==3)||
                        (direction==3&&lastDirection==0)||
                        (direction==1&&lastDirection==2)||
                        (direction==2&&lastDirection==1))||
                        checkIfExist(actualTrajectory,actualTrajectory.getCurrentCell(),direction,difficulty));
                        //||wouldExceedBounds(actualTrajectory.getCurrentCell(), direction, false));
            } else {
                //do {
                    direction = rand() % 4;
                //} while (wouldExceedBounds(actualTrajectory.getCurrentCell(), direction, false));
            }
            
            lastDirection = direction;
            removeFourDirection(actualTrajectory,getCurrentCell(actualTrajectory), direction);
        }
    }
}

void GameObject::generateRelativeTrajectory(int steps, bool difficulty) {
    // 清空现有相对轨迹
    relativeTrajectory.clear();
    
    // 生成随机初始坐标（范围-15到15）
    srand(static_cast<unsigned int>(time(0) + 100));
    double startRow = (rand() % (MAX_TRAJ_COORD - MIN_TRAJ_COORD + 1)) + MIN_TRAJ_COORD;
    double startCol = (rand() % (MAX_TRAJ_COORD - MIN_TRAJ_COORD + 1)) + MIN_TRAJ_COORD;
    GridCell startCell(startRow, startCol);
    relativeTrajectory.addCell(startCell);
    
    // 记录上一次的方向，确保轨迹连续性
    int lastDirection = -1;
    
    // 根据难度选择移动方式
    if (difficulty) {
        // 复杂模式：六方向移动
        for (int i = 0; i < steps; i++) {
            int direction;
            
            // 如果不是第一步，则确保方向与上一步相连
            if (lastDirection >=0) {
                // 生成与上一步相邻的方向，避免生成相反方向
                do {
                    direction = rand() % 6;
                } while ((abs(direction-lastDirection)==3)||
                    checkIfExist(relativeTrajectory,relativeTrajectory.getCurrentCell(),direction,difficulty)); 
                    //||wouldExceedBounds(relativeTrajectory.getCurrentCell(), direction, true));
            } else {
               // do {
                    direction = rand() % 6;
                //} while (wouldExceedBounds(relativeTrajectory.getCurrentCell(), direction, true));
            }
            
            lastDirection = direction;
            removeSixDirection(relativeTrajectory,getCurrentCell(relativeTrajectory), direction);
        }
    } else {
        // 简单模式：四方向移动
        for (int i = 0; i < steps; i++) {
            int direction;
            
            if (lastDirection >= 0) {
                do {
                    direction = rand() % 4;
                } while ((direction==0&&lastDirection==3)||
                        (direction==3&&lastDirection==0)||
                        (direction==1&&lastDirection==2)||
                        (direction==2&&lastDirection==1)||
                        checkIfExist(relativeTrajectory,relativeTrajectory.getCurrentCell(),direction,difficulty));
                        //||wouldExceedBounds(relativeTrajectory.getCurrentCell(), direction, false));
            } else {
                //do {
                    direction = rand() % 4;
                //} while (wouldExceedBounds(relativeTrajectory.getCurrentCell(), direction, false));
            }
            
            lastDirection = direction;
            removeFourDirection(relativeTrajectory,getCurrentCell(relativeTrajectory), direction);
        }
    }
}
    

void GameObject::calculateActualTrajectory() {
    // 清空现有实际轨迹
    finalTrajectory.clear();
    
    // 先获取两个轨迹的长度
    int actLength = actualTrajectory.getLength();
    int relLength = relativeTrajectory.getLength();
    
    // 确保至少有起始点
    if (actLength < 1 || relLength < 1) {
        return;
    }
    
    // 随机生成实际轨迹的起始点（范围-15到15）
    srand(static_cast<unsigned int>(time(0) + 200)); // 再次使用不同的随机种子
    double startRow = (rand() % (MAX_TRAJ_COORD - MIN_TRAJ_COORD + 1)) + MIN_TRAJ_COORD;
    double startCol = (rand() % (MAX_TRAJ_COORD - MIN_TRAJ_COORD + 1)) + MIN_TRAJ_COORD;
    finalTrajectory.addCell(GridCell(startRow, startCol));
    
    // 使用四方向移动生成实际轨迹，确保每次只移动1个单位
    int lastDirection = -1;
    int minLength = min(actLength, relLength);
    for(int i=1;i<minLength;i++){
        GridCell newCell = finalTrajectory.getCurrentCell()+
                         actualTrajectory.getCell(i)-actualTrajectory.getCell(i-1)+
                         relativeTrajectory.getCell(i)-relativeTrajectory.getCell(i-1);
        
        // 检查是否超出范围，如果超出则跳过
        if (newCell.getRow() > MAX_TRAJ_COORD || newCell.getRow() < MIN_TRAJ_COORD || 
            newCell.getCol() > MAX_TRAJ_COORD || newCell.getCol() < MIN_TRAJ_COORD) {
            continue;
        }
        
        finalTrajectory.addCell(newCell);
    }
}


const Trajectory& GameObject::getfinalTrajectory() const {
    // 返回玩家预测的轨迹
    return finalTrajectory;
}

const Trajectory& GameObject::getRelativeTrajectory() const {
    // 返回相对轨迹
    return relativeTrajectory;
}

const Trajectory& GameObject::getActualTrajectory() const {
    // 返回相对轨迹
    return actualTrajectory;
}

