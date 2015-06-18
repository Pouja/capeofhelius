#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "cocos2d.h"

class Enemy : public cocos2d::Sprite {
private:
    cocos2d::Vec2 startPoint;
    cocos2d::Vec2 steps;
    cocos2d::Size stepSize;

    virtual void animate() = 0;

    void setStartPoint(cocos2d::Vec2 startPoint);
public:
    static Enemy* create(Enemy* enemy);
    static Enemy* parse(cocos2d::ValueMap valueMap, cocos2d::Size size);
    cocos2d::Vec2 getStartPoint();
    cocos2d::Vec2 getSteps();
    cocos2d::Size getStepSize();
protected:
    Enemy(cocos2d::Vec2 _startPoint, cocos2d::Vec2 _steps, const std::string& _name, cocos2d::Size _size) :
        startPoint(_startPoint), steps(_steps), stepSize(_size) {
            this->setName(_name);
        };
};

#endif
