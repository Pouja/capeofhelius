#include "GameHub.h"

USING_NS_CC;

GameHub::GameHub() {

}

bool GameHub::init() {
    this->label = Label::createWithTTF("", "fonts/Marker Felt.ttf", 18);

    Director* director = Director::getInstance();
    Size contentSize = director->getVisibleSize();

    this->label->setWidth(contentSize.width * 0.5);

    this->addChild(label, 1);
    return true;
}

void GameHub::update(float delta) {
    Director* director = Director::getInstance();
    Size contentSize = director->getVisibleSize();
    this->label->setPosition(contentSize.width * 0.3, 40);
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