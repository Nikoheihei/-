#include "GameManager.h"
#include <iostream>
using namespace std;

int main() {
    // 创建游戏管理器实例
    GameManager gameManager;
    
    // 简单的控制台菜单，在实际图形界面中可能会有所不同
    int choice = 0;
    string username;
    
    cout << "欢迎来到轨迹预测游戏!" << endl;
    
    while (true) {
        cout << "\n=== 主菜单 ===" << endl;
        cout << "1. 注册新用户" << endl;
        cout << "2. 用户登录" << endl;
        cout << "3. 开始游戏" << endl;
        cout << "4. 退出" << endl;
        cout << "请选择: ";
        cin >> choice;
        
        switch (choice) {
            case 1: // 注册新用户
                cout << "请输入用户名: ";
                cin >> username;
                if (gameManager.registerUser(username)) {
                    cout << "注册成功!" << endl;
                } else {
                    cout << "用户名已存在!" << endl;
                }
                break;
                
            case 2: // 用户登录
                cout << "请输入用户名: ";
                cin >> username;
                if (gameManager.loginUser(username)) {
                    cout << "登录成功!" << endl;
                } else {
                    cout << "用户名不存在!" << endl;
                }
                break;
                
            case 3: // 开始游戏
                int gameMode;
                cout << "\n=== 选择游戏模式 ===" << endl;
                cout << "1. 简单单人模式" << endl;
                cout << "2. 复杂单人模式" << endl;
                cout << "3. 简单多人模式" << endl;
                cout << "4. 复杂多人模式" << endl;
                cout << "请选择: ";
                cin >> gameMode;
                
                // 初始化游戏
                switch (gameMode) {
                    case 1:
                        gameManager.initializeGame(GameManager::SIMPLE_SINGLE);
                        break;
                    case 2:
                        gameManager.initializeGame(GameManager::COMPLEX_SINGLE);
                        break;
                    case 3:
                        gameManager.initializeGame(GameManager::SIMPLE_MULTI);
                        break;
                    case 4:
                        gameManager.initializeGame(GameManager::COMPLEX_MULTI);
                        break;
                    default:
                        cout << "无效选择!" << endl;
                        continue;
                }
                
                // 在实际应用中，这里会启动游戏循环
                // 在此示例中，我们简单地生成和显示一些数据
                
                cout << "游戏开始!" << endl;
                
                // 这里实际上应该有一个游戏循环
                // 在每一帧中更新游戏状态并渲染
                
                break;
                
            case 4: // 退出
                cout << "谢谢使用，再见!" << endl;
                return 0;
                
            default:
                cout << "无效选择，请重试!" << endl;
        }
    }
    
    return 0;
} 