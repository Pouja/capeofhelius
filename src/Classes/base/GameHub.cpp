#include "GameHub.h"

#define LIVE_TAG 0

USING_NS_CC;

bool GameHub::init() {
    Director* director = Director::getInstance();
    Size contentSize = director->getVisibleSize();

    Vec2 topRightOffset(contentSize.width * 0.8, contentSize.height * 0.9);
    this->n1GoldCoin = cocos2d::Sprite::createWithSpriteFrameName("hud_0.png");
    this->n1GoldCoin->setPosition(contentSize.width * 0.8, topRightOffset.y);

    this->n2GoldCoin = cocos2d::Sprite::createWithSpriteFrameName("hud_0.png");
    this->n2GoldCoin->setPosition(topRightOffset.x + 40, topRightOffset.y);

    this->xGoldCoin = cocos2d::Sprite::createWithSpriteFrameName("hud_x.png");
    this->xGoldCoin->setPosition(topRightOffset.x + 80, topRightOffset.y);

    this->goldCoin = cocos2d::Sprite::createWithSpriteFrameName("hud_coins.png");
    this->goldCoin->setPosition(topRightOffset.x + 120, topRightOffset.y);

    this->textbox = cocos2d::Sprite::create("hud/textbox.png");
    this->textbox->setScaleX(contentSize.width / this->textbox->getContentSize().width);
    this->textbox->setPosition(contentSize.width / 2, 40);
    this->textbox->setVisible(false);

    this->label = CCLabelBMFontAnimated::createWithTTF("", "fonts/Gasalt-Regular.ttf", 30,
                  Size(contentSize.width, 40), TextHAlignment::LEFT, TextVAlignment::TOP);
    this->label->setTextColor(Color4B::BLACK);
    this->label->setPosition(contentSize.width * 0.55, 30);

    this->pulser = Sprite::create("hud/arrowSilver_right.png");
    this->pulser->setRotation(90.0f);
    this->pulser->setPosition(contentSize.width * 0.035, 40);
    this->pulser->setVisible(false);

    this->addChild(this->n1GoldCoin, 1);
    this->addChild(this->n2GoldCoin, 1);
    this->addChild(this->goldCoin, 1);
    this->addChild(this->xGoldCoin, 1);
    this->addChild(this->textbox, 1);
    this->addChild(this->label, 2);
    this->addChild(this->pulser, 2);

    // Creating a keyboard event listener
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = CC_CALLBACK_2(GameHub::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void GameHub::toggle(bool visibility) {
    this->n1GoldCoin->setVisible(visibility);
    this->n2GoldCoin->setVisible(visibility);
    this->xGoldCoin->setVisible(visibility);
    this->goldCoin->setVisible(visibility);
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

void GameHub::setLives(int nLives) {
    Director* director = Director::getInstance();
    Size contentSize = director->getVisibleSize();

    for (Node* child : this->getChildren()) {
        if (child->getTag() == LIVE_TAG) {
            removeChild(child);
        }
    }

    for (int i = 0; i < nLives; i++) {
        Sprite* live = Sprite::createWithSpriteFrameName("hud_heartFull.png");
        live->setPosition(contentSize.width * 0.1 + i * live->getContentSize().width + 1, contentSize.height * 0.9);
        addChild(live, 1, LIVE_TAG);
    }
}

void GameHub::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    CC_UNUSED_PARAM(event);
    if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) {
        if (!this->isAnimatingText) {
            this->stopPulse();
            if (!this->textQueue.empty()) {
                std::string nextText = this->textQueue.front();
                this->label->setString(nextText);

                this->textQueue.pop();
                this->isAnimatingText = true;
                this->label->animateInTypewriter(1, CallFunc::create([this] {
                    this->isAnimatingText = false;
                }));
                this->runPulse();
            } else if (!this->label->getString().empty()) {
                if (this->callback != nullptr) {
                    this->callback();
                }
                this->clearText();
            }
        } else {
            this->label->stopActionsOnAllSprites();
            this->label->setString(this->label->getString());
            this->label->setAllCharsScale(1);
            this->isAnimatingText = false;
            this->runPulse();
        }
    }
}

void GameHub::runPulse() {
    this->pulser->setVisible(true);
    this->pulser->runAction(
        RepeatForever::create(
            Sequence::create(FadeTo::create(0.8, 255), FadeTo::create(0.8, 50), nullptr)));
}

void GameHub::stopPulse() {
    this->pulser->stopAllActions();
    this->pulser->setVisible(false);
}

void GameHub::clearText() {
    std::queue<std::string>().swap(this->textQueue);
    this->label->setString("");
    this->textbox->setVisible(false);
}

void GameHub::setText(std::queue<std::string> textQueue) {
    this->setText(textQueue, nullptr);
    this->textbox->setVisible(true);
}

void GameHub::setText(std::queue<std::string> textQueue, std::function<void()> onFinish) {
    this->callback = onFinish;
    this->clearText();
    this->textbox->setVisible(true);
    this->textQueue = textQueue;
    this->onKeyReleased(EventKeyboard::KeyCode::KEY_SPACE, nullptr);
}
