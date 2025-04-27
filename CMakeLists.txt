cmake_minimum_required(VERSION 3.10)
project(TrajectoryGame)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置 SFML 的路径（注意：lib/cmake/SFML 是必须的）
set(SFML_DIR "D:/SFML-2.6.2/lib/cmake/SFML")

# 查找 SFML 组件
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

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

# 头文件列表（不是必须列出，但保留也可以）
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

# 链接 SFML 库（使用 2.x 的写法）
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)
