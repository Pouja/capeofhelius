#include "Cloud.h"

USING_NS_CC;

Cloud* Cloud::create(cocos2d::Vec2 startPosition) {
    Cloud* cloud = new Cloud(startPosition);
    if (cloud && cloud->initWithFile("cloud1.png")) {
        cloud->autorelease();
        cloud->reset();
        return cloud;
    }
    CC_SAFE_DELETE(cloud);
    return nullptr;
}

Cloud::Cloud(Vec2 startPosition) {
    this->setPosition(startPosition);
    this->startPosition = startPosition;
}

void Cloud::reset() {
    this->setOpacity(0);
    this->stopAllActions();
    this->setPosition(this->startPosition);
    this->animate();
}

void Cloud::animate() {
    FadeIn* fadeIn = FadeIn::create(2.0f);
    MoveTo* action = cocos2d::MoveTo::create(100, Vec2(0, startPosition.y));
    CallFunc* callback = CallFunc::create([this]() {
        this->reset();
    });
    Sequence* seq = Sequence::create(fadeIn, action, callback, NULL);
    this->runAction(seq);
}