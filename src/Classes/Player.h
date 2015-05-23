#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cocos2d.h"

class Player: public cocos2d::Sprite {
public:
    Player(cocos2d::Vec2 position);
    
    // Overide from IKeyListener
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

    /** Gets the 8 surrounding points of the sprite with respect to the pov.
    The points are returned in the following order: bottom, top, left, right, left top, left bottom, right bottom, right top.
    @param pov The point of view from which the 8 points should be retrieved.
    @return vector of points.
    */
    std::vector<cocos2d::Vec2> getBoundingPoints(cocos2d::Vec2 pov);
    
    /** Called when the player needs to updated his position and or other logic. */
    void update(float delta);

    /** Returns the desired position of this sprite */
    cocos2d::Vec2 getDesiredPosition();

    /** Returns the player state */
    cocos2d::Vec2 getState();

    // velocity and isOnGround are public and are allowed to be freely changed.
    // TODO make methods like: hitGround, isInAir, isOnSlope, isOnGround, instead of letting using public members
    cocos2d::Vec2 velocity;
    bool isOnGround;

    ~Player();
private:
    // The desired position after resolving the gravity, velocity etc.
    cocos2d::Vec2 desiredPosition;

    // The player state indicating which direction  it wants to go.
    cocos2d::Vec2 playerState;
};

#endif