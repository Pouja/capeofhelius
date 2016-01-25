#include "TitleScreen.h"

USING_NS_CC;

TitleScreen* TitleScreen::create(const std::string& title, const std::string& message, bool withDim) {
	TitleScreen* titleScreen = new TitleScreen();
	if (titleScreen && titleScreen->load(title, message, withDim)) {
		titleScreen->autorelease();
		return titleScreen;
	}

	CC_SAFE_DELETE(titleScreen);
	return nullptr;
}

bool TitleScreen::load(const std::string& title, const std::string& message, bool withDim) {
	Director* director = Director::getInstance();
	Size contentSize = director->getVisibleSize();

	this->title = Label::createWithTTF(title, "fonts/Gasalt-Regular.ttf", 40, Size(200,200), TextHAlignment::CENTER);
	this->title->setColor(cocos2d::Color3B::WHITE);
	this->title->setWidth(contentSize.width * 0.5);
	this->title->setPosition(Vec2(contentSize.width / 2, contentSize.height / 2));

	this->message = Label::createWithTTF(message, "fonts/Gasalt-Regular.ttf", 20, Size(200,200), TextHAlignment::CENTER);
	this->message->setColor(cocos2d::Color3B::WHITE);
	this->message->setWidth(contentSize.width * 0.5);
	this->message->setPosition(Vec2(contentSize.width / 2, contentSize.height / 2 - 50));

	float transparency = (withDim) ? 170 : 255;

	addChild(cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 0, transparency), contentSize.width, contentSize.height));
	addChild(this->title);
	addChild(this->message);
	return true;
}

void TitleScreen::setText(const std::string& title, const std::string& message) {
	this->message->setString(message);
	this->title->setString(title);
}
