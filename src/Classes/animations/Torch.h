#ifndef _FLAG_H_
#define _FLAG_H_

#include "cocos2d.h"

class Torch : public cocos2d::Sprite {
public:
	/**
	* @brief Creates a new Torch and starts the animation.
	*
	* @param position
	* @return pointer to the Torch
	*/
	static Torch* create(cocos2d::Vec2 position);

    /**
     * @brief Starts the animation of the torch.
     */
	void start();

    /**
     * @brief Stops the animation of the torch.
     */
	void stop();
private:
	Torch(cocos2d::Vec2 position);
};

#endif