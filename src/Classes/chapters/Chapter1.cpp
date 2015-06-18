#include "Chapter1.h"

USING_NS_CC;

Scene* Chapter1::createScene()
{
    auto scene = Scene::create();
    auto layer = Chapter1::create();
    scene->addChild(layer);
    return scene;
}

Chapter1* Chapter1::create() {
    Chapter1* chapter = new Chapter1();
    if (chapter && chapter->initVariables() && chapter->init()) {
        chapter->autorelease();
        return chapter;
    }
    CC_SAFE_DELETE(chapter);
    return nullptr;
}

bool Chapter1::initVariables() {
    this->mapName = "chapter1.tmx";
    this->mainName = "zoe-nocape";
    this->bgLocation = "backgrounds/chapter1-bg.png";
    return true;
}

void Chapter1::onGameOver() {
    Director::getInstance()->replaceScene(Chapter1::createScene());
}
