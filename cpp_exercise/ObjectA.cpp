#include "ObjectA.h"
#include<cstdlib>
#include<ctime>
#include <random>

ObjectA::ObjectA(int startRow, int startCol, const std::string& objectColor)
    : GameObject(startRow, startCol, objectColor) {
    // 初始化ObjectA对象
}

void ObjectA::generateTrajectory(bool difficulty,int steps) {
    // 清空现有轨迹，保留起始位置
    actualTrajectory.clear();
    actualTrajectory.addCell(currentCell);
    srand(static_cast<unsigned int>(time(0)));
    if(difficulty==0){ 
        for(int i=0;i<steps;i++){
        int direction=rand()%4+1;
        removeFourDirection(getCurrentCell(),direction);
        return;
    }}
    else if(difficulty==1){ 
        for(int i=0;i<steps;i++){
        int direction=rand()%6+1;
        removeSixDirection(getCurrentCell(),direction);
        return;
    }}
   
    
}


void ObjectA::draw() {
    // 在此处实现绘制A及其轨迹的代码
    // 可以使用不同的颜色和样式来区分A和其轨迹
} 