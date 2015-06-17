#include "Enemy.h"
#include "GroundEnemy.h"
#include "AirEnemy.h"

USING_NS_CC;

Enemy* Enemy::create(Enemy* enemy) {
    if (enemy && enemy->initWithSpriteFrameName(enemy->getName() + ".png")) {
        Vec2 pos = enemy->getStartPoint();
        pos.y = pos.y + (enemy->getContentSize().height / 2.0);
        enemy->setStartPoint(pos);
        enemy->setPosition(pos);
        enemy->animate();
        enemy->autorelease();
        return enemy;
    }
    CC_SAFE_DELETE(enemy);
    return nullptr;
}

Enemy* Enemy::parse(ValueMap valueMap, Size tileSize) {
    std::string type = valueMap.at("type").asString();
    std::string name = valueMap.at("name").asString();

    float x = valueMap.at("x").asFloat();
    float y = valueMap.at("y").asFloat();
    Vec2 startPoint(x, y);

    Vec2 steps = Vec2::ZERO;
    if (valueMap.find("move_x") != valueMap.end()) {
        steps.x =  valueMap.at("move_x").asInt();
    }
    if (valueMap.find("move_y") != valueMap.end()) {
        steps.y =  valueMap.at("move_y").asInt();
    }

    Enemy* enemy = nullptr;
    if (type.compare("ground") == 0) {
        enemy = new GroundEnemy(startPoint, steps, name, tileSize);
    } else if(type.compare("air") == 0) {
        enemy = new AirEnemy(startPoint, steps, name, tileSize);
    }

    assert(enemy != nullptr);
    return Enemy::create(enemy);
}

Size Enemy::getStepSize() {
    return this->stepSize;
}

Vec2 Enemy::getStartPoint() {
    return this->startPoint;
}

Vec2 Enemy::getSteps() {
    return this->steps;
}

void Enemy::setStartPoint(cocos2d::Vec2 point) {
    this->startPoint = point;
}
