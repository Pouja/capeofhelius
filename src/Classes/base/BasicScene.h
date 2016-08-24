#ifndef __BASIC_SCENE_H__
#define __BASIC_SCENE_H__

#include "cocos2d.h"
#include "GameMap.h"
#include "math/Vec2.h"
#include "player/Player.h"
#include "GameHub.h"
#include "utils/Background.h"
#include "dialog/DialogAction.h"
#include "physics/PhysicEngine.h"

class BasicScene : public cocos2d::Layer
{
//TODO: think about which attributes should go in protected, which should go to public (with only getters)
// And which should stay private.
protected:
    std::string mapName;
    std::string mainName;
    std::string bgLocation;
    std::map<std::string, Player*> players;
    cocos2d::Vec2 respawnPoint;
    GameMap* map;
    GameHub* hub;
    PhysicEngine* physicEngine;

    // For debugging
    cocos2d::Label* mouseLabel;

    // TODO: create addDialog which merges attribute dialogs with dialogRects
    std::map<std::string, DialogAction*> dialogs;
    DialogAction* activeDialog;

    // TODO: make this a map as well
    std::vector<std::pair<std::string, cocos2d::Rect>> dialogRects;

    std::vector<Platform*> platforms;

    // The main player
    Player* mainPlayer;
private:
    // If this is set to true it will not update the player or the enemies or check for collisions
    bool paused;

    Background* bg;

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
     * Initializes all the players
     */
    void initPlayers();

    /**
     * Initializes all the platforms and starts their movement.
     */
    void loadPlatforms();

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
