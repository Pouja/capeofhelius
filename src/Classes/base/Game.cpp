#include "Game.h"

USING_NS_CC;

static GameObjects* s_GameObjects = nullptr;

GameObjects* GameObjects::getInstance() {
    if (!s_GameObjects) {
        s_GameObjects = new (std::nothrow) GameObjects();
        CCASSERT(s_GameObjects, "FATAL: not enough memory");
    }
    return s_GameObjects;
}

void GameObjects::addPlayer(Player *p, std::string name) {
    this->players.insert(std::pair<std::string, Player*>(name, p));
}

GameHub *GameObjects::getHub() const {
    return hub;
}

void GameObjects::setHub(GameHub *hub) {
    this->hub = hub;
}

GameMap *GameObjects::getMap() const {
    return map;
}

void GameObjects::setMap(GameMap *map) {
    this->map = map;
}

GameCamera *GameObjects::getCamera() const {
    return camera;
}

void GameObjects::setCamera(GameCamera *camera) {
    this->camera = camera;
}
