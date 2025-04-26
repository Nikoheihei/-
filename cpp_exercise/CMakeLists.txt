cmake_minimum_required(VERSION 3.10)
project(TrajectoryGame)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 源文件列表
set(SOURCES
    Main.cpp
    GridCell.cpp
    Trajectory.cpp
    GameObject.cpp
    ObjectA.cpp
    ObjectB.cpp
    Player.cpp
    GameManager.cpp
)

# 头文件列表
set(HEADERS
    GridCell.h
    Trajectory.h
    GameObject.h
    ObjectA.h
    ObjectB.h
    Player.h
    GameManager.h
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 如果需要链接其他库（例如，图形库等），可以在这里添加
# target_link_libraries(${PROJECT_NAME} PRIVATE <library_name>) 
# 项目可以使用CMake构建，编译后运行即可开始游戏。
# 您可以根据需要添加图形库支持（如SFML、SDL等）来实现可视化部分