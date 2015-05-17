#ifndef _GAME_HUB_H_
#define _GAME_HUB_H_

#include "cocos2d.h"

class GameHub : public cocos2d::Layer {
private:
    cocos2d::Label* label;
    std::queue<std::string> textQueue;
public:
    GameHub();
    
    void update(float delta);
    virtual bool init();

    bool isDone();

    void setText(std::queue<std::string> text);

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event){};
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    CREATE_FUNC(GameHub);
};

#endif