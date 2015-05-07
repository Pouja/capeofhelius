#include "BasicScene.h"

USING_NS_CC;

Scene* BasicScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = BasicScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool BasicScene::init()
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
    ValueMap spawnPoint = objectGroup->getObject("spawnpoint");
    int xSpawnPoint = spawnPoint.at("x").asInt() * tilemap->getScale();
    int ySpawnPoint = spawnPoint.at("y").asInt() * tilemap->getScale();

    addChild(this->tilemap, -1);
    this->setViewPointCenter(Vec2(xSpawnPoint, ySpawnPoint));

    Sprite* mySprite = cocos2d::Sprite::create("CloseNormal.png");
    mySprite->setPosition(Vec2(xSpawnPoint, ySpawnPoint));
    // Move a sprite to a specific location over 2 seconds.
    addChild(mySprite);

    // creating a keyboard event listener
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(BasicScene::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(BasicScene::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // Implementation of the keyboard event callback function prototype

    // MoveBy - lets move the sprite by 500 on the x axis over 2 seconds
    // MoveBy is relative - since x = 200 + 200 move = x is now 400 after the move
    auto moveBy = MoveBy::create(2, Vec2(500, mySprite->getPositionY()));

    // MoveTo - lets move the new sprite to 300 x 256 over 2 seconds
    // MoveTo is absolute - The sprite gets moved to 300 x 256 regardless of
    // where it is located now.
    auto moveTo = MoveTo::create(2, Vec2(300, mySprite->getPositionY()));
    auto delay = DelayTime::create(0.25f);
    auto seq = Sequence::create(moveBy, delay, moveTo, nullptr);

    this->runAction(seq);

    return true;
}

void BasicScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event)
{
    log("Key with keycode %d pressed", keyCode);
}

void BasicScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event)
{
    log("Key with keycode %d released", keyCode);
}

void BasicScene::setViewPointCenter(Vec2 position) {
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
