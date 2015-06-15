#ifndef _GAME_HUB_H_
#define _GAME_HUB_H_

#include "cocos2d.h"

class GameHub : public cocos2d::Layer {
private:
    cocos2d::Sprite* goldCoin;
    cocos2d::Sprite* xGoldCoin;
    cocos2d::Sprite* n1GoldCoin;
    cocos2d::Sprite* n2GoldCoin;
    cocos2d::Sprite* textbox;
    cocos2d::Label* label;
    std::queue<std::string> textQueue;
    GameHub(){};

public:
    
    void update(float delta);
    virtual bool init();

    // Returns true iff there are no more text to be displayed
    bool isDone();
    
    void clearText();
    void setCoins(int number);

    // Sets the queue for text to be displayed.
    void setText(std::queue<std::string> text);

    // Empty onKeyPressed, we are not interested in keypresses
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event){};
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    CREATE_FUNC(GameHub);
};

#endif