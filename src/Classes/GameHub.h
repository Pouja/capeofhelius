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

    // Returns true iff there are no more text to be displayed
    bool isDone();

    // Sets the queue for text to be displayed.
    void setText(std::queue<std::string> text);

    // Empty onKeyPressed, we are not interested in keypresses
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event){};
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    CREATE_FUNC(GameHub);
};

#endif