#ifndef __BASIC_SCENE_H__
#define __BASIC_SCENE_H__
#define COCOS2D_DEBUG 1

#include "cocos2d.h"
#include "math/Vec2.h"

class BasicScene : public cocos2d::Layer
{
private:
    cocos2d::TMXTiledMap *tilemap;
    cocos2d::TMXLayer *background;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    void setViewPointCenter(cocos2d::Vec2);

    // implement the "static create()" method manually
    CREATE_FUNC(BasicScene);

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
};

#endif // __BASIC_SCENE_H__
