#ifndef _GROUND_ENEMY_H
#define _GROUND_ENEMY_H

#include "cocos2d.h"

class GroundEnemy : public cocos2d::Sprite {
private:
    GroundEnemy(): timeout(0) {};
    cocos2d::Animate* animation;
    float timeout;
public:
    static GroundEnemy* create(cocos2d::Vec2 position);
    void update(float delta);
    void initAnimation();
};

#endif
