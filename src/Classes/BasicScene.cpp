#include "BasicScene.h"

USING_NS_CC;

Vec2 getObjectPoint(TMXTiledMap* tilemap, std::string group, std::string objectName){
    TMXObjectGroup* objectGroup = tilemap->getObjectGroup(group);
    ValueMap spawnPoint = objectGroup->getObject(objectName);
    int xSpawnPoint = spawnPoint.at("x").asInt() * tilemap->getScale();
    int ySpawnPoint = spawnPoint.at("y").asInt() * tilemap->getScale();

    return Vec2(xSpawnPoint, ySpawnPoint);
}

Scene* BasicScene::createScene()
{
    auto scene = Scene::create();
    auto layer = BasicScene::create();
    scene->addChild(layer);

    return scene;
}

void BasicScene::createMap(){
    std::string file = "tilemap.tmx";
    auto str = String::createWithContentsOfFile(FileUtils::getInstance()->fullPathForFilename(file.c_str()).c_str());
    this->tilemap = TMXTiledMap::createWithXML(str->getCString(), "");
    this->tilemap->setScale(2.0);
}

bool BasicScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    // Create the tilemap
    this->createMap();
    addChild(this->tilemap);

    // Set the view point of the tilemap
    this->setViewPointCenter(getObjectPoint(this->tilemap, "objects", "spawnpoint"));

    // Create the player
    this->player = new Player();
    addChild(this->player->init(getObjectPoint(this->tilemap, "objects", "spawnpoint")));

    // Creating a keyboard event listener
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this->player);
    listener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this->player);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    this->scheduleUpdate();
    return true;
}

void BasicScene::update(float delta){
    this->setViewPointCenter(this->player->getPosition());
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
