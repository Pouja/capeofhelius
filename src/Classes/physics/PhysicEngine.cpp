#include "PhysicEngine.h"

PhysicEngine::PhysicEngine() {}

PhysicEngine* PhysicEngine::create() {
    PhysicEngine* engine = new PhysicEngine();
    if (engine) {
        engine->autorelease();
        return engine;
    }
    cocos2d::log("Could not create PhysicEngine");
    return nullptr;
}

void PhysicEngine::addStaticComponent(PhysicComponent *component) {
    component->retain();
    this->nonMovable.push_back(component);
}

void PhysicEngine::addMovableComponent(PhysicComponent *component) {
    component->retain();
    this->movable.push_back(component);
}

void PhysicEngine::setMap(GameMap* map) {
    this->map = map;
}

void PhysicEngine::clear() {
    for (PhysicComponent* comp : this->nonMovable) {
        comp->release();
    }
    for (PhysicComponent* comp : this->movable) {
        comp->release();
    }
    this->nonMovable.clear();
    this->movable.clear();
}

void PhysicEngine::update(float delta) {
    for (PhysicComponent* comp : this->nonMovable) {
        comp->update(delta);
    }
    for (PhysicComponent* comp : this->movable) {
        comp->update(delta);
    }
    this->checkCollisions();
    for (PhysicComponent* comp : this->movable) {
        comp->set();
    }
}

PhysicComponent* PhysicEngine::hasCollision(cocos2d::Vec2 pos) {
    for (PhysicComponent* nonMovable : this->nonMovable) {
        if (nonMovable->getFrame().containsPoint(pos)) {
            return nonMovable;
        }
    }
    return nullptr;
}

void PhysicEngine::checkCollisions() {
    for (PhysicComponent* movable : this->movable) {
        std::vector<cocos2d::Vec2> boundingPoints = movable->getBoundingPoints(movable->getDesiredPosition());

        // Check dynamic collision (with other players/enemies etc.)
        for (const int boundingPoint : PhysicComponent::collisionOrder) {
            PhysicComponent* collision = hasCollision(boundingPoints[boundingPoint]);
            if (collision != nullptr &&
                    movable->onCollision(collision, (PhysicComponent::BoundingPoint)boundingPoint)) {
                break;
            }
        }
        // Check for static collision (with map)
        std::vector<GameMap::TileType> tiles(9);
        std::vector<cocos2d::Vec2> tilePositions(9);
        for (const int boundingPoint : PhysicComponent::collisionOrder) {
            tiles[boundingPoint] = map->groundCollision(boundingPoints[boundingPoint]);
            tilePositions[boundingPoint] = this->map->mapToWorld(
                this->map->worldToMap(boundingPoints[boundingPoint]));
        }
        movable->onStaticCollisions(tiles, tilePositions);
    }
}

PhysicEngine::~PhysicEngine() {
    this->clear();
}
