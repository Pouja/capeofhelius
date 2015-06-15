#include "TitleScreen.h"

USING_NS_CC;

TitleScreen* TitleScreen::create(const std::string& title, const std::string& message, bool withDim) {
	TitleScreen* titleScreen = new TitleScreen();
	if (titleScreen && titleScreen->init(title, message, withDim)) {
		titleScreen->autorelease();
		return titleScreen;
	}

	CC_SAFE_DELETE(titleScreen);
	return nullptr;
}

bool TitleScreen::init(const std::string& title, const std::string& message, bool withDim) {
	Director* director = Director::getInstance();
	Size contentSize = director->getVisibleSize();

	this->dimScreen = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 0, 170), contentSize.width, contentSize.height);
	this->title = Label::createWithTTF(title, "kenpixel.ttf", 20);
	this->title->setColor(cocos2d::Color3B::BLACK);
	this->title->setWidth(contentSize.width * 0.5);
	this->title->setPosition(Vec2(contentSize.width / 2, contentSize.height / 2));

	this->message = Label::createWithTTF(message, "kenpixel.ttf", 15);
	this->message->setColor(cocos2d::Color3B::BLACK);
	this->message->setWidth(contentSize.width * 0.5);
	this->title->setPosition(Vec2(contentSize.width / 2, (contentSize.height / 2) - 30));

	if (withDim) {
		addChild(this->dimScreen);
	}

	addChild(this->title);
	addChild(this->message);
	return true;
}

void TitleScreen::setText(const std::string& title, const std::string& message) {
	this->message->setString(message);
	this->title->setString(title);
}
