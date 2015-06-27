#ifndef _GAME_MAP_H_
#define _GAME_MAP_H_

#include "cocos2d.h"
#include "dynamic-scene/Platform.h"
#include "enemies/Enemy.h"

class GameMap : public cocos2d::TMXTiledMap {
public:
    enum TileType {
        DEATH, //0
        WALL, //1
        COLLECTABLE, // 2
        SLOPE_LEFT, // 3
        SLOPE_RIGHT, // 4
        STUMP, // 4
        SPAWNPOINT, //5
        NONE //6
    };
    /**
     * Creates a new game map with the given map name and scale.
     * @param  mapName The name of the map, should match to [name].tmx
     * @param  scale   The scale to be set.
     * @return         The game map.
     */
    static GameMap* create(const std::string& mapName, float scale);

    /** Converts a map coordinate to the world coordinate */
    cocos2d::Vec2 worldToMap(cocos2d::Vec2 worldCoord);

    /** Converts a world coordinate to the map coordinate. */
    cocos2d::Vec2 mapToWorld(cocos2d::Vec2 mapCoord);

    /** Retrieves the vector for the given object name in a specific group. */
    cocos2d::Vec2 objectPoint(std::string group, std::string objectName);

    /** Retrieves the sprites for each point in the layer "ground" */
    std::vector<TileType> groundCollision(std::vector<cocos2d::Vec2> points);

    /** Called when the map is allowed to update **/
    void update(float delta);

    /**
     * Returns all the enemies which are spawned in the map.
     */
    std::vector<Enemy*> getEnemies();

    /**
     * Removes a collectable at the given world coordinates.
     * @param worldCoord The coordinates of the collectable.
     */
    void removeCollectable(cocos2d::Vec2 worldCoord);

    /**
     * Returns all the platforms
     */
    std::vector<Platform*> getPlatforms();
private:
    /**
     * @brief Initializes the collision tiles.
     * @details Should be called after initWithXml.
     */
    void initTiles();

    /**
     * @brief Loads all the dynamic non collidable sprites (such as torches and clouds).
     * @details Should be called after initWithXml.
     */
    void loadDynamicScene();

    /**
     * Initializes all the platforms and starts their movement.
     */
    void loadPlatforms();

    /**
     * Initializes all the enemies.
     */
    void loadEnemies();

    std::vector<Enemy*> enemies;
    std::vector<Platform*> platforms;
    GameMap(const std::string& mapName, float scale);
    std::vector<TileType> tiles;
    std::string name;
};

#endif
