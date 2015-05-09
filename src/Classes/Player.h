#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "IKeyListener.h"
#include "cocos2d.h"

class Player: public IKeyListener, public cocos2d::Node {
public:
    enum class PlayerState{
        MOVING_LEFT,
        MOVING_RIGHT,
        MOVING_JUMP,
        IDLE
    };

    Player();
    cocos2d::Sprite* init(cocos2d::Vec2 position);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

    bool isKeyTransparent();

    cocos2d::Vec2 getPosition();

    void update(float delta);

    //Moves
    void move(cocos2d::Vec2 direction);
    ~Player();
private:
    cocos2d::Vec2 velocity;
    PlayerState playerState;
    cocos2d::Sprite* playerSprite;
};

#endif