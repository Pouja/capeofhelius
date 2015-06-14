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
    this->previousEvent = GameMap::CollisionType::NONE;
    this->paused = false;
    this->map = GameMap::create("chapter1.tmx", 1);
    this->hub = new GameHub();
    this->hub->init();
    this->mainPlayer = Player::create(this->map->objectPoint("objects", "spawnpoint"));

    addChild(this->map);
    addChild(this->hub);
    addChild(this->mainPlayer);

    // Creating a keyboard event listener
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(BasicScene::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(BasicScene::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    this->scheduleUpdate();
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

void BasicScene::resolveSlopeCollision(Vec2 tilePos, float playerHeight, Vec2* desiredPosition, bool isLeftSlope) {
    float a = (isLeftSlope) ? 1.0 : -1.0;
    float b = tilePos.y + (playerHeight / 2) - a * tilePos.x;
    desiredPosition->y = (a * desiredPosition->x) + b + 3;
}

void BasicScene::resolveCollision(Player* player) {
    Vec2 desiredPosition = player->getDesiredPosition();

    float tileHeight = this->map->getTileSize().height * this->map->getScale();
    float tileWidth = this->map->getTileSize().width * this->map->getScale();
    float mapWidth = this->map->getMapSize().width * tileWidth;

    // Retrieve all the sprites on which the player collides
    std::vector<Vec2> boundingPoints = player->getBoundingPoints(desiredPosition);
    float playerHeight = boundingPoints[1].y - boundingPoints[0].y;
    float playerWidth = boundingPoints[3].x - boundingPoints[2].x;

    // Make sure that the player can not move outside the map
    if (desiredPosition.x - playerWidth / 2 < 0) {
        desiredPosition.x = playerWidth / 2;
    } else if (desiredPosition.x + playerWidth / 2 >= mapWidth) {
        desiredPosition.x = mapWidth - playerWidth / 2;
    }

    std::vector<GameMap::CollisionType> collisions = this->map->groundCollision(boundingPoints);

    Vec2 velocity = player->velocity;

    // We always asume that the player is not on the ground
    player->isOnGround = false;

    // Keep track of the number of collisions
    int collisionCount = 0;

    // Bottom collision, so the player is on the ground
    if (collisions[0] != GameMap::CollisionType::NONE) {
        player->isOnGround = true;
    }

    // Slope collision
    if (collisions[0] == GameMap::CollisionType::SLOPE_LEFT || collisions[0] == GameMap::CollisionType::SLOPE_RIGHT) {
        bool isLeft = collisions[0] == GameMap::CollisionType::SLOPE_LEFT;

        Vec2 mapCoord = this->map->worldToMap(boundingPoints[0]);
        Vec2 pos = this->map->mapToWorld(mapCoord);

        resolveSlopeCollision(pos, playerHeight, &desiredPosition, isLeft);
        collisionCount++;
    } else if (collisions[0] == GameMap::CollisionType::STUMP) {
        Vec2 mapCoord = this->map->worldToMap(boundingPoints[0]);
        Vec2 pos = this->map->mapToWorld(mapCoord);

        // Magic numbers which makes the slopes a bit better, I honestly do not know why this works
        pos.add(Vec2(3, 3));

        this->resolveVertCollision(tileHeight, playerHeight, pos, &velocity, &desiredPosition);
        collisionCount++;
    } else {
        // Bottom (no slope) or top collision
        if (collisions[0] == GameMap::CollisionType::WALL || collisions[1] == GameMap::CollisionType::WALL) {
            int index = (collisions[0] == GameMap::CollisionType::WALL) ? 0 : 1;
            Vec2 mapCoord = this->map->worldToMap(boundingPoints[index]);
            Vec2 pos = this->map->mapToWorld(mapCoord);
            this->resolveVertCollision(tileHeight, playerHeight, pos, &velocity, &desiredPosition);
            collisionCount++;
        }
        // Left or Right collision
        if (collisions[2] == GameMap::CollisionType::WALL || collisions[3] == GameMap::CollisionType::WALL) {
            int index = (collisions[2] == GameMap::CollisionType::WALL) ? 2 : 3;
            Vec2 pos = this->map->mapToWorld(this->map->worldToMap(boundingPoints[index]));
            this->resolveHorCollision(tileWidth, playerWidth,  pos, &desiredPosition);
            collisionCount++;
        }
    }
    // Left/right top/bottom collision
    for (int index = 4; index < 8 && collisionCount == 0; index++) {
        if (collisions[index] == GameMap::CollisionType::WALL) {
            Vec2 pos = this->map->mapToWorld(this->map->worldToMap(boundingPoints[index]));
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

void BasicScene::resolvePlatforms(Player* player, float delta) {
    Platform* collidedPlatform = nullptr;

    std::vector<Vec2> boundingPoints = player->getBoundingPoints(player->getPosition());

    float playerHeight = boundingPoints[1].y - boundingPoints[0].y;
    float playerWidth = boundingPoints[3].x - boundingPoints[2].x;
    Vec2 playerPosition = player->getPosition();

    for (Platform* platform : this->map->getPlatforms()) {
        Size platformSize = platform->getContentSize();
        Vec2 platformPos = platform->getPosition();
        Rect platformRect;
        platformRect.origin = Vec2(platformPos.x - platformSize.width / 2, platformPos.y - platformSize.height / 2);
        platformRect.size = platformSize;

        if (platformRect.containsPoint(boundingPoints[0])) {
            playerPosition.y = platformPos.y + playerHeight / 2 + platformSize.height / 2;
            player->velocity.y = 0;
            player->isOnGround = true;
            Vec2 platformVel = platform->getVelocity();
            platformVel.scale(delta);
            player->setExternalForce(platformVel);
            break;
        } else if (platformRect.containsPoint(boundingPoints[1])) {
            playerPosition.y = platformPos.y - playerHeight / 2 - platformSize.height / 2;
            player->velocity.y = 0;
            break;
        }
    }

    player->setPosition(playerPosition);
}

void BasicScene::update(float delta) {
    if (!this->paused) {
        this->mainPlayer->updatePhysics();
        this->resolveCollision(this->mainPlayer);
    }
    Vec2 vpc = this->getViewPointCenter(this->mainPlayer->getPosition());
    this->setPosition(vpc);
    this->map->update(delta);
    this->resolvePlatforms(this->mainPlayer, delta);
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

    return Vec2(-1 * xView, -1 * yView);
}
