#include "Player.h"

USING_NS_CC;

Player::Player() {
    this->velocity = cocos2d::Vec2::ZERO;
    this->playerState = PlayerState::IDLE;
}

Sprite* Player::init(cocos2d::Vec2 position) {
    this->playerSprite = Sprite::create("CloseNormal.png");
    this->playerSprite->setPosition(position);
    return this->playerSprite;
}

void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event)
{
    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        this->playerState = PlayerState::MOVING_LEFT;
        break;
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
        this->playerState = PlayerState::MOVING_JUMP;
        break;
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        this->playerState = PlayerState::MOVING_RIGHT;
        break;
    default:
        break;
    }
}

void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event)
{
    this->playerState = PlayerState::IDLE;
}

void Player::update(float delta) {
    switch (this->playerState) {
    case PlayerState::MOVING_LEFT:
        this->move(Vec2(-1, 0));
        break;
    case PlayerState::MOVING_RIGHT:
        this->move(Vec2(1, 0));
        break;
    case PlayerState::MOVING_JUMP:
        this->move(Vec2(0, 1));
        break;
    default: break;
    }
}

Vec2 Player::getPosition() {
    return this->playerSprite->getPosition();
}

void Player::move(Vec2 direction) {
    Vec2 currentPosition = this->playerSprite->getPosition();
    direction.scale(5.0);
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