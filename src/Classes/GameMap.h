#ifndef _GAME_MAP_H_
#define _GAME_MAP_H_

#include "cocos2d.h"

class GameMap : public cocos2d::TMXTiledMap {
public:
    GameMap(std::string mapName, float scale);
    cocos2d::Vec2 worldToMap(cocos2d::Vec2 worldCoord);
    cocos2d::Vec2 mapToWorld(cocos2d::Vec2 mapCoord);
    cocos2d::Vec2 tileToWorld(cocos2d::Sprite* tile);
    cocos2d::Vec2 objectPoint(std::string group, std::string objectName);
    std::vector<cocos2d::Sprite*> groundCollision(std::vector<cocos2d::Vec2> points);
    ~GameMap();
private:
    std::string name;
};

#endif
