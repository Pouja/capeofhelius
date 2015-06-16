#ifndef _CLOUD_H_
#define _CLOUD_H_

#include "cocos2d.h"

/**
* Cloud animations, spawns a cloud, moves it to left and respawns it again.
*/
class Cloud : public cocos2d::Sprite {
public:
	/**
	 * @brief Creates a new Clouds and starts the aimation.
	 * 
	 * @param startPosition The start position of the cloud.
	 * @return pointer to the new cloud.
	 */
	static Cloud* create(cocos2d::Vec2 startPosition);

	/**
	 * @brief Resets the animation and position of the cloud.
	 */
	void reset();
private:
	/**
	 * @brief Starts the animation.
	 */
	void animate();

	cocos2d::Vec2 startPosition;
	Cloud(cocos2d::Vec2 startPosition);
};

#endif