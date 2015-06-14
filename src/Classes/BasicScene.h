#ifndef __BASIC_SCENE_H__
#define __BASIC_SCENE_H__

#include "cocos2d.h"
#include "GameMap.h"
#include "math/Vec2.h"
#include "Player.h"
#include "GameHub.h"

class BasicScene : public cocos2d::Layer
{
private:
	GameMap::CollisionType previousEvent;

	// If this is set to true it will not update the player or the enemies or check for collisions
	bool paused;

	GameMap* map;
	GameHub* hub;

	// The main player
	Player* mainPlayer;

	// Collision resolving
	void resolveEvent(Player*);
	void onEventEnter(GameMap::CollisionType, cocos2d::Vec2);
	void onEventLeave(GameMap::CollisionType, cocos2d::Vec2);
	void resolveCollision(Player* player);
	void resolvePlatforms(Player* player, float delta);
	void resolveSlopeCollision(cocos2d::Vec2 tilePos, float playerHeight, cocos2d::Vec2* desiredPosition, bool isLeftSlope);
	void resolveVertCollision(float tileHeight, float playerHeight, cocos2d::Vec2 tilePos, cocos2d::Vec2* velocity, cocos2d::Vec2* desiredPosition);
	void resolveHorCollision(float tileWidth, float playerWidth, cocos2d::Vec2 tilePos, cocos2d::Vec2* desiredPosition);
public:
	/** Creates the parent scenes and adds this scene instance to the parent */
	static cocos2d::Scene* createScene();

	/** Initializes all the sprites, the map, sets the viewpoint etc. */
	virtual bool init();

	/** Sets the view point of the scene.
	* @param position The center point
	*/
	cocos2d::Vec2 getViewPointCenter(cocos2d::Vec2 position);

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	/** Called on each game tick */
	void update(float);

	CREATE_FUNC(BasicScene);
};

#endif // __BASIC_SCENE_H__
