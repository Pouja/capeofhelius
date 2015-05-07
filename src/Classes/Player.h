#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "IKeyListener.h"
#include "cocos2d.h"

class Player: public IKeyListener, public cocos2d::Node {
private:
    cocos2d::Sprite* playerSprite;
public:
    Player();
    cocos2d::Sprite* init(cocos2d::Vec2 position);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
    bool isKeyTransparent();
    ~Player();
};

#endif