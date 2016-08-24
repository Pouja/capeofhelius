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
    this->physicEngine = PhysicEngine::create();
    this->physicEngine->retain();
    this->physicEngine->setMap(map);

    initPlayers();
    loadPlatforms();

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

    if (SHOW_MOUSE) addChild(this->mouseLabel, 1);
    addChild(this->bg);
    addChild(this->map);
    addChild(this->hub);

    std::for_each(players.begin(), players.end(), [this](std::pair<std::string, Player*> pair) {
        this->addChild(pair.second);
    });
    std::for_each(platforms.begin(), platforms.end(), [this](Platform* platform) {
        this->addChild(platform);
    });

    // Creating a keyboard event listener
    EventListenerKeyboard* listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(BasicScene::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(BasicScene::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    if (SHOW_MOUSE) {
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->onTouchBegan = [this](Touch * touch, Event * event) {
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
void BasicScene::loadPlatforms() {
    TMXObjectGroup* objectGroup = map->getObjectGroup("platforms");
    float tileWidth = map->getTileSize().width * map->getScale();
    float tileHeight = map->getTileSize().height * map->getScale();
    for (Value object : objectGroup->getObjects()) {
        ValueMap dict = object.asValueMap();
        Platform* platform = Platform::parse(dict, map->getScale(), tileWidth, tileHeight);
        platforms.push_back(platform);
    }
}
void BasicScene::initPlayers() {
    ChapterManager* chapterManager = ChapterManager::getInstance();
    for (Value value : this->map->getObjectGroup("players")->getObjects()) {
        ValueMap object = value.asValueMap();

        std::string name = object.at("name").asString();
        float x = object.at("x").asFloat();
        float y = object.at("y").asFloat();

        Player* p;

        if (name.compare("zoe-nocape") == 0) {
            ChapterManager::Progress progress = chapterManager->getProgress();

            if (!progress.spawnPoint.equals(Vec2::ZERO)) {
                Vec2 worldCoord = map->mapToWorld(progress.spawnPoint);
                p = Player::create(worldCoord, name);
                this->respawnPoint = worldCoord;
            } else {
                p = Player::create(Vec2(x, y), name);
                this->respawnPoint = Vec2(x, y);
            }
            this->mainPlayer = p;
            name = "main";
        } else {
            p = Player::create(Vec2(x, y), name);
        }
        this->physicEngine->addMovableComponent(p->getPhysicBody());
        this->players[name] = p;
    }
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
    this->physicEngine->update(delta);
    if (this->mainPlayer->finished) {
        this->onFinish();
        this->unscheduleAllCallbacks();
        return;
    }

    Vec2 vpc = this->getViewPointCenter(this->mainPlayer->getPosition());
    this->map->update(delta);

    if (!this->paused) {
        // this->checkDialog();
        std::for_each(players.begin(), players.end(), [this](std::pair<std::string, Player*> pair) {
            pair.second->updateAnimation();
        });
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
