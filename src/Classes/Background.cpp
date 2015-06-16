#include "Background.h"

USING_NS_CC;

Background* Background::create(const std::string& filename, Size mapSize) {
	Background* bg = new Background(mapSize);
	if (bg && bg->initWithFile(filename)) {
		bg->setPosition(Vec2(bg->getContentSize().width / 2, mapSize.height / 2));
		bg->autorelease();
		return bg;
	}
	CC_SAFE_DELETE(bg);
	return nullptr;
}

void Background::move(Vec2 vpc) {
	Size size = this->getContentSize();

	float screenWidth = Director::getInstance()->getWinSize().width;

	float maxBgTravel = size.width - screenWidth;
	float maxVpcTravel = mapSize.width - screenWidth;
	float minVpc = screenWidth / 2;

	float newX = vpc.x - (vpc.x * maxBgTravel / maxVpcTravel) + size.width / 2;

	this->setPosition(newX, this->getPosition().y);
}