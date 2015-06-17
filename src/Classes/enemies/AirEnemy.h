#ifndef _AIR_ENEMY_H_
#define _AIR_ENEMY_H_

#include "Enemy.h"

class AirEnemy : public Enemy{
private:
    bool alt;
    virtual void animate();
public:
    AirEnemy(cocos2d::Vec2 _pointA, cocos2d::Vec2 _pointB, const std::string& _name, cocos2d::Size _size) :
    Enemy(_pointA, _pointB, _name, _size), alt(false){};
};

#endif
