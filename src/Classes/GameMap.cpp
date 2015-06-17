#include "GameMap.h"
#include "dynamic-scene/Torch.h"
#include "dynamic-scene/Cloud.h"
#include "dynamic-scene/Flag.h"

USING_NS_CC;

GameMap* GameMap::create(const std::string& mapName, float scale) {
    GameMap* gameMap = new GameMap(mapName, scale);
    auto str = String::createWithContentsOfFile(FileUtils::getInstance()->fullPathForFilename(mapName.c_str()).c_str());
    if (gameMap && gameMap->initWithXML(str->getCString(), "")) {
        gameMap->initTiles();
        gameMap->loadDynamicScene();
        gameMap->loadPlatforms();
        gameMap->loadEnemies();
        gameMap->autorelease();
        return gameMap;
    }

    CC_SAFE_DELETE(gameMap);
    return nullptr;
}

GameMap::GameMap(const std::string& mapName, float scale) {
    this->name = mapName;
    this->setScale(scale);
}

void GameMap::update(float delta) {
    for (Platform* platform : platforms) {
        platform->update(delta);
    }
}

void GameMap::loadDynamicScene() {
    TMXObjectGroup* objectGroup = this->getObjectGroup("animations");
    for (Value object : objectGroup->getObjects()) {

        //TODO: Create a seperate super class for torch, flag and cloud and call parse on that class
        ValueMap dict = object.asValueMap();
        std::string name = dict.at("name").asString();
        float x = dict.at("x").asFloat() * this->getScale();
        float y = dict.at("y").asFloat() * this->getScale();
        Vec2 coord = mapToWorld(worldToMap(Vec2(x, y)));
        if (name.compare("torch") == 0) {
            addChild(Torch::create(coord), 1);
        } else if (name.compare("flag") == 0) {
            addChild(Flag::create(coord), 1);
        } else if (name.compare("cloud") == 0) {
            addChild(Cloud::create(coord), -1);
        }
    }
}

void GameMap::loadPlatforms() {
    TMXObjectGroup* objectGroup = this->getObjectGroup("platforms");
    float tileWidth = this->getTileSize().width * this->getScale();
    float tileHeight = this->getTileSize().height * this->getScale();
    for (Value object : objectGroup->getObjects()) {
        //TODO: move this to Platform::parse
        ValueMap dict = object.asValueMap();
        std::string name = dict.at("sprite").asString();

        float x = dict.at("x").asFloat() * this->getScale();
        float y = dict.at("y").asFloat() * this->getScale();
        Vec2 start = Vec2(x, y);

        float xStep = dict.at("move_x").asFloat();
        float yStep = dict.at("move_y").asFloat();
        Vec2 end = Vec2(x + xStep * tileWidth, y + yStep * tileWidth);

        Vec2 velocity = Vec2(tileWidth * 2, tileHeight * 2);
        if (xStep == 0) velocity.x = 0;
        if (yStep == 0) velocity.y = 0;

        bool alternate = dict.find("alternate") != dict.end();
        Platform* platform = Platform::create(start, end, velocity, name, this->getScale(), alternate);

        addChild(platform);
        platforms.push_back(platform);
    }
}

void GameMap::loadEnemies(){
    Size tileSize = this->getTileSize();
    TMXObjectGroup* objectGroup = this->getObjectGroup("enemies");
    for(Value object : objectGroup->getObjects()){
        Enemy* enemy = Enemy::parse(object.asValueMap(), tileSize);
        addChild(enemy, 5);
        this->enemies.push_back(enemy);
    }
}

GameMap::TileTyp parseTileProperties(ValueMap properties) {
    if (!properties.empty()) {
        std::string type = properties["type"].asString();
        if (type.compare("death") == 0) {
            return GameMap::TileTyp::DEATH;
        } else if (type.compare("slope") == 0) {
            if (properties["angle"].asString().compare("left") == 0) {
                return GameMap::TileTyp::SLOPE_LEFT;
            }
            return GameMap::TileTyp::SLOPE_RIGHT;
        } else if (type.compare("stump") == 0) {
            return GameMap::TileTyp::STUMP;
        } else if (type.compare("wall") == 0) {
            return GameMap::TileTyp::WALL;
        } else if (type.compare("collectable") == 0) {
            return GameMap::TileTyp::COLLECTABLE;
        }
    }
    return GameMap::TileTyp::NONE;
}

void GameMap::initTiles() {
    int width = this->getMapSize().width;
    int height = this->getMapSize().height;
    TMXLayer* layer = this->getLayer("static scene");
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++)
        {
            uint32_t tileGID = layer->getTileGIDAt(Vec2(x, y));
            if (tileGID != 0) {
                ValueMap properties = this->getPropertiesForGID(tileGID).asValueMap();
                TileTyp tileType = parseTileProperties(properties);
                this->tiles.push_back(tileType);
            } else {
                this->tiles.push_back(TileTyp::NONE);
            }
        }
    }
    layer->setVisible(false);
}

void GameMap::removeCollectable(cocos2d::Vec2 worldCoord) {
    Vec2 mapCoord = this->worldToMap(worldCoord);

    // First remove it the sprite
    TMXLayer* layer = this->getLayer("items");
    layer->removeTileAt(mapCoord);

    // Update the tile map collisions
    Size mapSize = this->getMapSize();
    int index = (int) mapCoord.x + mapSize.width * mapCoord.y;
    tiles[index] = TileTyp::NONE;
}

std::vector<Platform*> GameMap::getPlatforms() {
    return this->platforms;
}

std::vector<Enemy*> GameMap::getEnemies(){
    return this->enemies;
}

std::vector<GameMap::TileTyp> GameMap::groundCollision(std::vector<cocos2d::Vec2> points) {
    std::vector<GameMap::TileTyp> collisions;
    for (Vec2 point : points) {
        Vec2 mapCoord = this->worldToMap(point);
        Size mapSize = this->getMapSize();
        if (mapCoord.x < 0 || mapCoord.y < 0 || mapCoord.x >= mapSize.width || mapCoord.y >= mapSize.height) {
            collisions.push_back(GameMap::TileTyp::NONE);
        } else {
            int index = (int) mapCoord.x + mapSize.width * mapCoord.y;
            collisions.push_back(tiles[index]);
        }
    }
    return collisions;
}

Vec2 GameMap::worldToMap(Vec2 worldCoord) {
    // Scale with the map size.
    worldCoord.scale(1 / this->getScale());
    float x = floorf(worldCoord.x / this->getTileSize().width);
    float y = floorf(((this->getMapSize().height * this->getTileSize().height) - worldCoord.y) / this->getTileSize().height);

    // (0,0) for tilemap is topleft, so we need the inverse.
    return Vec2(x, y);
}

Vec2 GameMap::mapToWorld(Vec2 mapCoord) {
    float x = (mapCoord.x) * this->getTileSize().width + this->getTileSize().width / 2;
    float y = (this->getMapSize().height - mapCoord.y - 1) * this->getTileSize().height + this->getTileSize().height / 2;
    Vec2 coords(x, y);
    coords.scale(this->getScale());

    return coords;
}

Vec2 GameMap::objectPoint(std::string group, std::string objectName) {
    TMXObjectGroup* objectGroup = this->getObjectGroup(group);
    assert(objectGroup != nullptr);

    ValueMap object = objectGroup->getObject(objectName);
    int x = object.at("x").asInt() * this->getScale();
    int y = object.at("y").asInt() * this->getScale();

    return Vec2(x, y);
}
