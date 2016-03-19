#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cocos2d.h"
#include "physics/PhysicComponent.h"

class Player: public cocos2d::Sprite {
public:
    bool finished = false;
    /**
    * Creates and initializes a new player, sets the pointer to auto release.
    * @param position The position it should be placed.
    * @return the player if it initialized otherwise null.
    */
    static Player* create(cocos2d::Vec2 position, const std::string& name);

    // Overide from IKeyListener
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

    /**
    * Should be called after the desiredPosition is set as the new position.
    * Sets the correct animation to be shown.
    */
    void updateAnimation();

    /** Returns the player state */
    cocos2d::Vec2 getState();

    /**
     * @brief Adds a score point to the player
     */
    void addCoin();
    void clearCoins();
    /**
     * @return The score of the player
     */
    int getScore();

    PhysicComponent* getPhysicBody();

    /**
     * @return The number of lives the player has.
     */
    int getLives();

    /**
     * Executes the die procedure of the player
     * @param callback The callback to be called after the actions of the death animation has completed.
     */
    void die(cocos2d::CallFunc* callback);

    /**
     * Executes the respawn procedure of the player
     * @param position The position to be respawned to.
     */
    void respawn(cocos2d::Vec2 position);
    void stop();
    void moveTo(cocos2d::Rect tilePosition, std::function<void()> onFinish);
    void onMoveFinish();

    ~Player();
private:
    PhysicComponent* physicBody;
    cocos2d::Rect targetRect;
    std::function<void()> callback;

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
        WALKING,
        IDLE,
        JUMPING,
        RUNNING
    };

    // The current animation it is running
    AnimationState animationState;

    // The different animation with the sprite animation initialized
    cocos2d::Animate* running;
    cocos2d::Animate* walking;

    // The player state indicating which direction  it wants to go.
    cocos2d::Vec2 movingState;
};

#endif
