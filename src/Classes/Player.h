#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "IKeyListener.h"
#include "cocos2d.h"

class Player: public IKeyListener, public cocos2d::Sprite {
public:

    Player();
    void init(cocos2d::Vec2 position);
    
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
    bool isKeyTransparent();

    std::vector<cocos2d::Vec2> getBoundingPoints(cocos2d::Vec2 pov);
    
    void update(float delta);
    void move(cocos2d::Vec2 direction, float delta);
    cocos2d::Vec2 getState();

    cocos2d::Vec2 desiredPosition;
    cocos2d::Vec2 velocity;
    bool isOnGround;

    ~Player();
private:
    cocos2d::Vec2 playerState;
};

#endif