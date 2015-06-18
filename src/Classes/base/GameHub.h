#ifndef _GAME_HUB_H_
#define _GAME_HUB_H_

#include "cocos2d.h"
#include "utils/CCLabelBMFontAnimated.h"

class GameHub : public cocos2d::Layer {
private:
    cocos2d::Sprite* goldCoin;
    cocos2d::Sprite* xGoldCoin;
    cocos2d::Sprite* n1GoldCoin;
    cocos2d::Sprite* n2GoldCoin;
    cocos2d::Sprite* lives;
    cocos2d::Sprite* textbox;
    cocos2d::Sprite* pulser;
    CCLabelBMFontAnimated* label;
    std::queue<std::string> textQueue;
    std::function<void()> callback;
    bool block;
    GameHub() : block(false){};

public:
    virtual bool init();

    void toggleHud();

    void clearText();
    void setCoins(int number);

    void setLives(int lives);

    // Sets the queue for text to be displayed.
    void setText(std::queue<std::string> text, std::function<void()> onFinish);
    void setText(std::queue<std::string> text);

    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    CREATE_FUNC(GameHub);
};

#endif
