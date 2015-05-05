#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    std::string file = "tilemap.tmx";
    auto str = String::createWithContentsOfFile(FileUtils::getInstance()->fullPathForFilename(file.c_str()).c_str());
    this->tilemap = TMXTiledMap::createWithXML(str->getCString(), "");
    this->tilemap->setScale(2.0);

    this->background = this->tilemap->getLayer("bg");
    TMXObjectGroup* objectGroup = tilemap->getObjectGroup("objects");
    ValueMap spawnPoint = objectGroup->getObject("end");
    int xSpawnPoint = spawnPoint.at("x").asInt() * tilemap->getScale();
    int ySpawnPoint = spawnPoint.at("y").asInt() * tilemap->getScale();

    addChild(this->tilemap, -1);
    this->setViewPointCenter(Vec2(xSpawnPoint, ySpawnPoint));
    return true;
}

void HelloWorld::setViewPointCenter(Vec2 position) {
    Size winSize = Director::getInstance()->getWinSize();

    Size mapSize = tilemap->getMapSize();
    mapSize.width = mapSize.width * tilemap->getTileSize().width * tilemap->getScale();
    mapSize.height = mapSize.height * tilemap->getTileSize().height * tilemap->getScale();

    float xView = position.x - winSize.width / 2;
    if (position.x >= 0 && position.x <= winSize.width / 2) {
        xView = 0;
    } else if (mapSize.width - winSize.width / 2 <= position.x && position.x <= mapSize.width) {
        xView = mapSize.width - winSize.width;
    }

    float yView = position.x - winSize.width / 2;;
    if (position.y >= 0 && position.y <= winSize.height / 2) {
        yView = 0;
    } else if (mapSize.height - winSize.height / 2 <= position.y && position.y <= mapSize.height) {
        yView = mapSize.height - winSize.height;
    }

    this->setPosition(Vec2(-1 * xView, yView));
}
