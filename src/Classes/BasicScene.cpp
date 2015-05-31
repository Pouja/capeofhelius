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
    this->previousEvent = GameMap::CollisionType::NONE;

    this->paused = false;

    // Create the tilemap
    this->map = GameMap::create("tilemap.tmx", 2.0);
    addChild(this->map);

    this->hub = new GameHub();
    this->hub->init();
    addChild(this->hub);

    this->drawNode = DrawNode::create();
    addChild(this->drawNode);

    // Create the player
    this->mainPlayer = Player::create(this->map->objectPoint("objects", "spawnpoint"));
    addChild(this->mainPlayer);

    // Creating a keyboard event listener
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(BasicScene::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(BasicScene::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    this->scheduleUpdate();
    onStart();
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

    float playerHeight = player->getContentSize().height * player->getScale();
    float playerWidth = player->getContentSize().width * player->getScale();

    float tileHeight = this->map->getTileSize().height * this->map->getScale();
    float tileWidth = this->map->getTileSize().width * this->map->getScale();

    float mapWidth = this->map->getMapSize().width * tileWidth;

    // Make sure that the player can not move outside the map
    if (desiredPosition.x - playerWidth / 2 < 0) {
        desiredPosition.x = playerWidth / 2;
    } else if (desiredPosition.x + playerWidth / 2 >= mapWidth) {
        desiredPosition.x = mapWidth - playerWidth / 2;
    }

    // Retrieve all the sprites on which the player collides
    std::vector<Sprite*> collisions = this->map->groundCollision(player->getBoundingPoints(desiredPosition));

    Vec2 velocity = player->velocity;

    // We always asume that the player is not on the ground
    player->isOnGround = false;

    // Keep track of the number of collisions
    int collisionCount = 0;

    // Bottom collision, so the player is on the ground
    if (collisions[0]) {
        player->isOnGround = true;
    }
    if (collisions[0] || collisions[1]) {
        int index = (collisions[0]) ? 0 : 1;
        Vec2 pos = this->map->tileToWorld(collisions[index]);
        this->resolveVertCollision(tileHeight, playerHeight, pos, &velocity, &desiredPosition);
        collisionCount++;
    }
    // Left or Right collision
    if (collisions[2] || collisions[3]) {
        int index = (collisions[2]) ? 2 : 3;
        Vec2 pos = this->map->tileToWorld(collisions[index]);
        this->resolveHorCollision(tileWidth, playerWidth,  pos, &desiredPosition);
        collisionCount++;
    }
    // Left/right top/bottom collision
    for (int index = 4; index < 8 && collisionCount == 0; index++) {
        if (collisions[index]) {
            Vec2 pos = this->map->tileToWorld(collisions[index]);
            if (fabsf(pos.x - desiredPosition.x) > fabsf(pos.y - desiredPosition.y)) {
                this->resolveHorCollision(tileWidth, playerWidth,  pos, &desiredPosition);
            } else {
                this->resolveVertCollision(tileWidth, playerHeight, pos, &velocity, &desiredPosition);
                // left/right bottom collision, so the player is on the ground
                if (index == 5 || index == 6) {
                    player->isOnGround = true;
                }
            }
            // We only resolve one corner collision
            collisionCount++;
        }
    }

    player->velocity = velocity;
    player->setPosition(desiredPosition);
}

void BasicScene::onEventEnter(GameMap::CollisionType event, Vec2 tilePosition) {
    switch (event) {
    case GameMap::CollisionType::COLLECTABLE:
        this->mainPlayer->addCoin();
        this->hub->setCoins(this->mainPlayer->getScore());
        break;
    case GameMap::CollisionType::TEXTBOX:
        onTextBox(tilePosition);
        break;
    case GameMap::CollisionType::START:
        onStart();
        break;
    case GameMap::CollisionType::DEATH:
        onDeath();
        break;
    default:
        this->hub->clearText();
        break;
    }
}

void BasicScene::resolveEvent(Player* sprite) {
    Vec2 tilePosition = this->map->worldToMap(sprite->getDesiredPosition());
    GameMap::CollisionType type = this->map->eventCollision(tilePosition);
    if (previousEvent != type) {
        onEventEnter(type, tilePosition);
    }
    previousEvent = type;
}

void BasicScene::update(float delta) {
    if (!this->paused) {
        this->mainPlayer->updatePhysics();
        this->resolveCollision(this->mainPlayer);
        this->resolveEvent(this->mainPlayer);
    }
    Vec2 vpc = this->getViewPointCenter(this->mainPlayer->getPosition());

    this->setPosition(vpc);

    this->hub->setPosition(vpc * -1);
    this->hub->update(delta);
    this->mainPlayer->updateAnimation();
}

Vec2 BasicScene::getViewPointCenter(Vec2 position) {
    Size winSize = Director::getInstance()->getWinSize();

    Size mapSize = this->map->getMapSize();
    mapSize.width = mapSize.width * this->map->getTileSize().width * this->map->getScale();
    mapSize.height = mapSize.height * this->map->getTileSize().height * this->map->getScale();

    float xView = position.x - winSize.width / 2;
    if (position.x <= winSize.width / 2 || winSize.width >= mapSize.width) {
        xView = 0;
    } else if (mapSize.width - winSize.width / 2 <= position.x ) {
        xView = mapSize.width - winSize.width;
    }

    float yView = position.y - winSize.height / 2;;
    if (position.y <= winSize.height / 2 || winSize.height >= mapSize.height) {
        yView = 0;
    } else if (mapSize.height - winSize.height / 2 <= position.y) {
        yView = mapSize.height - winSize.height;
    }

    return Vec2(-1 * xView, yView);
}

void BasicScene::onTextBox(Vec2 tilePosition) {
    std::queue<std::string> textQueue;
    textQueue.push("Couple of message inbouding! (1)");
    textQueue.push("Couple of message inbouding! (2)");
    textQueue.push("Couple of message inbouding! (3)");
    this->hub->setText(textQueue);
}

void BasicScene::onDeath() {
    this->mainPlayer->setPosition(this->map->objectPoint("objects", "spawnpoint"));

    std::queue<std::string> textQueue;
    textQueue.push("Oh noo you died :(. But try again!");
    this->hub->setText(textQueue);
}

void BasicScene::onStart() {
    std::queue<std::string> textQueue;
    textQueue.push("Lets start the game!\nPress spacebar to continue...");
    this->hub->setText(textQueue);
}

BasicScene::~BasicScene(){
    // this->stopAllActions();
    // CC_SAFE_RELEASE(mainPlayer);
    // CC_SAFE_RELEASE(map);
    // CC_SAFE_RELEASE(hub);
}