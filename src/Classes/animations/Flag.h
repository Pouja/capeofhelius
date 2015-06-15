#ifndef _GAME_FLAG_H_
#define _GAME_FLAG_H_

#include "cocos2d.h"

class Flag : public cocos2d::Sprite {
public:
    /**
     * @brief Creates a new Flag and starts the animation.
     *
     * @param position
     * @return pointer to the flag
     */
    static Flag* create(cocos2d::Vec2 position);

    /**
     * @brief Starts the animation of the flag.
     */
    void start();

    /**
     * @brief Stops the animation of the flag.
     */
    void stop();
private:
    /**
     * @brief Creates a new flag with the given position
     *
     * @param position
     */
    Flag(cocos2d::Vec2 position);
};

#endif