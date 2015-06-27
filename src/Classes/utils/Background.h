#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include "cocos2d.h"

/**
 * Represents the background in the game as a parallax.
 */
class Background : public cocos2d::Sprite
{
private:
	cocos2d::Size mapSize;
	Background(cocos2d::Size _mapSize): mapSize(_mapSize) {};
public:
    /**
     * Creates a new background with the given sprite file name.
     * @param  filename The sprite png file name.
     * @param  mapSize  The site of the map in pixels.
     * @return          The background
     */
	static Background* create(const std::string& filename, cocos2d::Size mapSize);

    /**
     * Asks the background to move to a new positon relative to the given vpc.
     * @param newPosition The new vpc.
     */
	void move(cocos2d::Vec2 newPosition);
};

#endif
