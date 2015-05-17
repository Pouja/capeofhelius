#include "GameMap.h"

USING_NS_CC;

GameMap::GameMap(std::string mapName, float scale) {
    this->name = mapName;
    auto str = String::createWithContentsOfFile(FileUtils::getInstance()->fullPathForFilename(mapName.c_str()).c_str());
    this->initWithXML(str->getCString(), "");
    this->autorelease();
    this->setScale(scale);
}

std::vector<Sprite*> GameMap::groundCollision(std::vector<cocos2d::Vec2> points) {
    std::vector<Sprite*> collisions;
    TMXLayer* layer = this->getLayer("foreground");

    for (Vec2 point : points) {
        Vec2 mapCoord = this->worldToMap(point);
        collisions.push_back(layer->getTileAt(mapCoord));
    }
    return collisions;
}


Vec2 GameMap::worldToMap(Vec2 worldCoord) {
    // Scale with the map size.
    worldCoord.scale(1 / this->getScale());

    float x = floorf(worldCoord.x / this->getTileSize().width);
    float y = floorf(worldCoord.y / this->getTileSize().height) + 1;

    // (0,0) for tilemap is topleft, so we need the inverse.
    y = this->getMapSize().height - y;
    return Vec2(x, y);
}

Vec2 GameMap::mapToWorld(Vec2 mapCoord) {
    float x = mapCoord.x * this->getTileSize().width + this->getTileSize().width / 2;
    float y = (mapCoord.y - 1) * this->getTileSize().height + this->getTileSize().height / 2;
    Vec2 coords(x, y);
    coords.scale(this->getScale());
    return coords;
}

Vec2 GameMap::tileToWorld(Sprite* tile) {
    Vec2 position = tile->getPosition();
    position.scale(this->getScale());
    Vec2 halfTileSize = this->getTileSize();
    halfTileSize.scale(0.5);
    halfTileSize.scale(this->getScale());
    position.add(halfTileSize);
    return position;
}

Vec2 GameMap::objectPoint(std::string group, std::string objectName) {
    TMXObjectGroup* objectGroup = this->getObjectGroup(group);
    assert(objectGroup != nullptr);

    ValueMap object = objectGroup->getObject(objectName);
    int x = object.at("x").asInt() * this->getScale();
    int y = object.at("y").asInt() * this->getScale();

    return Vec2(x, y);
}
GameMap::~GameMap() {}
