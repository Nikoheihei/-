#include "GameManager.h"
#include <iostream>
#include <SFML/Graphics.hpp>

// 简单游戏循环函数
void runGameLoop(GameManager& gameManager, sf::RenderWindow& window) {
    sf::Font font;
    if (!font.loadFromFile("fonts/KOMIKAP_.ttf")) {
        std::cerr << "无法加载字体!" << std::endl;
        return;
    }
    
    // 创建文本提示
    sf::Text instructionText;
    instructionText.setFont(font);
    instructionText.setCharacterSize(24);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setString("使用鼠标点击预测轨迹位置\n按空格键提交预测\n按ESC返回主菜单");
    instructionText.setPosition(20, 20);
    
    // 创建轨迹显示对象
    sf::CircleShape objectA(10);
    objectA.setFillColor(sf::Color::Red);
    
    sf::CircleShape objectB(10);
    objectB.setFillColor(sf::Color::Blue);
    
    sf::CircleShape predictedPoint(5);
    predictedPoint.setFillColor(sf::Color(0, 255, 0, 128)); // 半透明绿色
    
    // 玩家的预测轨迹
    Trajectory playerPrediction;
    
    // 当前显示的轨迹点索引
    size_t currentIndex = 0;
    sf::Clock animationClock;
    
    // 游戏状态
    bool showingResults = false;
    bool predictionSubmitted = false;
    
    // 主游戏循环
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            // 处理按键
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    return; // 返回主菜单
                }
                
                if (event.key.code == sf::Keyboard::Space && !predictionSubmitted) {
                    // 提交预测
                    gameManager.getCurrentPlayer().setPrediction(playerPrediction);
                    gameManager.submitPrediction(gameManager.getCurrentPlayer().getPlayerID());
                    predictionSubmitted = true;
                    showingResults = true;
                    currentIndex = 0;
                    animationClock.restart();
                }
            }
            
            // 处理鼠标点击（添加预测点）
            if (event.type == sf::Event::MouseButtonPressed && 
                event.mouseButton.button == sf::Mouse::Left && 
                !predictionSubmitted) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                // 将鼠标位置转换为GridCell
                GridCell cell(mousePos.y / 20.0, mousePos.x / 20.0); // 简单缩放
                playerPrediction.addCell(cell);
            }
        }
        
        // 更新
        if (!showingResults) {
            // 显示A的轨迹动画
            if (animationClock.getElapsedTime().asSeconds() > 0.5f) {
                animationClock.restart();
                if (currentIndex < gameManager.getObjectA().getActualTrajectory().getLength() - 1)
                    currentIndex++;
            }
        } else {
            // 显示结果动画
            if (animationClock.getElapsedTime().asSeconds() > 0.5f) {
                animationClock.restart();
                if (currentIndex < gameManager.getObjectB().getActualTrajectory().getLength() - 1)
                    currentIndex++;
            }
        }
        
        // 绘制
        window.clear(sf::Color(50, 50, 50));
        
        // 绘制当前轨迹
        if (currentIndex < gameManager.getObjectA().getActualTrajectory().getLength()) {
            GridCell aPos = gameManager.getObjectA().getActualTrajectory().getCell(currentIndex);
            objectA.setPosition(aPos.getCol() * 20, aPos.getRow() * 20);
            window.draw(objectA);
        }
        
        // 如果显示结果，绘制B的实际轨迹
        if (showingResults && currentIndex < gameManager.getObjectB().getActualTrajectory().getLength()) {
            GridCell bPos = gameManager.getObjectB().getActualTrajectory().getCell(currentIndex);
            objectB.setPosition(bPos.getCol() * 20, bPos.getRow() * 20);
            window.draw(objectB);
        }
        
        // 绘制玩家的预测点
        for (size_t i = 0; i < playerPrediction.getLength(); i++) {
            GridCell pos = playerPrediction.getCell(i);
            predictedPoint.setPosition(pos.getCol() * 20, pos.getRow() * 20);
            window.draw(predictedPoint);
        }
        
        // 绘制信息文本
        window.draw(instructionText);
        
        // 如果预测已提交，显示分数
        if (predictionSubmitted) {
            sf::Text scoreText;
            scoreText.setFont(font);
            scoreText.setCharacterSize(30);
            scoreText.setFillColor(sf::Color::Yellow);
            scoreText.setString("得分: " + std::to_string(static_cast<int>(gameManager.getCurrentPlayer().getCurrentScore())));
            scoreText.setPosition(window.getSize().x - 200, 20);
            window.draw(scoreText);
        }
        
        window.display();
    }
}

int main() {
    // 创建游戏管理器实例
    GameManager gameManager;
    
    // 创建SFML窗口
    sf::RenderWindow window(sf::VideoMode(1280, 720), "轨迹预测游戏");
    window.setFramerateLimit(60);
    
    // 加载字体
    sf::Font font;
    if (!font.loadFromFile("fonts/KOMIKAP_.ttf")) {
        std::cerr << "无法加载字体!" << std::endl;
        return -1;
    }
    
    // 简单的控制台菜单，在实际图形界面中可能会有所不同
    int choice = 0;
    std::string username;
    bool running = true;
    
    // 创建UI元素
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("轨迹预测游戏");
    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(window.getSize().x / 2 - titleText.getLocalBounds().width / 2, 50);
    
    sf::Text menuText;
    menuText.setFont(font);
    menuText.setCharacterSize(30);
    menuText.setFillColor(sf::Color::White);
    
    sf::Text inputPrompt;
    inputPrompt.setFont(font);
    inputPrompt.setCharacterSize(24);
    inputPrompt.setFillColor(sf::Color::White);
    
    // 输入状态
    bool waitingForUsernameInput = false;
    std::string inputBuffer;
    
    while (running) {
        // 构建菜单文本
        std::string menuString = "\n\n1. 注册新用户\n2. 用户登录\n3. 开始游戏\n4. 退出";
        menuText.setString(menuString);
        menuText.setPosition(window.getSize().x / 2 - menuText.getLocalBounds().width / 2, 150);
        
        // 处理事件
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                running = false;
            }
            
            if (event.type == sf::Event::KeyPressed) {
                if (waitingForUsernameInput) {
                    if (event.key.code == sf::Keyboard::Return) {
                        username = inputBuffer;
                        inputBuffer.clear();
                        waitingForUsernameInput = false;
                        
                        // 处理用户名输入
                        if (choice == 1) { // 注册
                            if (gameManager.registerUser(username)) {
                                inputPrompt.setString("注册成功!");
                            } else {
                                inputPrompt.setString("用户名已存在!");
                            }
                        } else if (choice == 2) { // 登录
                            if (gameManager.loginUser(username)) {
                                inputPrompt.setString("登录成功!");
                            } else {
                                inputPrompt.setString("用户名不存在!");
                            }
                        }
                    } 
                    else if (event.key.code == sf::Keyboard::BackSpace && !inputBuffer.empty()) {
                        inputBuffer.pop_back();
                    }
                } else {
                    // 主菜单选择
                    if (event.key.code == sf::Keyboard::Num1 || event.key.code == sf::Keyboard::Numpad1) {
                        choice = 1;
                        waitingForUsernameInput = true;
                        inputPrompt.setString("请输入用户名: ");
                    } 
                    else if (event.key.code == sf::Keyboard::Num2 || event.key.code == sf::Keyboard::Numpad2) {
                        choice = 2;
                        waitingForUsernameInput = true;
                        inputPrompt.setString("请输入用户名: ");
                    } 
                    else if (event.key.code == sf::Keyboard::Num3 || event.key.code == sf::Keyboard::Numpad3) {
                        // 游戏模式选择子菜单
                        bool modeSelection = true;
                        sf::Text modeText;
                        modeText.setFont(font);
                        modeText.setCharacterSize(30);
                        modeText.setFillColor(sf::Color::White);
                        modeText.setString("\n\n选择游戏模式:\n1. 简单单人模式\n2. 复杂单人模式\n3. 简单多人模式\n4. 复杂多人模式\n5. 返回");
                        modeText.setPosition(window.getSize().x / 2 - modeText.getLocalBounds().width / 2, 150);
                        
                        while (modeSelection && window.isOpen()) {
                            sf::Event modeEvent;
                            while (window.pollEvent(modeEvent)) {
                                if (modeEvent.type == sf::Event::Closed) {
                                    window.close();
                                    running = false;
                                    modeSelection = false;
                                }
                                
                                if (modeEvent.type == sf::Event::KeyPressed) {
                                    if (modeEvent.key.code == sf::Keyboard::Num1 || modeEvent.key.code == sf::Keyboard::Numpad1) {
                                        gameManager.initializeGame(GameManager::SIMPLE_SINGLE);
                                        modeSelection = false;
                                        // 运行游戏循环
                                        runGameLoop(gameManager, window);
                                    } 
                                    else if (modeEvent.key.code == sf::Keyboard::Num2 || modeEvent.key.code == sf::Keyboard::Numpad2) {
                                        gameManager.initializeGame(GameManager::COMPLEX_SINGLE);
                                        modeSelection = false;
                                        // 运行游戏循环
                                        runGameLoop(gameManager, window);
                                    } 
                                    else if (modeEvent.key.code == sf::Keyboard::Num3 || modeEvent.key.code == sf::Keyboard::Numpad3) {
                                        gameManager.initializeGame(GameManager::SIMPLE_MULTI);
                                        modeSelection = false;
                                        // 运行游戏循环
                                        runGameLoop(gameManager, window);
                                    } 
                                    else if (modeEvent.key.code == sf::Keyboard::Num4 || modeEvent.key.code == sf::Keyboard::Numpad4) {
                                        gameManager.initializeGame(GameManager::COMPLEX_MULTI);
                                        modeSelection = false;
                                        // 运行游戏循环
                                        runGameLoop(gameManager, window);
                                    } 
                                    else if (modeEvent.key.code == sf::Keyboard::Num5 || modeEvent.key.code == sf::Keyboard::Numpad5 || 
                                             modeEvent.key.code == sf::Keyboard::Escape) {
                                        modeSelection = false;
                                    }
                                }
                            }
                            
                            window.clear(sf::Color(50, 50, 50));
                            window.draw(titleText);
                            window.draw(modeText);
                            window.display();
                        }
                    } 
                    else if (event.key.code == sf::Keyboard::Num4 || event.key.code == sf::Keyboard::Numpad4 ||
                             event.key.code == sf::Keyboard::Escape) {
                        running = false;
                    }
                }
            }
            
            // 处理文本输入
            if (waitingForUsernameInput && event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128 && event.text.unicode != '\b' && event.text.unicode != '\r' && event.text.unicode != '\n') {
                    inputBuffer += static_cast<char>(event.text.unicode);
                }
            }
        }
        
        // 更新输入提示
        if (waitingForUsernameInput) {
            inputPrompt.setString("请输入用户名: " + inputBuffer);
        }
        inputPrompt.setPosition(window.getSize().x / 2 - inputPrompt.getLocalBounds().width / 2, 400);
        
        // 绘制
        window.clear(sf::Color(50, 50, 50));
        window.draw(titleText);
        window.draw(menuText);
        window.draw(inputPrompt);
        window.display();
    }
    
    return 0;
} 