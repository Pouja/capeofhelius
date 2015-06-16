#include "GroundEnemy.h"

USING_NS_CC;

GroundEnemy* GroundEnemy::create(cocos2d::Vec2 position) {
    GroundEnemy* groundEnemy = new GroundEnemy();
    if (groundEnemy && groundEnemy->initWithSpriteFrameName("snail.png")) {
        groundEnemy->autorelease();
        groundEnemy->setPosition(position);
        groundEnemy->initAnimation();
        return groundEnemy;
    }
    CC_SAFE_DELETE(groundEnemy);
    return nullptr;
}

void GroundEnemy::initAnimation() {
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();

    Vector<SpriteFrame*> animFrames(2);

    animFrames.pushBack(cache->getSpriteFrameByName("bat.png"));
    animFrames.pushBack(cache->getSpriteFrameByName("bat_fly.png"));

    Animation* anim = Animation::createWithSpriteFrames(animFrames, 0.5);
    this->animation = Animate::create(anim);
    this->runAction(RepeatForever::create(animation));
}

void GroundEnemy::update(float delta){
    this->timeout += delta;
}
