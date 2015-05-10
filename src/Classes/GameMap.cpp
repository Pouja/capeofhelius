#include "GameMap.h"

USING_NS_CC;

GameMap::GameMap(std::string mapName, float scale) {
    this->name = mapName;
    auto str = String::createWithContentsOfFile(FileUtils::getInstance()->fullPathForFilename(mapName.c_str()).c_str());
    this->initWithXML(str->getCString(), "");
    this->autorelease();
    this->setScale(scale);
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
    return Vec2(0,0);
}

Vec2 GameMap::objectPoint(std::string group, std::string objectName) {
    TMXObjectGroup* objectGroup = this->getObjectGroup(group);
    if(!objectGroup){
        log("The map %s has no group %s", this->name.c_str(), group.c_str());
    }
    ValueMap object = objectGroup->getObject(objectName);
    // if(!object){
    //     log("The map %s with group %s has no object %s", this->name.c_str(), group.c_str(), objectName.c_str());

    // }
    int x = object.at("x").asInt() * this->getScale();
    int y = object.at("y").asInt() * this->getScale();

    return Vec2(x, y);
}
GameMap::~GameMap() {}
