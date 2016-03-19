#include "Player.h"

#define SCALE 1.0
#define BOTTOM_OFFSET 8.0f

USING_NS_CC;
Player* Player::create(Vec2 position, const std::string& name) {
    Player* player = new Player(position, name);
    if (player && player->initWithSpriteFrameName(name + "-walk-right/0.png")) {
        player->autorelease();
        player->initAnimations();
        return player;
    }

    CC_SAFE_DELETE(player);
    return nullptr;
}

Player::Player(cocos2d::Vec2 position, const std::string& name) {
    this->animationState = AnimationState::IDLE;
    this->movingState = cocos2d::Vec2::ZERO;
    this->setScale(SCALE);
    this->setPosition(position);
    this->lives = 4;
    this->coins = 0;
    this->name = name;
    this->physicBody = PhysicComponent::create(this, PhysicComponent::Type::PLAYER);
    this->physicBody->setOffsets(BOTTOM_OFFSET, 0.0f, 0.0f, 0.0f);
}

PhysicComponent* Player::getPhysicBody(){
    return this->physicBody;
}

void Player::stop() {
    this->movingState = Vec2::ZERO;
    this->updateAnimation();
}
void Player::addCoin() {
    this->coins++;
}

int Player::getScore() {
    return this->coins;
}

int Player::getLives() {
    return this->lives;
}

void Player::initAnimations() {
    float initHeigh = this->getContentSize().height;
    this->setAnchorPoint(Vec2(0.5, ((initHeigh / 2) + BOTTOM_OFFSET) / initHeigh));

    SpriteFrameCache* cache = SpriteFrameCache::getInstance();

    char str[100] = {0};
    Vector<SpriteFrame*> walkRightFrames(9);
    for (int i = 1; i < 9; i++)
    {
        sprintf(str, (this->name + "-walk-right/%d.png").c_str(), i);
        SpriteFrame* frame = cache->getSpriteFrameByName( str );
        walkRightFrames.pushBack(frame);
    }
    Animation* walkRightAnimation = Animation::createWithSpriteFrames(walkRightFrames, 0.05);
    this->walking = Animate::create(walkRightAnimation);
    this->walking->retain();

    Animation* runningRightAnimation = Animation::createWithSpriteFrames(walkRightFrames, 0.025);
    this->running = Animate::create(runningRightAnimation);
    this->running->retain();
}

void Player::updateAnimation() {
    if (movingState.x > 0 && isFlippedX()) {
        this->setFlippedX(false);
    }
    if (movingState.x < 0 && !isFlippedX()) {
        this->setFlippedX(true);
    }
    // log("%s movingState(%f,%f) onground: %d", name.c_str(), movingState.x, movingState.y, physicBody->isOnGround());
    if (this->movingState.isZero()) {
        if (this->physicBody->isOnGround() && this->animationState != AnimationState::IDLE) {
            this->stopAllActions();
            this->setSpriteFrame(this->name + "-walk-right/0.png");
            this->animationState = AnimationState::IDLE;
        }
    } else if (this->movingState.y == 0 && this->physicBody->isOnGround()) {
        if (this->animationState == AnimationState::WALKING && fabsf(this->physicBody->getSpeed().x) > 7) {
            this->stopAllActions();
            this->runAction(RepeatForever::create(this->running));
            this->animationState = AnimationState::RUNNING;
        } else if (this->animationState != AnimationState::WALKING && this->animationState != AnimationState::RUNNING) {
            this->stopAllActions();
            this->runAction(RepeatForever::create(this->walking));
            this->animationState = AnimationState::WALKING;
        }
    } else {
        this->stopAllActions();
        this->setSpriteFrame(this->name + "-walk-right/7.png");
        this->animationState = AnimationState::JUMPING;
    }
}

void Player::clearCoins() {
    this->coins = 0;
}

void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event)
{
    if (!this->targetRect.equals(Rect::ZERO)) return;

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
    this->physicBody->setAngle(this->movingState);
}

void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event)
{
    if (!this->targetRect.equals(Rect::ZERO)) return;

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
    this->physicBody->setAngle(this->movingState);
}

void Player::die(CallFunc* callback) {
    log("die");
    this->lives--;
    this->animationState = AnimationState::IDLE;
    this->movingState = cocos2d::Vec2::ZERO;

    this->stopAllActions();
    FadeOut* fadeOut = FadeOut::create(2);
    Sequence* seq = Sequence::create(fadeOut, callback, nullptr);
    this->runAction(seq);
}

void Player::respawn(Vec2 position) {
    log("respawn");
    this->stopAllActions();

    this->movingState = Vec2::ZERO;
    this->setPosition(position);
    this->setOpacity(255.0f);
}

Vec2 Player::getState() {
    return this->movingState;
}

void Player::moveTo(Rect target, std::function<void()> onFinish) {
    assert(!target.equals(Rect::ZERO));
    log("moveTo");

    this->movingState = Vec2::ZERO;
    this->targetRect = target;
    this->callback = onFinish;

    if (target.origin.x > this->getPosition().x) {
        this->movingState.x = 1;
    } else {
        this->movingState.x = -1;
    }
    this->updateAnimation();
}

void Player::onMoveFinish() {
    this->movingState = Vec2::ZERO;
    this->callback();
    this->targetRect = Rect::ZERO;
    this->callback = nullptr;
}

Player::~Player() {
    CC_SAFE_RELEASE(running);
    CC_SAFE_RELEASE(walking);
}
