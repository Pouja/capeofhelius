#include "Player.h"

USING_NS_CC;

Player::Player() {
    this->velocity = cocos2d::Vec2::ZERO;
    this->playerState = cocos2d::Vec2::ZERO;
}

std::vector<cocos2d::Vec2> Player::getBoundingPoints(Vec2 pov) {
    int offset = 0;
    std::vector<Vec2> points;
    Rect boundingBox = this->getBoundingBox();

    float left = std::fmax(pov.x - (boundingBox.size.width / 2) - offset, 0);
    float right = pov.x + (boundingBox.size.width / 2) + offset;
    float top = pov.y + (boundingBox.size.height / 2) + offset;
    float bottom = pov.y - (boundingBox.size.height / 2) - offset;

    points.push_back(Vec2(pov.x, bottom));
    points.push_back(Vec2(pov.x, top));
    points.push_back(Vec2(left, pov.y));
    points.push_back(Vec2(right, pov.y));
    points.push_back(Vec2(left, top));
    points.push_back(Vec2(left, bottom));
    points.push_back(Vec2(right, bottom));
    points.push_back(Vec2(right, top));
    return points;
}

void Player::init(cocos2d::Vec2 position) {
    this->initWithFile("CloseNormal.png");
    this->setPosition(position);
    this->desiredPosition = position;
    this->isOnGround = true;
}

void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event)
{
    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        this->playerState.x = -1;
        break;
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
        this->playerState.y = 1;
        break;
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        this->playerState.x = 1;
        break;
    default:
        break;
    }
}

void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event)
{
    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        this->playerState.x = fmaxf(this->playerState.x, 0);
        break;
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
        this->playerState.y = 0;
        break;
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        this->playerState.x = fminf(this->playerState.x, 0);
        break;
    default:
        break;
    }
}

void Player::update(float delta) {
    Vec2 gravity(0.0, -1);
    this->velocity.add(gravity);

    Vec2 horizontalForce(1, 0.0);
    horizontalForce.scale(this->playerState.x);

    Vec2 verticalForce(0.0, 20);
    verticalForce.scale(this->playerState.y);

    if (this->isOnGround) {
        this->velocity.add(verticalForce);
        this->velocity.add(horizontalForce);
    } else if (this->playerState.x == 0 || (this->playerState.x == -1 && this->velocity.x >= 0) || (this->playerState.x == 1 && this->velocity.x <= 0)) {
        horizontalForce.scale(5);
        this->velocity.add(horizontalForce);
    }
    if (this->playerState.x == 0) {
        this->velocity = Vec2(this->velocity.x * 0.1, this->velocity.y);
    }

    Vec2 minMovement(-10, -15);
    Vec2 maxMovement(10, 15);

    this->velocity.clamp(minMovement, maxMovement);
    this->desiredPosition = this->getPosition() + this->velocity;
}


Vec2 Player::getState() {
    return this->playerState;
}

bool Player::isKeyTransparent() {
    return false;
}

Player::~Player() {
    this->release();
}