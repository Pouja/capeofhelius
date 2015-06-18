#include "GameHub.h"

USING_NS_CC;

bool GameHub::init() {
    Director* director = Director::getInstance();
    Size contentSize = director->getVisibleSize();

    Vec2 topRightOffset(contentSize.width * 0.8, contentSize.height * 0.9);
    this->n1GoldCoin = cocos2d::Sprite::createWithSpriteFrameName("hud_0.png");
    this->n1GoldCoin->setPosition(contentSize.width * 0.8, topRightOffset.y);

    this->n2GoldCoin = cocos2d::Sprite::createWithSpriteFrameName("hud_0.png");
    this->n2GoldCoin->setPosition(topRightOffset.x + 40, topRightOffset.y);

    xGoldCoin = cocos2d::Sprite::createWithSpriteFrameName("hud_x.png");
    xGoldCoin->setPosition(topRightOffset.x + 80, topRightOffset.y);

    goldCoin = cocos2d::Sprite::createWithSpriteFrameName("hud_coins.png");
    goldCoin->setPosition(topRightOffset.x + 120, topRightOffset.y);

    lives = Sprite::createWithSpriteFrameName("hud_heartFull.png");
    lives->setPosition(contentSize.width * 0.1, topRightOffset.y);

    this->textbox = cocos2d::Sprite::create("tiles/textbox.png");
    this->textbox->setScaleX(contentSize.width / this->textbox->getContentSize().width);

    this->label = CCLabelBMFontAnimated::createWithTTF("","fonts/Gasalt-Regular.ttf", 30,
        Size(contentSize.width, 40), TextHAlignment::LEFT, TextVAlignment::TOP);
    this->label->setTextColor(Color4B::BLACK);

    this->pulser = Sprite::create("arrowSilver_right.png");
    this->pulser->setRotation(90.0f);
    this->label->setPosition(contentSize.width * 0.55, 30);
    this->textbox->setPosition(contentSize.width / 2, 45);
    pulser->setPosition(contentSize.width * 0.035, 40);

    pulser->runAction(RepeatForever::create(Sequence::create(FadeOut::create(1), FadeIn::create(1), nullptr)));
    this->addChild(lives, 1);
    this->addChild(this->n1GoldCoin, 1);
    this->addChild(this->n2GoldCoin, 1);
    this->addChild(goldCoin, 1);
    this->addChild(xGoldCoin, 1);
    this->addChild(this->textbox, 1);
    this->addChild(this->label, 2);
    this->addChild(pulser, 3);

    std::queue<std::string> v({"Mom: \"Are you already up? Wake up!\"",
            "Unknown: \"Mh... just 5 more minutes...\" *Falls asleep*",
            "Mom: \"Zoe! It's gettting late! Wake up now!\"",
            "Zoe: \"Allright, allright, I'm comming\""});
    this->setText(v);
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

    auto goUp = MoveBy::create(0.05f, Vec2(0, 20));
    auto goDown = MoveBy::create(0.1f, Vec2(0, -20));
    auto seq = Sequence::create(goUp, goDown, nullptr);
    goldCoin->runAction(seq);
}

void GameHub::update(float delta) {
    Director* director = Director::getInstance();
    Size contentSize = director->getVisibleSize();

}

void GameHub::setLives(int nLives) {
    assert(nLives <= 2 && nLives >= 0);
    if (nLives == 2) {
        lives->setSpriteFrame("hud_heartFull.png");
    } else if (nLives == 1) {
        lives->setSpriteFrame("hud_heartHalf.png");
    } else {
        lives->setSpriteFrame("hud_heartEmpty.png");
    }
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
            this->label->animateInTypewriter(1);
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
