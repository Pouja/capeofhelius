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

    this->paused = false;

    // Create the tilemap
    this->map = new GameMap("tilemap.tmx", 2.0);
    addChild(this->map);

    this->hub = new GameHub();
    this->hub->init();
    addChild(this->hub);

    this->drawNode = DrawNode::create();
    addChild(this->drawNode);

    // Create the player
    this->mainPlayer = new Player(this->map->objectPoint("objects", "spawnpoint"));
    addChild(this->mainPlayer);

    // Creating a keyboard event listener
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(BasicScene::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(BasicScene::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    this->scheduleUpdate();

    this->onStart();
    return true;
}

void BasicScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    this->mainPlayer->onKeyPressed(keyCode, event);
    this->hub->onKeyPressed(keyCode, event);
}

void BasicScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    this->mainPlayer->onKeyReleased(keyCode, event);
    this->hub->onKeyReleased(keyCode, event);
}

void BasicScene::resolveVertCollision(float tileHeight, float playerHeight, Vec2 tilePos, Vec2* velocity, Vec2* desiredPosition) {
    if (desiredPosition->y > tilePos.y) {
        desiredPosition->y = tilePos.y  + playerHeight / 2  + tileHeight / 2;
    } else {
        desiredPosition->y = tilePos.y - playerHeight / 2  - tileHeight / 2;
    }
    velocity->y = 0;
}

void BasicScene::resolveHorCollision(float tileWidth, float playerWidth, Vec2 tilePos, Vec2* desiredPosition) {
    if (desiredPosition->x > tilePos.x) {
        desiredPosition->x = tilePos.x + playerWidth / 2 + tileWidth / 2;
    } else {
        desiredPosition->x = tilePos.x - playerWidth / 2 - tileWidth / 2;
    }
}

void BasicScene::resolveCollision(Player* player) {
    Vec2 desiredPosition = player->getDesiredPosition();

    float playerHeight = player->getContentSize().height;
    float playerWidth = player->getContentSize().width;

    float tileHeight = this->map->getTileSize().height * this->map->getScale();
    float tileWidth = this->map->getTileSize().width * this->map->getScale();

    std::vector<Sprite*> collisions = this->map->groundCollision(player->getBoundingPoints(desiredPosition));

    Vec2 velocity = player->velocity;
    player->isOnGround = false;

    int collisionCount = 0;
    if (collisions[0]) {
        player->isOnGround = true;
    }
    if (collisions[0] || collisions[1]) {
        int index = (collisions[0]) ? 0 : 1;
        Vec2 pos = this->map->tileToWorld(collisions[index]);
        this->resolveVertCollision(tileHeight, playerHeight, pos, &velocity, &desiredPosition);
        collisionCount++;
    }
    if (collisions[2] || collisions[3]) {
        int index = (collisions[2]) ? 2 : 3;
        Vec2 pos = this->map->tileToWorld(collisions[index]);
        this->resolveHorCollision(tileWidth, playerWidth,  pos, &desiredPosition);
        collisionCount++;
    }
    for (int index = 4; index < 8 && collisionCount == 0; index++) {
        if (collisions[index]) {
            Vec2 pos = this->map->tileToWorld(collisions[index]);
            if (fabsf(pos.x - desiredPosition.x) > fabsf(pos.y - desiredPosition.y)) {
                this->resolveHorCollision(tileWidth, playerWidth,  pos, &desiredPosition);
            } else {
                this->resolveVertCollision(tileWidth, playerHeight, pos, &velocity, &desiredPosition);
                if (index == 5 || index == 6) {
                    player->isOnGround = true;
                }
            }
            collisionCount++;
        }
    }

    player->velocity = velocity;
    player->setPosition(desiredPosition);
}

void BasicScene::update(float delta) {
    if (!this->paused && this->hub->isDone()) {
        this->mainPlayer->update(delta);
        this->resolveCollision(this->mainPlayer);
    }
    Vec2 vpc = this->getViewPointCenter(this->mainPlayer->getPosition());

    this->setPosition(vpc);

    this->hub->setPosition(vpc * -1);
    this->hub->update(delta);
}

Vec2 BasicScene::getViewPointCenter(Vec2 position) {
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

    return Vec2(-1 * xView, yView);
}

void BasicScene::onStart() {
    std::queue<std::string> textQueue;
    textQueue.push("Lets start the game!");
    this->hub->setText(textQueue);
}