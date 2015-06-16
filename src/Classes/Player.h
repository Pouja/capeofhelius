#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cocos2d.h"

class Player: public cocos2d::Sprite {
public:
    /**
    * Creates and initializes a new player, sets the pointer to auto release.
    * @param position The position it should be placed.
    * @return the player if it initialized otherwise null.
    */
    static Player* create(cocos2d::Vec2 position, const std::string& name);

    // Overide from IKeyListener
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

    /** Gets the 8 surrounding points of the sprite with respect to the pov.
    * The points are returned in the following order: bottom, top, left, right, left top, left bottom, right bottom, right top.
    * @param pov The point of view from which the 8 points should be retrieved.
    * @return vector of points.
    */
    std::vector<cocos2d::Vec2> getBoundingPoints(cocos2d::Vec2 pov);

    /** Called when the player needs to updated his position and or other logic. */
    void updatePhysics();

    /**
    * Should be called after the desiredPosition is set as the new position.
    * Sets the correct animation to be shown.
    */
    void updateAnimation();

    /** Returns the desired position of this sprite */
    cocos2d::Vec2 getDesiredPosition();

    /** Returns the player state */
    cocos2d::Vec2 getState();

    // velocity and isOnGround are public and are allowed to be freely changed.
    // TODO make methods like: hitGround, isInAir, isOnSlope, isOnGround, instead of letting using public members
    cocos2d::Vec2 velocity;
    bool isOnGround;

    /**
     * @brief Adds a score point to the player
     */
    void addCoin();

    /**
     * @return The score of the player
     */
    int getScore();

    /**
     * @return The number of lives the player has.
     */
    int getLives();

    /**
     * @brief Adds an external force to the player.
     * @details The force will be added after the physics is applied.
     *
     * @param force
     */
    void setExternalForce(cocos2d::Vec2 force);

    /**
     * Executes the die procedure of the player
     * @param callback The callback to be called after the actions of the death animation has completed.
     */
    void die(cocos2d::CallFunc* callback);

    /**
     * Executes the respawn procedure of the player
     * @param position The position to be respawned to.
     * @param callback The callback for when the animations has finished.
     */
    void respawn(cocos2d::Vec2 position, cocos2d::CallFunc* callback);

    ~Player();
private:
    int coins;
    int lives;

    std::string name;

    /**
    * Sets the default values for the player.
    * @param position The position in the parent node that it should be placed.
    */
    Player(cocos2d::Vec2 position, const std::string& name);

    /** Initializes all the animation, should be called in the create function.*/
    void initAnimations();

    // The different animation state in which the player can be.
    enum AnimationState{
        WALKING_LEFT,
        WALKING_RIGHT,
        IDLE_LEFT,
        IDLE_RIGHT,
        JUMP_LEFT,
        JUMP_RIGHT,
        RUNNING_LEFT,
        RUNNING_RIGHT
    };

    // The current animation it is running
    AnimationState animationState;

    // The different animation with the sprite animation initialized
    cocos2d::Animate* walkLeft;
    cocos2d::Animate* runningLeft;
    cocos2d::Animate* runningRight;
    cocos2d::Animate* walkRight;

    // The desired position after resolving the gravity, velocity etc.
    cocos2d::Vec2 desiredPosition;

    // The player state indicating which direction  it wants to go.
    cocos2d::Vec2 movingState;

    // The external force to be applied on each tick.
    cocos2d::Vec2 externalForce;
};

#endif
