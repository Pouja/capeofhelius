#include "BasicScene.h"

USING_NS_CC;

Vec2 getObjectPoint(TMXTiledMap* tilemap, std::string group, std::string objectName) {
    TMXObjectGroup* objectGroup = tilemap->getObjectGroup(group);
    ValueMap spawnPoint = objectGroup->getObject(objectName);
    int xSpawnPoint = spawnPoint.at("x").asInt() * tilemap->getScale();
    int ySpawnPoint = spawnPoint.at("y").asInt() * tilemap->getScale();

    return Vec2(xSpawnPoint, ySpawnPoint);
}

Vec2 getTileVec(Vec2 position, TMXTiledMap* tilemap){
    // Scale with the map size.
    position.scale(1/tilemap->getScale());

    float x = floorf(position.x / tilemap->getTileSize().width);
    float y = floorf(position.y / tilemap->getTileSize().height) - 1;

    // (0,0) for tilemap is topleft, so we need the inverse.
    y = tilemap->getMapSize().height - y;
    return Vec2(x,y);
}

Scene* BasicScene::createScene()
{
    auto scene = Scene::create();
    auto layer = BasicScene::create();
    scene->addChild(layer);

    return scene;
}

void BasicScene::createMap() {
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

    log("tilesize(%f,%f) mapsize(%f,%f)", tilemap->getTileSize().width, tilemap->getTileSize().height, tilemap->getMapSize().width, tilemap->getMapSize().height);

    this->scheduleUpdate();
    return true;
}

void BasicScene::update(float delta) {
    this->player->update(delta);
    this->setViewPointCenter(this->player->getPosition());

    TMXLayer* layer = tilemap->getLayer("foreground");
    Sprite* tile = layer->getTileAt(getTileVec(this->player->getPosition(), this->tilemap));
    if(tile){
        log("collision");
    } else {
        log("no collision");
    }
}

void BasicScene::setViewPointCenter(Vec2 position) {
    Size winSize = Director::getInstance()->getWinSize();

    Size mapSize = tilemap->getMapSize();
    mapSize.width = mapSize.width * tilemap->getTileSize().width * tilemap->getScale();
    mapSize.height = mapSize.height * tilemap->getTileSize().height * tilemap->getScale();

    float xView = position.x - winSize.width / 2;
    if (position.x <= winSize.width / 2) {
        xView = 0;
    } else if (mapSize.width - winSize.width / 2 <= position.x ) {
        xView = mapSize.width - winSize.width;
    }

    float yView = position.y - winSize.height / 2;;
    if (position.y <= winSize.height / 2) {
        yView = 0;
    } else if (mapSize.height - winSize.height / 2 <= position.y) {
        yView = mapSize.height - winSize.height;
    }

    this->setPosition(Vec2(-1 * xView, yView));
}
