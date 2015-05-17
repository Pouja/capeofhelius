#ifndef _GAME_MAP_H_
#define _GAME_MAP_H_

#include "cocos2d.h"

class GameMap : public cocos2d::TMXTiledMap {
public:
    GameMap(std::string mapName, float scale);

    /** Converts a map coordinate to the world coordinate */
    cocos2d::Vec2 worldToMap(cocos2d::Vec2 worldCoord);

    /** Converts a world coordinate to the map coordinate. */
    cocos2d::Vec2 mapToWorld(cocos2d::Vec2 mapCoord);

    /** Calculates the world coordinate for a sprite tile. */
    cocos2d::Vec2 tileToWorld(cocos2d::Sprite* tile);

    /** Retrieves the vector for the given object name in a specific group. */
    cocos2d::Vec2 objectPoint(std::string group, std::string objectName);

    /** Retrieves the sprites for each point in the layer "ground" */
    std::vector<cocos2d::Sprite*> groundCollision(std::vector<cocos2d::Vec2> points);
    ~GameMap();
private:
    std::string name;
};

#endif
