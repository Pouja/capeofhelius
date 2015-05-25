#include "GameHub.h"

USING_NS_CC;

GameHub::GameHub() {}

bool GameHub::init() {
    Director* director = Director::getInstance();
    Size contentSize = director->getVisibleSize();

    Sprite* hud3 = cocos2d::Sprite::createWithSpriteFrameName("hud_3.png");
    hud3->setPosition(contentSize.width * 0.9 - 30, contentSize.height * 0.9);

    Sprite* hudX = cocos2d::Sprite::createWithSpriteFrameName("hud_x.png");
    hudX->setPosition(contentSize.width * 0.9, contentSize.height * 0.9);

    Sprite* coin = cocos2d::Sprite::createWithSpriteFrameName("hud_coins.png");
    coin->setPosition(contentSize.width * 0.9 + 40, contentSize.height * 0.9);

    this->textbox = cocos2d::Sprite::create("textbox.png");
    this->textbox->setScaleX(contentSize.width / this->textbox->getContentSize().width);

    this->label = Label::createWithTTF("", "kenpixel.ttf", 12);
    this->label->setColor(cocos2d::Color3B::BLACK);
    this->label->setWidth(contentSize.width * 0.5);

    this->addChild(hud3, 1);
    this->addChild(coin, 1);
    this->addChild(hudX, 1);
    this->addChild(this->textbox, 1);
    this->addChild(this->label, 2);
    return true;
}

void GameHub::update(float delta) {
    Director* director = Director::getInstance();
    Size contentSize = director->getVisibleSize();
    this->label->setPosition(contentSize.width * 0.3, 40);
    this->textbox->setPosition(contentSize.width / 2, 45);
}

bool GameHub::isDone() {
    return this->textQueue.empty();
}

void GameHub::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) {
        if (!this->textQueue.empty()) {
            this->textQueue.pop();
            std::string nextText = (this->textQueue.empty()) ? "" : this->textQueue.front();
            this->label->setString(nextText);
        }
    }
}

void GameHub::clearText() {
    std::queue<std::string>().swap(this->textQueue);
    this->label->setString("");
}

void GameHub::setText(std::queue<std::string> textQueue) {
    this->clearText();
    this->textQueue = textQueue;
    std::string nextText = this->textQueue.front();
    this->label->setString(nextText);
}