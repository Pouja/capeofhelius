#ifndef _GAME_H_
#define _GAME_H_

#include <string>
#include <map>

#include "GameHub.h"
#include "cocos2d.h"
#include "player/Player.h"
#include "GameCamera.h"
#include "GameMap.h"

#define getGOCamera() GameObjects::getInstance()->getCamera()

class GameObjects {
private:
    GameHub* hub;
    GameMap* map;
    GameCamera* camera;
    std::map<std::string, Player*> players;
public:
    static GameObjects* getInstance();
    void addPlayer(Player* p, std::string name);

    GameHub *getHub() const;
    void setHub(GameHub *hub);

    GameMap *getMap() const;

    void setMap(GameMap *map);

    GameCamera *getCamera() const;

    void setCamera(GameCamera *camera);

private:
    GameObjects(){};
    ~GameObjects(){};

    GameObjects(GameObjects const&) = delete;
    void operator=(GameObjects const&) = delete;
};

#endif
