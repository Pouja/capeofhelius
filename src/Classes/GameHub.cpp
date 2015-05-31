#include "GameHub.h"

USING_NS_CC;

GameHub::GameHub() {}

bool GameHub::init() {
    Director* director = Director::getInstance();
    Size contentSize = director->getVisibleSize();

    this->n1GoldCoin = cocos2d::Sprite::createWithSpriteFrameName("hud_0.png");
    this->n1GoldCoin->setPosition(contentSize.width * 0.8, contentSize.height * 0.9);

    this->n2GoldCoin = cocos2d::Sprite::createWithSpriteFrameName("hud_0.png");
    this->n2GoldCoin->setPosition(contentSize.width * 0.8 + 40, contentSize.height * 0.9);

    xGoldCoin = cocos2d::Sprite::createWithSpriteFrameName("hud_x.png");
    xGoldCoin->setPosition(contentSize.width * 0.8 + 80, contentSize.height * 0.9);

    goldCoin = cocos2d::Sprite::createWithSpriteFrameName("hud_coins.png");
    goldCoin->setPosition(contentSize.width * 0.8 + 120, contentSize.height * 0.9);

    this->textbox = cocos2d::Sprite::create("textbox.png");
    this->textbox->setScaleX(contentSize.width / this->textbox->getContentSize().width);

    this->label = Label::createWithTTF("", "kenpixel.ttf", 12);
    this->label->setColor(cocos2d::Color3B::BLACK);
    this->label->setWidth(contentSize.width * 0.5);

    this->addChild(this->n1GoldCoin, 1);
    this->addChild(this->n2GoldCoin, 1);
    this->addChild(goldCoin, 1);
    this->addChild(xGoldCoin, 1);
    this->addChild(this->textbox, 1);
    this->addChild(this->label, 2);
    return true;
}

void GameHub::setCoins(int number) {
    int secondDigit = number % 10;
    int firstDigit = (number >= 10) ? (number - secondDigit) / 10 : 0;

    char str[100] = {0};
    sprintf(str, "hud_%d.png", firstDigit);
    n1GoldCoin->setSpriteFrame(str);

    sprintf(str, "hud_%d.png", secondDigit);
    n2GoldCoin->setSpriteFrame(str);

    auto goUp = MoveBy::create(0.05f, Vec2(0,20));
    auto goDown = MoveBy::create(0.1f, Vec2(0,-20));
    auto seq = Sequence::create(goUp, goDown, nullptr);
    goldCoin->runAction(seq);
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