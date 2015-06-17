#include "GroundEnemy.h"

USING_NS_CC;

void GroundEnemy::animate() {
    this->stopAllActions();

    int flipped = (isFlippedX()) ? 1 : -1;
    float step = getStepSize().width / 4;

    MoveBy* move = MoveBy::create(0.5, Vec2(flipped * step, 0));

    CallFunc* altFunc = CallFunc::create([this] {
        SpriteFrameCache* cache = SpriteFrameCache::getInstance();
        std::string frame = getName() + ((this->alt) ? "_walk.png" : ".png");
        this->setSpriteFrame(cache->getSpriteFrameByName(frame));
        this->alt = !this->alt;
    });

    CallFunc* reverse = CallFunc::create([this] {
        this->setFlippedX(!this->isFlippedX());
        this->animate();
    });

    Sequence* animation = Sequence::create(move, altFunc, move, altFunc, nullptr);
    Repeat* repeat = Repeat::create(animation, getSteps().x);
    Sequence* sequence = Sequence::create(repeat, reverse, nullptr);
    this->runAction(sequence);
}
