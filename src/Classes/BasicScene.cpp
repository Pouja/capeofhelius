#include "BasicScene.h"
USING_NS_CC;


Scene* BasicScene::createScene()
{
    auto scene = Scene::create();
    auto layer = BasicScene::create();
    scene->addChild(layer);

    return scene;
}

bool BasicScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    // Create the tilemap
    this->map = new GameMap("tilemap.tmx", 2.0);
    addChild(this->map);

    this->drawNode = DrawNode::create();
    addChild(this->drawNode);

    // Set the view point of the tilemap
    this->setViewPointCenter(this->map->objectPoint("objects", "spawnpoint"));

    // Create the player
    this->player = new Player();
    this->player->init(this->map->objectPoint("objects", "spawnpoint"));
    addChild(this->player);

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

    Size mapSize = this->map->getMapSize();
    mapSize.width = mapSize.width * this->map->getTileSize().width * this->map->getScale();
    mapSize.height = mapSize.height * this->map->getTileSize().height * this->map->getScale();

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
