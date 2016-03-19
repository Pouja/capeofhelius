#include "physics/PhysicComponent.h"

#define GRAVITY -1.0
#define FRICTION 5.0
#define HORIZONTAL_FORCE 2.0
#define VERTICAL_FORCE 20.0
#define MAX_HORIZONTAL_speed 10.0
#define MAX_VERTICAL_speed 20.0
#define MIN_HORIZONTAL_speed -10.0
#define MIN_VERTICAL_speed -15.0

USING_NS_CC;

int PhysicComponent::collisionOrder[] = {
    MIDDLE, BOTTOM, TOP, LEFT, RIGHT, TOP_LEFT, TOP_RIGHT, BOTTOM_RIGHT, BOTTOM_LEFT
};

PhysicComponent::PhysicComponent(cocos2d::Sprite* _body, Type _type) :
    type(_type),
    body(_body),
    externalForce(Vec2::ZERO),
    speed(Vec2::ZERO),
    angle(Vec2::ZERO),
    onGround(false),
    bottomOffset(0.0f),
    topOffset(0.0f),
    leftOffset(0.0f),
    rightOffset(0.0f),
    isSolid(true) {
    this->desiredPosition = body->getPosition();
    this->body->retain();
}

PhysicComponent::~PhysicComponent() {
    this->body->release();
}

PhysicComponent* PhysicComponent::create(cocos2d::Sprite *body, PhysicComponent::Type type) {
    PhysicComponent* comp = new PhysicComponent(body, type);
    if (comp != nullptr) {
        comp->init();
        comp->autorelease();
        return comp;
    }
    cocos2d::log("Failed to create physic component.");
    return nullptr;
}

void PhysicComponent::init() {}

void PhysicComponent::update(float delta) {
    Vec2 gravity(0.0, GRAVITY);
    this->speed.add(gravity);

    Vec2 horizontalForce(HORIZONTAL_FORCE, 0.0);
    horizontalForce.scale(this->angle.x);

    Vec2 verticalForce(0.0, VERTICAL_FORCE);
    verticalForce.scale(this->angle.y);

    if (this->onGround) {
        this->speed.add(verticalForce);
        horizontalForce.scale(1 / FRICTION);
        this->speed.add(horizontalForce);
    } else if (this->angle.x == 0 ||
               (this->angle.x == -1 && this->speed.x >= 0) ||
               (this->angle.x == 1 && this->speed.x <= 0)) {
        this->speed.add(horizontalForce);
    }
    if (this->angle.x == 0 && this->onGround) {
        this->speed = Vec2(this->speed.x * 0.1, this->speed.y);
    }

    Vec2 minMovement(MIN_HORIZONTAL_speed, MIN_VERTICAL_speed);
    Vec2 maxMovement(MAX_HORIZONTAL_speed, MAX_VERTICAL_speed);

    this->speed.clamp(minMovement, maxMovement);
    this->desiredPosition = this->getPosition() + this->speed + this->externalForce;
    this->externalForce = Vec2::ZERO;
}

std::vector<cocos2d::Vec2> PhysicComponent::getBoundingPoints(Vec2 pov) {
    std::vector<Vec2> points(8);
    Rect boundingBox = this->body->getBoundingBox();
    float left = pov.x - (30 / 2) - this->leftOffset;
    float right = pov.x + (30 / 2) - this->rightOffset;
    float top = pov.y + (55 / 2) - this->topOffset;
    float bottom = pov.y - (55 / 2) - this->bottomOffset;

    points[BOTTOM] = Vec2(pov.x, bottom);
    points[TOP] = Vec2(pov.x, top);
    points[LEFT] = Vec2(left, pov.y);
    points[RIGHT] = Vec2(right, pov.y);
    points[TOP_LEFT] = Vec2(pov.x, top);
    points[BOTTOM_LEFT] = Vec2(pov.x, bottom);
    points[BOTTOM_RIGHT] = Vec2(pov.x, bottom);
    points[TOP_RIGHT] = Vec2(pov.x, top);
    points[MIDDLE] = Vec2((left + right) / 2 , (top + bottom) / 2);
    return points;
}

cocos2d::Rect PhysicComponent::getFrame() {
    return this->body->getBoundingBox();
}

bool PhysicComponent::onCollision(PhysicComponent* other, BoundingPoint point) {
    return false;
}

void resolveVertCollision(float tileHeight, float playerHeight, Vec2 tilePos,
                          Vec2* speed, Vec2* desiredPosition) {
    if (desiredPosition->y > tilePos.y) {
        desiredPosition->y = tilePos.y  + playerHeight / 2  + tileHeight / 2;
        speed->y = fmaxf(speed->y, 0);
    } else {
        desiredPosition->y = tilePos.y - playerHeight / 2  - tileHeight / 2;
        speed->y = fminf(speed->y, 0);
    }
}

void resolveHorCollision(float tileWidth, float playerWidth, Vec2 tilePos,
                         Vec2* speed, Vec2* desiredPosition) {
    if (desiredPosition->x > tilePos.x) {
        desiredPosition->x = tilePos.x + playerWidth / 2 + tileWidth / 2;
    } else {
        desiredPosition->x = tilePos.x - playerWidth / 2 - tileWidth / 2;
    }
    speed->x = 0;
    speed->y = fminf(speed->y, 0);
}

void resolveSlopeCollision(Vec2 tilePos, float playerHeight, Vec2* desiredPosition, bool isLeftSlope) {
    float a = (isLeftSlope) ? 1.0 : -1.0;
    float b = tilePos.y + (playerHeight / 2) - a * tilePos.x;
    desiredPosition->y = (a * desiredPosition->x) + b;
}

void PhysicComponent::onStaticCollisions(std::vector<GameMap::TileType> tiles,
        std::vector<cocos2d::Vec2> tilePositions) {
    this->onGround = (tiles[BoundingPoint::BOTTOM] == GameMap::TileType::SLOPE_LEFT
                      || tiles[BoundingPoint::BOTTOM] == GameMap::TileType::SLOPE_LEFT
                      || tiles[BoundingPoint::BOTTOM] == GameMap::TileType::SLOPE_RIGHT
                      || tiles[BoundingPoint::BOTTOM] == GameMap::TileType::STUMP
                      || tiles[BoundingPoint::BOTTOM] == GameMap::TileType::WALL);
    int collisionCount = 0;
    for (const int point : collisionOrder) {
        collisionCount += (onCollision(tiles[point], (BoundingPoint)point, tilePositions[point])) ? 1 : 0;
        // if (collisionCount > 0 && (point == BOTTOM_RIGHT || point == BOTTOM_LEFT || point == TOP_RIGHT ||
        //                            point == TOP_LEFT )) {
        //     break;
        // }
    }
}

bool PhysicComponent::onCollision(GameMap::TileType tile, BoundingPoint point, Vec2 tilePos) {
    if (!isSolid) {
        return true;
    }
    std::vector<cocos2d::Vec2> frame = this->getBoundingPoints(desiredPosition);
    float playerWidth = frame[RIGHT].x - frame[LEFT].x;
    float playerHeight = frame[TOP].y - frame[BOTTOM].y;
    float tileWidth = 70;
    float tileHeight = 70;

    if ((tile == GameMap::TileType::SLOPE_LEFT || tile == GameMap::TileType::SLOPE_RIGHT)) {
        bool isLeft = tile == GameMap::TileType::SLOPE_LEFT;
        resolveSlopeCollision(tilePos, playerHeight, &this->desiredPosition, isLeft);
        return true;
    } else if (tile == GameMap::TileType::STUMP && point == BOTTOM) {
        // This is kinda of hack situation. What happening is that when you hit a stump, you are actually on a slope
        // So we take the tile above (which is the -= tileHeight) and
        // resolve as if we are colliding with the slop tile.
        tilePos.y -= tileHeight;
        resolveVertCollision(tileHeight, playerHeight, tilePos, &this->speed,
                              &this->desiredPosition);
    } else if (tile == GameMap::TileType::WALL) {
        if (fabsf(tilePos.x - desiredPosition.x) > fabsf(tilePos.y - desiredPosition.y)) {
            resolveHorCollision(tileWidth, playerWidth,  tilePos, &this->speed,
                                &this->desiredPosition);
        } else {
            resolveVertCollision(tileWidth, playerHeight, tilePos, &this->speed,
                                 &this->desiredPosition);
        }
        return true;
    }
    return false;
}

PhysicComponent::Type PhysicComponent::getType() {
    return this->type;
}

cocos2d::Vec2 PhysicComponent::getDesiredPosition() {
    return this->desiredPosition;
}

cocos2d::Vec2 PhysicComponent::getSpeed() {
    return this->speed;
}

cocos2d::Vec2 PhysicComponent::getPosition() {
    return this->body->getPosition();
}

void PhysicComponent::set() {
    this->body->setPosition(this->desiredPosition);
}

void PhysicComponent::setExternalForce(cocos2d::Vec2 force) {
    this->externalForce = force;
}

void PhysicComponent::setSpeed(cocos2d::Vec2 speed) {
    this->speed = speed;
}

void PhysicComponent::setOffsets(float bottom, float top, float left, float right) {
    this->bottomOffset = bottom;
    this->topOffset = top;
    this->leftOffset = left;
    this->rightOffset = right;
}

bool PhysicComponent::isOnGround() {
    return this->onGround;
}

void PhysicComponent::setAngle(cocos2d::Vec2 angle) {
    this->angle = angle;
}
