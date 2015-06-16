#include "Torch.h"

USING_NS_CC;

Torch* Torch::create(cocos2d::Vec2 position) {
	Torch* torch = new Torch(position);
	if (torch && torch->initWithFile("tiles/base_pack.png", Rect(72, 142, 70, 70))) {
		torch->autorelease();
		torch->start();
		return torch;
	}
	CC_SAFE_DELETE(torch);
	return nullptr;
}

Torch::Torch(Vec2 postion) {
	this->setPosition(postion);
}

void Torch::start() {
	Vector<SpriteFrame*> animFrames;
	animFrames.reserve(2);
	animFrames.pushBack(cocos2d::SpriteFrame::create("tiles/base_pack.png", Rect(72, 142, 70, 70)));
	animFrames.pushBack(cocos2d::SpriteFrame::create("tiles/base_pack.png", Rect(72, 214, 70, 70)));
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 1.0f);
	Animate* animate = Animate::create(animation);
	this->runAction(RepeatForever::create(animate));
}

void Torch::stop() {
	this->stopAllActions();
}