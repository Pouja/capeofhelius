#include "Player.h"

USING_NS_CC;

Player::Player() {}

Sprite* Player::init(cocos2d::Vec2 position) {
    this->playerSprite = Sprite::create("CloseNormal.png");
    this->playerSprite->setPosition(position);
    return this->playerSprite;
}

void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event)
{
    log("Key with keycode %d pressed", keyCode);
}

bool Player::isKeyTransparent() {
    return false;
}

Player::~Player() {
    if (this->playerSprite) {
        this->playerSprite->release();
    }
}