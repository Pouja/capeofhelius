#include "AirEnemy.h"

USING_NS_CC;

void AirEnemy::animate() {
    this->stopAllActions();

    int flipped = (isFlippedX()) ? 1 : -1;
    float stepSize = getStepSize().width / 2;
    bool stay = RandomHelper::random_int(1, 4) == 1;

    float distanceX = (stay) ? 0 : flipped * stepSize;

    MoveBy* moveUp = MoveBy::create(0.5, Vec2(distanceX, -10));
    MoveBy* moveDown = MoveBy::create(0.5, Vec2(distanceX, 10));

    CallFunc* altFunc = CallFunc::create([this] {
        SpriteFrameCache* cache = SpriteFrameCache::getInstance();
        std::string frame = getName() + ((this->alt) ? "_fly.png" : ".png");
        this->setSpriteFrame(cache->getSpriteFrameByName(frame));
        this->alt = !this->alt;
    });

    CallFunc* reverse = CallFunc::create([this, stay] {
        if(!stay){
            this->setFlippedX(!this->isFlippedX());
        }

        this->animate();
    });

    int steps = (stay) ? RandomHelper::random_int(1, 6) : getSteps().x;

    Sequence* animation = Sequence::create(moveUp, altFunc, moveDown, altFunc, nullptr);
    Repeat* repeat = Repeat::create(animation, steps);
    Sequence* sequence = Sequence::create(repeat, reverse, nullptr);
    this->runAction(sequence);
}
