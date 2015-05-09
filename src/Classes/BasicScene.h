#ifndef __BASIC_SCENE_H__
#define __BASIC_SCENE_H__
#define COCOS2D_DEBUG 1

#include "cocos2d.h"
#include "math/Vec2.h"
#include "Player.h"

class BasicScene : public cocos2d::Layer
{
private:
    cocos2d::TMXTiledMap *tilemap;
    Player* player;
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void setViewPointCenter(cocos2d::Vec2);
    void createMap();
    void update(float);

    CREATE_FUNC(BasicScene);
};

#endif // __BASIC_SCENE_H__
