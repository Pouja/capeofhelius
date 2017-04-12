#include "GameCamera.h"
#include "Game.h"

USING_NS_CC;

void GameCamera::followPlayer(Player *p) {
    assert(p);
    if (this->following != p) {
        this->following = p;
        this->following->retain();
        this->moveTo(p->getPosition());
    }
}

void GameCamera::moveTo(cocos2d::Vec2 pos) {
    Camera* camera = Camera::getDefaultCamera();

    Vec3 current = camera->getPosition3D();

    if (runningAction) {
        return;
    }

    if(pos.distance(Vec2(current.x, current.y)) < 50.0f) {
        camera->setPosition3D(Vec3(pos.x, pos.y, current.z));
    } else if(!runningAction){
        MoveTo* moveTo = MoveTo::create(2.0f, Vec3(pos.x, pos.y, current.z));
        CallFunc* onFinished = CallFunc::create([this](){
            this->runningAction = false;
        });
        Sequence* sequence = Sequence::create(moveTo, onFinished, nullptr);

        this->runningAction = true;
        camera->runAction(sequence);
    }
}

void GameCamera::update() {
    if (this->following != nullptr) {
        this->vpc = calcVpc(this->following->getPosition());
        this->moveTo(vpc);
    }
}

Vec2 GameCamera::calcVpc(cocos2d::Vec2 position) {
    Size winSize = Director::getInstance()->getWinSize();
    GameMap* map = GameObjects::getInstance()->getMap();

    // mapsize returns the number of tiles ... confusing name...
    Size mapSize = map->getMapSize();

    // Calculating the actual map width and height
    float mapWidth = mapSize.width * map->getTileSize().width * map->getScale();
    float mapHeight = mapSize.height * map->getTileSize().height * map->getScale();

    // We are calculating the following four points:
    /* * * * * * * * * * * *
     * p * * * * * * * * p *
     * * * * * * * * * * * *
     * * * * * * * * * * * *
     * p * * * * * * * * p *
     * * * * * * * * * * * *
     */
    float yMin = winSize.height / 2;
    float yMax = mapHeight - (winSize.height / 2);

    float xMin = winSize.width / 2;
    float xMax = mapWidth - (winSize.width / 2);

    // And now just adjusting the x and y to fall between those four points
    float xView = fmin(xMax, fmax(xMin, position.x));
    float yView = fmin(yMax, fmax(yMin, position.y));

    return Vec2(xView, yView);
}
