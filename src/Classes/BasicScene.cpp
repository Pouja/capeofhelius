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
    this->player = new Player(this->map->objectPoint("objects", "spawnpoint"));
    addChild(this->player);

    // Creating a keyboard event listener
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this->player);
    listener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this->player);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    this->scheduleUpdate();
    return true;
}

void verticalCollision(float playerHeight, Vec2 playerPos, Vec2 tilePos, Vec2* velocity, Vec2* desiredPosition) {
    if (playerPos.y > tilePos.y) {
        desiredPosition->y = tilePos.y  + playerHeight + 1;
    } else {
        desiredPosition->y = tilePos.y - playerHeight - 1;
    }
    velocity->y = 0;
}

void horizontalCollision(float playerWidth, Vec2 playerPos, Vec2 tilePos, Vec2* desiredPosition) {
    if (playerPos.x > tilePos.x) {
        desiredPosition->x = tilePos.x + playerWidth + 1;
    } else {
        desiredPosition->x = tilePos.x - playerWidth - 1;
    }
}


void BasicScene::update(float delta) {
    this->player->update(delta);

    Vec2 desiredPosition = this->player->desiredPosition;

    float playerHeight = player->getContentSize().height;
    float playerWidth = player->getContentSize().width;

    std::vector<Sprite*> collisions = this->map->groundCollision(this->player->getBoundingPoints(desiredPosition));

    Vec2 velocity = this->player->velocity;
    this->player->isOnGround = false;

    int collisionCount = 0;
    if (collisions[0]) {
        this->player->isOnGround = true;
    }
    if (collisions[0] || collisions[1]) {
        int index = (collisions[0]) ? 0 : 1;
        Vec2 pos = this->map->tileToWorld(collisions[index]);
        verticalCollision(playerHeight, desiredPosition, pos, &velocity, &desiredPosition);
        collisionCount++;
    }
    if (collisions[2] || collisions[3]) {
        int index = (collisions[2]) ? 2 : 3;
        Vec2 pos = this->map->tileToWorld(collisions[index]);
        collisionCount++;
    }
    for (int index = 5; index < 8 && collisionCount == 0; index++) {
        if (collisions[index]) {
            Vec2 pos = this->map->tileToWorld(collisions[index]);
            if (fabsf(pos.x - desiredPosition.x) > fabsf(pos.y - desiredPosition.y)) {
                horizontalCollision(playerWidth, desiredPosition, pos, &desiredPosition);
            } else {
                verticalCollision(playerHeight, desiredPosition, pos, &velocity, &desiredPosition);
                if (index == 5 || index == 6) {
                    this->player->isOnGround = true;
                }
            }
            break;
        }
    }

    this->player->velocity = velocity;
    this->player->setPosition(desiredPosition);

    this->setViewPointCenter(this->player->getPosition());
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
