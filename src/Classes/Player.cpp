#include "Player.h"

#define GRAVITY -1.0
#define FRICTION 5.0
#define HORIZONTAL_FORCE 5.0
#define VERTICAL_FORCE 20.0
#define MAX_HORIZONTAL_VELOCITY 10.0
#define MAX_VERTICAL_VELOCITY 20.0
#define MIN_HORIZONTAL_VELOCITY -10.0
#define MIN_VERTICAL_VELOCITY -15.0

USING_NS_CC;

Player::Player(cocos2d::Vec2 position) {
    this->velocity = cocos2d::Vec2::ZERO;
    this->playerState = cocos2d::Vec2::ZERO;
    this->initWithFile("CloseNormal.png");
    this->setPosition(position);
    this->desiredPosition = position;
    this->isOnGround = false;
}

std::vector<cocos2d::Vec2> Player::getBoundingPoints(Vec2 pov) {
    std::vector<Vec2> points;
    Rect boundingBox = this->getBoundingBox();

    float left = pov.x - boundingBox.size.width / 2;
    float right = pov.x + (boundingBox.size.width / 2);
    float top = pov.y + (boundingBox.size.height / 2);
    float bottom = pov.y - (boundingBox.size.height / 2);

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
    Vec2 gravity(0.0, GRAVITY);
    this->velocity.add(gravity);

    Vec2 horizontalForce(HORIZONTAL_FORCE, 0.0);
    horizontalForce.scale(this->playerState.x);

    Vec2 verticalForce(0.0, VERTICAL_FORCE);
    verticalForce.scale(this->playerState.y);

    if (this->isOnGround) {
        this->velocity.add(verticalForce);
        horizontalForce.scale(1/FRICTION);
        this->velocity.add(horizontalForce);
    } else if (this->playerState.x == 0 || (this->playerState.x == -1 && this->velocity.x >= 0) || (this->playerState.x == 1 && this->velocity.x <= 0)) {
        this->velocity.add(horizontalForce);
    }
    if (this->playerState.x == 0) {
        this->velocity = Vec2(this->velocity.x * 0.1, this->velocity.y);
    }

    Vec2 minMovement(MIN_HORIZONTAL_VELOCITY, MIN_VERTICAL_VELOCITY);
    Vec2 maxMovement(MAX_HORIZONTAL_VELOCITY, MAX_VERTICAL_VELOCITY);

    this->velocity.clamp(minMovement, maxMovement);
    this->desiredPosition = this->getPosition() + this->velocity;
}


Vec2 Player::getState() {
    return this->playerState;
}

Vec2 Player::getDesiredPosition() {
    return this->desiredPosition;
}

Player::~Player() {
    this->release();
}