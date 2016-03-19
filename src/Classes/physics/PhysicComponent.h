#ifndef _PHYSIC_COMPONENT_H_
#define _PHYSIC_COMPONENT_H_

#include "2d/CCSprite.h"
#include "math/Vec2.h"
#include "base/GameMap.h"

class PhysicComponent : public cocos2d::Ref {
public:
    enum Type {
        ENEMY,
        PLATFORM,
        TILE,
        PLAYER
    };
    enum BoundingPoint {
        MIDDLE = 0,
        BOTTOM,
        TOP,
        LEFT,
        RIGHT,
        TOP_LEFT,
        TOP_RIGHT,
        BOTTOM_RIGHT,
        BOTTOM_LEFT
    };
    static int collisionOrder[9];
private:
    PhysicComponent(cocos2d::Sprite* body, Type type);
    virtual ~PhysicComponent();

    Type type;
    cocos2d::Sprite* body;
    cocos2d::Vec2 externalForce;
    cocos2d::Vec2 speed;
    cocos2d::Vec2 angle;
    bool onGround;
    cocos2d::Vec2 desiredPosition;
    float bottomOffset;
    float topOffset;
    float leftOffset;
    float rightOffset;
    bool isSolid;
public:
    virtual void init();
    virtual void update(float delta);
    virtual std::vector<cocos2d::Vec2> getBoundingPoints(cocos2d::Vec2 pov);
    virtual cocos2d::Rect getFrame();
    virtual bool onCollision(PhysicComponent* other, BoundingPoint point);
    virtual void onStaticCollisions(std::vector<GameMap::TileType> tiles, std::vector<cocos2d::Vec2> tilePositions,
        cocos2d::Size tileSize);
    virtual bool onCollision(GameMap::TileType tile, BoundingPoint point, cocos2d::Vec2 tilePos,
        cocos2d::Size tileSize);
    static PhysicComponent* create(cocos2d::Sprite* body, Type type);

    /* getters and setters */
    Type getType();
    cocos2d::Vec2 getDesiredPosition();
    cocos2d::Vec2 getExternalForce();
    cocos2d::Vec2 getSpeed();
    cocos2d::Vec2 getPosition();

    bool isOnGround();
    void setOffsets(float bottom, float top, float left, float right);
    void setAngle(cocos2d::Vec2 angle);
    void set();
    void setExternalForce(cocos2d::Vec2 force);
    void setSpeed(cocos2d::Vec2 speed);
};

#endif
