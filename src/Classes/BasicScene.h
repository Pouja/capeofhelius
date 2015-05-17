#ifndef __BASIC_SCENE_H__
#define __BASIC_SCENE_H__
#define COCOS2D_DEBUG 1

#include "cocos2d.h"
#include "GameMap.h"
#include "math/Vec2.h"
#include "Player.h"

class BasicScene : public cocos2d::Layer
{
private:
    cocos2d::DrawNode* drawNode;

    // The TMXTileMap to use with this scene
    GameMap *map;

    // The main player
    Player* mainPlayer;

    // Collision resolving
    void resolveCollision(Player* player);
    void resolveVertCollision(float playerHeight, cocos2d::Vec2 tilePos, cocos2d::Vec2* velocity, cocos2d::Vec2* desiredPosition);
    void resolveHorCollision(float playerWidth, cocos2d::Vec2 tilePos, cocos2d::Vec2* desiredPosition);
public:
    /** Creates the parent scenes and adds this scene instance to the parent */
    static cocos2d::Scene* createScene();

    /** Initializes all the sprites, the map, sets the viewpoint etc. */
    virtual bool init();

    /** Sets the view point of the scene.
    * @param position The center point
    */
    void setViewPointCenter(cocos2d::Vec2 position);

    /** Called on each game tick */
    void update(float);

    CREATE_FUNC(BasicScene);
};

#endif // __BASIC_SCENE_H__
