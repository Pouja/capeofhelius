#ifndef _GAME_MAP_H_
#define _GAME_MAP_H_

#include "cocos2d.h"
#include "dynamic-scene/Platform.h"
#include "enemies/Enemy.h"

class GameMap : public cocos2d::TMXTiledMap {
public:
	enum TileTyp {
		DEATH, //0
		WALL, //1
		COLLECTABLE, // 2
		SLOPE_LEFT, // 3
		SLOPE_RIGHT, // 4
		STUMP, // 4
        SPAWNPOINT, //5
		NONE //6
	};
	static GameMap* create(const std::string& mapName, float scale);

	/** Converts a map coordinate to the world coordinate */
	cocos2d::Vec2 worldToMap(cocos2d::Vec2 worldCoord);

	/** Converts a world coordinate to the map coordinate. */
	cocos2d::Vec2 mapToWorld(cocos2d::Vec2 mapCoord);

	/** Retrieves the vector for the given object name in a specific group. */
	cocos2d::Vec2 objectPoint(std::string group, std::string objectName);

	/** Retrieves the sprites for each point in the layer "ground" */
	std::vector<TileTyp> groundCollision(std::vector<cocos2d::Vec2> points);

	/** Called when the map is allowed to update **/
	void update(float delta);
    std::vector<Enemy*> getEnemies();
    void removeCollectable(cocos2d::Vec2 worldCoord);
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

	void loadPlatforms();

    void loadEnemies();

    std::vector<Enemy*> enemies;
	std::vector<Platform*> platforms;
	GameMap(const std::string& mapName, float scale);
	std::vector<TileTyp> tiles;
	std::string name;
};

#endif
