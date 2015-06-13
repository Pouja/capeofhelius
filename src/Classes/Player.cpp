#include "Player.h"

#define SCALE 1.0
#define GRAVITY -1.0
#define FRICTION 5.0
#define HORIZONTAL_FORCE 2.0
#define VERTICAL_FORCE 20.0
#define MAX_HORIZONTAL_VELOCITY 10.0
#define MAX_VERTICAL_VELOCITY 20.0
#define MIN_HORIZONTAL_VELOCITY -10.0
#define MIN_VERTICAL_VELOCITY -15.0
#define BOTTOM_OFFSET 3

USING_NS_CC;

Player* Player::create(Vec2 position) {
    Player* player = new Player(position);
    if (player && player->initWithSpriteFrameName("right.png")) {
        player->autorelease();
        player->initAnimations();
        return player;
    }

    CC_SAFE_DELETE(player);
    return nullptr;
}

Player::Player(cocos2d::Vec2 position) {
    this->animationState = AnimationState::IDLE_RIGHT;
    this->velocity = cocos2d::Vec2::ZERO;
    this->movingState = cocos2d::Vec2::ZERO;
    this->setScale(SCALE);
    this->setPosition(position);
    this->desiredPosition = position;
    this->isOnGround = false;
}

void Player::addCoin() {    
    this->coins++;
}

int Player::getScore() {
    return this->coins;
}

void Player::initAnimations() {
    float initHeigh = this->getContentSize().height;
    this->setAnchorPoint(Vec2(0.5, ((initHeigh / 2) + BOTTOM_OFFSET) / initHeigh));

    SpriteFrameCache* cache = SpriteFrameCache::getInstance();

    Vector<SpriteFrame*> walkLeftFrames(9);

    char str[100] = {0};
    for (int i = 1; i < 9; i++)
    {
        sprintf(str, "walk_left/%d.png", i);
        SpriteFrame* frame = cache->getSpriteFrameByName( str );
        walkLeftFrames.pushBack(frame);
    }
    Animation* walkLeftAnimation = Animation::createWithSpriteFrames(walkLeftFrames, 0.05);
    this->walkLeft = Animate::create(walkLeftAnimation);
    this->walkLeft->retain();

    Animation* runningLeftAnimation = Animation::createWithSpriteFrames(walkLeftFrames, 0.025);
    this->runningLeft = Animate::create(runningLeftAnimation);
    this->runningLeft->retain();

    Vector<SpriteFrame*> walkRightFrames(9);
    for (int i = 1; i < 9; i++)
    {
        sprintf(str, "walk_right/%d.png", i);
        SpriteFrame* frame = cache->getSpriteFrameByName( str );
        walkRightFrames.pushBack(frame);
    }
    Animation* walkRightAnimation = Animation::createWithSpriteFrames(walkRightFrames, 0.05);
    this->walkRight = Animate::create(walkRightAnimation);
    this->walkRight->retain();

    Animation* runningRightAnimation = Animation::createWithSpriteFrames(walkRightFrames, 0.025);
    this->runningRight = Animate::create(runningRightAnimation);
    this->runningRight->retain();
}

void Player::updateAnimation() {
    if (this->movingState.isZero()) {
        if (this->isOnGround && !(this->animationState == AnimationState::IDLE_LEFT
                                  || this->animationState == AnimationState::IDLE_RIGHT)) {
            this->stopAllActions();
            if (this->animationState == JUMP_LEFT || this->animationState == WALKING_LEFT
                    || this->animationState == RUNNING_LEFT) {
                this->setSpriteFrame("left.png");
                this->animationState = IDLE_LEFT;
            } else {
                this->setSpriteFrame("right.png");
                this->animationState = IDLE_RIGHT;
            }
        }
    } else if (this->movingState.y == 0 && this->isOnGround) {
        if (this->movingState.x >= 0 ) {
            if (this->animationState == WALKING_RIGHT && this->velocity.x > 7) {
                this->stopAllActions();
                this->runAction(RepeatForever::create(this->runningRight));
                this->animationState = RUNNING_RIGHT;
            } else if (this->animationState != WALKING_RIGHT && this->animationState != RUNNING_RIGHT) {
                this->stopAllActions();
                this->runAction(RepeatForever::create(this->walkRight));
                this->animationState = WALKING_RIGHT;
            }
        } else {
            if (this->animationState == WALKING_LEFT && this->velocity.x < -7) {
                this->stopAllActions();
                this->runAction(RepeatForever::create(this->runningLeft));
                this->animationState = RUNNING_LEFT;
            } else if (this->animationState != WALKING_LEFT && this->animationState != RUNNING_LEFT) {
                this->stopAllActions();
                this->runAction(RepeatForever::create(this->walkLeft));
                this->animationState = WALKING_LEFT;
            }
        }
    } else {
        this->stopAllActions();
        if (this->movingState.x >= 0 &&
                (animationState == RUNNING_RIGHT || animationState == WALKING_RIGHT || animationState == IDLE_RIGHT) ) {
            this->setSpriteFrame("right_jump.png");
            this->animationState = JUMP_RIGHT;
        } else if (animationState != JUMP_RIGHT) {
            this->setSpriteFrame("left_jump.png");
            this->animationState = JUMP_LEFT;
        }
    }
}

std::vector<cocos2d::Vec2> Player::getBoundingPoints(Vec2 pov) {
    std::vector<Vec2> points;
    Rect boundingBox = this->getBoundingBox();

    float left = pov.x - boundingBox.size.width / 2;
    float right = pov.x + (boundingBox.size.width / 2);
    float top = pov.y + (boundingBox.size.height / 2);
    float bottom = pov.y - (boundingBox.size.height / 2) - BOTTOM_OFFSET;

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
        this->movingState.x = -1;
        break;
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
        this->movingState.y = 1;
        break;
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        this->movingState.x = 1;
        break;
    default:
        break;
    }
}

void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event)
{
    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        this->movingState.x = fmaxf(this->movingState.x, 0);
        break;
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
        this->movingState.y = 0;
        break;
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        this->movingState.x = fminf(this->movingState.x, 0);
        break;
    default:
        break;
    }
}

void Player::updatePhysics() {
    Vec2 gravity(0.0, GRAVITY);
    this->velocity.add(gravity);

    Vec2 horizontalForce(HORIZONTAL_FORCE, 0.0);
    horizontalForce.scale(this->movingState.x);

    Vec2 verticalForce(0.0, VERTICAL_FORCE);
    verticalForce.scale(this->movingState.y);

    if (this->isOnGround) {
        this->velocity.add(verticalForce);
        horizontalForce.scale(1 / FRICTION);
        this->velocity.add(horizontalForce);
    } else if (this->movingState.x == 0 || (this->movingState.x == -1 && this->velocity.x >= 0) || (this->movingState.x == 1 && this->velocity.x <= 0)) {
        this->velocity.add(horizontalForce);
    }
    if (this->movingState.x == 0 && this->isOnGround) {
        this->velocity = Vec2(this->velocity.x * 0.1, this->velocity.y);
    }

    Vec2 minMovement(MIN_HORIZONTAL_VELOCITY, MIN_VERTICAL_VELOCITY);
    Vec2 maxMovement(MAX_HORIZONTAL_VELOCITY, MAX_VERTICAL_VELOCITY);

    this->velocity.clamp(minMovement, maxMovement);
    this->desiredPosition = this->getPosition() + this->velocity;
}


Vec2 Player::getState() {
    return this->movingState;
}

Vec2 Player::getDesiredPosition() {
    return this->desiredPosition;
}

Player::~Player() {
    CC_SAFE_RELEASE(walkLeft);
    CC_SAFE_RELEASE(runningLeft);
    CC_SAFE_RELEASE(runningRight);
    CC_SAFE_RELEASE(walkRight);
}