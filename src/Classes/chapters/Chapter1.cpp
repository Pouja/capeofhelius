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
    this->bgLocation = "backgrounds/chapter1-bg.png";

    return true;
}

bool Chapter1::initPlayers() {
    for(Value value : this->map->getObjectGroup("players")->getObjects()){
        ValueMap object = value.asValueMap();

        std::string name = object.at("name").asString();
        float x = object.at("x").asFloat();
        float y = object.at("y").asFloat();

        Player* p = Player::create(Vec2(x,y), name);
        this->players.push_back(p);

        if(name.compare("zoe-nocape") == 0) {
            this->mainPlayer = p;
            this->respawnPoint = Vec2(x,y);
        }
    }
    return true;
}

void Chapter1::onGameOver() {
    Director::getInstance()->replaceScene(Chapter1::createScene());
}
