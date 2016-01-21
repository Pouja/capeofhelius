#include "BasicScene.h"
#include "utils/TitleScreen.h"
#include "base/ChapterManager.h"

#define SHOW_MOUSE true

USING_NS_CC;

bool BasicScene::init()
{
    this->paused = false;
    this->map = GameMap::create(this->mapName, 1);
    this->hub = GameHub::create();

    if (SHOW_MOUSE) {
        this->mouseLabel = Label::createWithTTF("", "fonts/Gasalt-Regular.ttf", 30);
        this->mouseLabel->setTextColor(Color4B::BLACK);
    }

    Size mapSize(this->map->getMapSize().width * this->map->getTileSize().width,
                 this->map->getMapSize().height * this->map->getTileSize().height);
    this->bg = Background::create(this->bgLocation, mapSize);

    //TODO: this should stay in gamemap
    std::vector<Value> dialogObjects = map->getObjectGroup("dialogs")->getObjects();
    std::for_each(dialogObjects.begin(), dialogObjects.end(), [this](Value v) {
        ValueMap object = v.asValueMap();

        std::string name = object.at("name").asString();
        float width = object.at("width").asFloat();
        float height = object.at("height").asFloat();
        float x = object.at("x").asFloat();
        float y = object.at("y").asFloat();

        this->dialogRects.push_back(std::pair<std::string, Rect>(name, Rect(x, y, width, height)));
    });

    if (SHOW_MOUSE) addChild(this->mouseLabel,1);
    addChild(this->bg);
    addChild(this->map);
    addChild(this->hub);

    if (initPlayers()) {
        std::for_each(players.begin(), players.end(), [this](std::pair<std::string, Player*> pair) {
            this->addChild(pair.second);
        });
    }
    // Creating a keyboard event listener
    EventListenerKeyboard* listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(BasicScene::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(BasicScene::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    if (SHOW_MOUSE) {
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->onTouchBegan = [this](Touch* touch, Event* event) {
            Vec2 pos = event->getCurrentTarget()->convertToNodeSpace(touch->getLocation());
            char strPos[100];
            sprintf(strPos, "mouse at (%d,%d)", (int) pos.x, (int) pos.y);
            this->mouseLabel->setString(strPos);
            return true;
        };

        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    }

    this->scheduleUpdate();
    return true;
}

void BasicScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (!this->paused && this->activeDialog == nullptr) {
        this->mainPlayer->onKeyPressed(keyCode, event);
    }
}

void BasicScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
        ChapterManager::getInstance()->save();
        Director::getInstance()->end();
    } else if (!this->paused && this->activeDialog == nullptr) {
        this->mainPlayer->onKeyReleased(keyCode, event);
    }
}

void BasicScene::resolveVertCollision(float tileHeight, float playerHeight, Vec2 tilePos,
                                      Vec2* velocity, Vec2* desiredPosition) {
    if (desiredPosition->y > tilePos.y) {
        desiredPosition->y = tilePos.y  + playerHeight / 2  + tileHeight / 2;
        velocity->y = fmaxf(velocity->y, 0);
    } else {
        desiredPosition->y = tilePos.y - playerHeight / 2  - tileHeight / 2;
        velocity->y = fminf(velocity->y, 0);
    }
}

void BasicScene::resolveHorCollision(float tileWidth, float playerWidth, Vec2 tilePos,
                                     Vec2* velocity, Vec2* desiredPosition) {
    if (desiredPosition->x > tilePos.x) {
        desiredPosition->x = tilePos.x + playerWidth / 2 + tileWidth / 2;
    } else {
        desiredPosition->x = tilePos.x - playerWidth / 2 - tileWidth / 2;
    }
    velocity->x = 0;
    velocity->y = fminf(velocity->y, 0);
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

    std::vector<GameMap::TileType> collisions = this->map->groundCollision(boundingPoints);
    if (collisions[8] == GameMap::TileType::DEATH) {
        onDeath();
        return;
    } else if (collisions[8] == GameMap::TileType::COLLECTABLE) {
        onCollectable(boundingPoints[8]);
    } else if (collisions[8] == GameMap::TileType::SPAWNPOINT) {
        onSpawnpoint(boundingPoints[8]);
    }

    Vec2 velocity = player->velocity;

    // We always asume that the player is not on the ground
    player->isOnGround = false;

    // Keep track of the number of collisions
    int collisionCount = 0;

    // Bottom collision, so the player is on the ground
    if (collisions[0] == GameMap::TileType::SLOPE_LEFT
            || collisions[0] == GameMap::TileType::SLOPE_LEFT
            || collisions[0] == GameMap::TileType::SLOPE_RIGHT
            || collisions[0] == GameMap::TileType::STUMP
            || collisions[0] == GameMap::TileType::WALL) {
        player->isOnGround = true;
    }

    // Slope collision
    if (collisions[0] == GameMap::TileType::SLOPE_LEFT || collisions[0] == GameMap::TileType::SLOPE_RIGHT) {
        bool isLeft = collisions[0] == GameMap::TileType::SLOPE_LEFT;
        Vec2 mapCoord = this->map->worldToMap(boundingPoints[0]);
        Vec2 pos = this->map->mapToWorld(mapCoord);

        resolveSlopeCollision(pos, playerHeight, &desiredPosition, isLeft);
        collisionCount++;
    } else if (collisions[0] == GameMap::TileType::STUMP) {
        Vec2 mapCoord = this->map->worldToMap(boundingPoints[0]);
        Vec2 pos = this->map->mapToWorld(mapCoord);

        //TODO: Magic numbers which makes the slopes a bit better, I honestly do not know why this works
        pos.add(Vec2(3, 3));

        this->resolveVertCollision(tileHeight, playerHeight, pos, &velocity, &desiredPosition);
        collisionCount++;
    } else {
        // Bottom (no slope) or top collision
        if (collisions[0] == GameMap::TileType::WALL || collisions[1] == GameMap::TileType::WALL) {
            int index = (collisions[0] == GameMap::TileType::WALL) ? 0 : 1;
            Vec2 mapCoord = this->map->worldToMap(boundingPoints[index]);
            Vec2 pos = this->map->mapToWorld(mapCoord);
            this->resolveVertCollision(tileHeight, playerHeight, pos, &velocity, &desiredPosition);
            collisionCount++;
        }
        // Left or Right collision
        if (collisions[2] == GameMap::TileType::WALL || collisions[3] == GameMap::TileType::WALL) {
            int index = (collisions[2] == GameMap::TileType::WALL) ? 2 : 3;
            Vec2 pos = this->map->mapToWorld(this->map->worldToMap(boundingPoints[index]));
            this->resolveHorCollision(tileWidth, playerWidth,  pos, &velocity, &desiredPosition);
            collisionCount++;
        }
    }
    // Left/right top/bottom collision
    for (int index = 4; index < 8 && collisionCount == 0; index++) {
        if (collisions[index] == GameMap::TileType::WALL) {
            Vec2 pos = this->map->mapToWorld(this->map->worldToMap(boundingPoints[index]));
            if (fabsf(pos.x - desiredPosition.x) > fabsf(pos.y - desiredPosition.y)) {
                this->resolveHorCollision(tileWidth, playerWidth,  pos, &velocity, &desiredPosition);
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
    std::vector<Vec2> boundingPoints = player->getBoundingPoints(player->getPosition());

    float playerHeight = boundingPoints[1].y - boundingPoints[0].y;
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

void BasicScene::onCollectable(Vec2 position) {
    this->map->removeCollectable(position);
    this->mainPlayer->addCoin();
    this->hub->setCoins(this->mainPlayer->getScore());
}

void BasicScene::onSpawnpoint(Vec2 position) {
    Vec2 newSpawn = map->mapToWorld(map->worldToMap(position));
    if (!this->respawnPoint.equals(newSpawn)) {
        std::string text[] = {"A spawnpoint, that will come in handy.",
                              "New spawnpoint, sweet!",
                              "I can continue my journey from here, if something happens."
                             };
        hub->setText(std::queue<std::string>({text[RandomHelper::random_int(0, 2)]}));
        this->respawnPoint = newSpawn;
    }
}

void BasicScene::onDeath() {
    this->paused = true;

    TitleScreen* titleScreen = TitleScreen::create("Oh now you died :(", "Be carefull!", true); \
    if (this->mainPlayer->getLives() - 1 == 0) {
        titleScreen = TitleScreen::create("Game Over", "", false);
    }

    titleScreen->setPosition(this->getPosition() * -1);

    CallFunc* cbDie = CallFunc::create([this] {
        if (this->mainPlayer->getLives() == 0) {
            onGameOver();
        } else {
            this->mainPlayer->respawn(this->respawnPoint);
            this->removeChildByTag(1);
            this->hub->setLives(this->mainPlayer->getLives());
            this->paused = false;
        }
    });

    this->mainPlayer->die(cbDie);

    addChild(titleScreen, 3, 1);
}

void BasicScene::checkEnemyCollision() {
    std::vector<Vec2> boundingPoints = mainPlayer->getBoundingPoints(mainPlayer->getPosition());
    float playerHeight = boundingPoints[1].y - boundingPoints[0].y;
    float playerWidth = boundingPoints[3].x - boundingPoints[2].x;
    Rect playerRect(boundingPoints[5].x, boundingPoints[5].y, playerWidth, playerHeight);

    std::vector<Enemy*> enemies = map->getEnemies();

    auto result = std::find_if(std::begin(enemies), std::end(enemies), [playerRect](Enemy * enemy) {
        return enemy->getBoundingBox().intersectsRect(playerRect);
    });

    if (result != std::end(enemies)) {
        onDeath();
    }
}

void BasicScene::checkDialog() {
    if (activeDialog != nullptr) {
        return;
    }
    auto result = std::find_if(std::begin(this->dialogRects), std::end(this->dialogRects), [this](std::pair<std::string, Rect> dialogRect) {
        return std::get<1>(dialogRect).containsPoint(this->mainPlayer->getPosition());
    });
    if (result != std::end(this->dialogRects)) {
        std::string id = std::get<0>(*result);
        if (id.back() == 'c') {
            if (this->dialogCondition(id)) {
                id += "-1";
            }
        } else {
            this->dialogRects.erase(result);
        }

        activeDialog = dialogs[id];
        this->mainPlayer->stop();

        activeDialog->run([this] {
            this->activeDialog = nullptr;
        });
    }
}

void BasicScene::updateVPC(cocos2d::Vec2 vpc) {
    this->setPosition(vpc);
    this->bg->move(vpc * -1);
    this->hub->setPosition(vpc * -1);
    if (SHOW_MOUSE) this->mouseLabel->setPosition((vpc + Vec2(-120, -720)) * -1);
}

void BasicScene::update(float delta) {
    if (this->mainPlayer->finished) {
        this->onFinish();
        this->unscheduleAllCallbacks();
        return;
    }
    if (!this->paused) {
        std::for_each(players.begin(), players.end(), [this](std::pair<std::string, Player*> pair) {
            pair.second->updatePhysics();
            this->resolveCollision(pair.second);
        });
    }

    Vec2 vpc = this->getViewPointCenter(this->mainPlayer->getPosition());
    this->map->update(delta);
    this->resolvePlatforms(this->mainPlayer, delta);

    if (!this->paused) {
        this->checkDialog();
        std::for_each(players.begin(), players.end(), [this](std::pair<std::string, Player*> pair) {
            pair.second->updateAnimation();
        });
        this->checkEnemyCollision();
        this->hub->setLives(mainPlayer->getLives());
    }

    this->updateVPC(vpc);
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
