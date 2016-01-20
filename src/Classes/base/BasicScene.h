#ifndef __BASIC_SCENE_H__
#define __BASIC_SCENE_H__

#include "cocos2d.h"
#include "GameMap.h"
#include "math/Vec2.h"
#include "player/Player.h"
#include "GameHub.h"
#include "utils/Background.h"
#include "dialog/DialogAction.h"

class BasicScene : public cocos2d::Layer
{
//TODO: think about which attributes should go in protected, which should go to public (with only getters)
// And which should stay private.
protected:
    std::string mapName;
    std::string mainName;
    std::string bgLocation;
    std::vector<Player*> players;
    cocos2d::Vec2 respawnPoint;
    GameMap* map;
    GameHub* hub;

    // TODO: create addDialog which merges attribute dialogs with dialogRects
    std::map<std::string, DialogAction*> dialogs;
    DialogAction* activeDialog;

    // TODO: make this a map as well
    std::vector<std::pair<std::string, cocos2d::Rect>> dialogRects;

    // The main player
    Player* mainPlayer;
private:
    // If this is set to true it will not update the player or the enemies or check for collisions
    bool paused;

    Background* bg;

    /**
     * @brief Resolves the collision of the given player.
     * @details Fetches the 8 bounding points of the player and checks for each point if it collides with the static scene.
     * The static scene is the set of all sprites (aka tiles in this context) which do not move.
     * Sets the position and velocity of the player at the end of the function in such a way that is it not colliding anymore.
     * @param player
     */
    void resolveCollision(Player* player);

    /**
     * @brief Checks if there is a collision with a platform and resolves it.
     * @details Loops through all the platforms and checks if the bottom or top point of the player is colliding with a moving platform.
     * If it colliding, it sets the external force and resolves the vertical collision.
     * @param player
     * @param delta Time in seconds since the last call of this method.
     */
    void resolvePlatforms(Player* player, float delta);

    /**
     * @brief Resolves the slope collision
     * @details It assumes that the player has collided with a slope and sets the position in such a way that is not colliding anymore.
     *
     * @param tilePos The position of the tile in the world.
     * @param playerHeight The height of the player in px.
     * @param desiredPosition The pointer to the position to be altered.
     * @param isLeftSlope if true it means that the collided tile is a slope with an angle of -1 otherwise 1.
     */
    void resolveSlopeCollision(cocos2d::Vec2 tilePos, float playerHeight, cocos2d::Vec2* desiredPosition, bool isLeftSlope);

    /**
     * @brief Resolves a vertical collision.
     * @details Checks whether the player is colliding at top or at bottom and sets the position accordingly.
     * Sets the velocity.y to zero.
     * @param tileHeight The height of the tile or sprite with which it collided.
     * @param playerHeight The player height.
     * @param tilePos The tile position in the world.
     * @param velocity The pointer to the velocity to be altered.
     * @param desiredPosition The pointer to the position to be altered.
     */
    void resolveVertCollision(float tileHeight, float playerHeight, cocos2d::Vec2 tilePos,
        cocos2d::Vec2* velocity, cocos2d::Vec2* desiredPosition);

    /**
     * @brief Resolves a horizontal collision
     * @details Checks weheter the player is colliding at left or right and sets the position accordingly.
     *
     * @param tileWidth
     * @param playerWidth
     * @param tilePos
     * @param desiredPosition
     */
    void resolveHorCollision(float tileWidth, float playerWidth, cocos2d::Vec2 tilePos,
        cocos2d::Vec2* velocity, cocos2d::Vec2* desiredPosition);

    /**
     * Checks if the position of the player matches the start of a dialog and if the condition
     * of the dialog are set.
     * If so, it sets the currentDialog to the matching dialog and runs it.
     */
    void checkDialog();

    /**
     * Called when the player is at a spawnpoint.
     * Should contain logic of setting new spawnpoint, creating new save entry etc.
     * @param position The position of the spawnpoint reaced.
     */
    void onSpawnpoint(cocos2d::Vec2 position);

    /**
     * Checks if the player is colliding with an enemy by doing rectangle collision.
     * Also contains logic when the collision is true.
     */
    void checkEnemyCollision();

    /**
     * Called when the player collides with a collectable.
     * Removes the collectable from the map and sets the coins.
     * @param position The position of the collectable
     */
    void onCollectable(cocos2d::Vec2 position);

    /**
     * Called when the player has met the death criteria.
     * Resets the player position the last spawnpoint or ends the game, depending on the criteria.
     */
    void onDeath();

    /**
     * Called when the win condition is met.
     */
    virtual void onFinish() = 0;

    /**
     * Called when the condition of a specific dialog should be checked.
     * @param  id The id of the dialog
     * @return    true iff the conditions of the dialog are met, false otherwise.
     */
    virtual bool dialogCondition(const std::string& id) = 0;
public:
    /**
     * @brief Initializes all the sprites, the map, sets the viewpoint etc.
     * @return true if it succeeded in setting everything, false otherwise.
     */
    virtual bool init();
    virtual bool initPlayers() = 0;
    /**
     * @brief Sets the view point of the scene.
     * @details [long description]
     *
     * @param position The view point.
     * @return the new centerpoint
     */
    cocos2d::Vec2 getViewPointCenter(cocos2d::Vec2 position);

    //TODO: think about whether the basicscene should hold the full control of input or not
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    /**
     * @brief Called on each game tick
     * @details Updates the physics of the player, the position of the map and gamehub,
     * updates the position of all the platforms etc.
     * In short it calls the update function of all nodes that want to update on each game tick
     *
     * @param delta The time since the last time this method was called in seconds.
     */
    void update(float delta);

    /**
     * Called at each game tick update the VPC for the map, gamehub and background.
     * @param vpc The new view point center.
     */
    void updateVPC(cocos2d::Vec2 vpc);

    /**
     * Called when the game over conditions are met.
     */
    virtual void onGameOver() = 0;

    /**
     * Contains the logic for setting all the necessary variables.
     * Should be called before ::init()
     * @return true iff it succeeded in setting the variables.
     * When false is returned the game will crash.
     */
    virtual bool initVariables() = 0;
};

#endif // __BASIC_SCENE_H__
