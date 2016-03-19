#ifndef _PHYSIC_ENGINE_H_
#define _PHYSIC_ENGINE_H_

#include <vector>
#include "physics/PhysicComponent.h"
#include "physics/PlayerPhysic.h"
#include "base/CCRef.h"
#include "base/GameMap.h"

class PhysicEngine : public cocos2d::Ref {
private:
    std::vector<PhysicComponent*> movable;
    std::vector<PhysicComponent*> nonMovable;
    GameMap* map;
    PhysicEngine();
    ~PhysicEngine();
public:
    void addMovableComponent(PhysicComponent* component);
    void addStaticComponent(PhysicComponent* component);
    void setMap(GameMap* map);
    void clear();
    void update(float delta);
    void checkCollisions();
    PhysicComponent* hasCollision(cocos2d::Vec2 pos);
    static PhysicEngine* create();
};

#endif
