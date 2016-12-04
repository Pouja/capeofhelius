#ifndef _GAME_CAMERA_H_
#define _GAME_CAMERA_H_

#include "cocos2d.h"
#include "player/Player.h"

class GameCamera {
private:
    cocos2d::Vec2 vpc;
    Player* following;
    bool runningAction = false;
public:
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void followPlayer(Player* p);
    void update();

    /**
     * @brief Sets the view point of the scene.
     * @details [long description]
     *
     * @param position The view point.
     * @return the new centerpoint
     */
    cocos2d::Vec2 calcVpc(cocos2d::Vec2 position);

    cocos2d::Vec2 getVPC() {
        return this->vpc;
    }

    void moveTo(cocos2d::Vec2 pos);
    GameCamera(){}
    ~GameCamera() {}
};

#endif
