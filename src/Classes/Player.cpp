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
    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        this->move(Vec2(-1,0));
        break;
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        this->move(Vec2(0,-1));
        break;
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
        this->move(Vec2(0,1));
        break;
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        this->move(Vec2(1,0));
        break;
    default:
        break;
    }
}

Vec2 Player::getPosition(){
    return this->playerSprite->getPosition();
}

void Player::move(Vec2 direction){
    Vec2 currentPosition = this->playerSprite->getPosition();
    direction.scale(50.0);
    currentPosition.add(direction);
    this->playerSprite->setPosition(currentPosition);
}

bool Player::isKeyTransparent() {
    return false;
}

Player::~Player() {
    if (this->playerSprite) {
        this->playerSprite->release();
    }
}