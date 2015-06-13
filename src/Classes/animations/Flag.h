#ifndef _GAME_FLAG_H_
#define _GAME_FLAG_H_

#include "cocos2d.h"

class Flag : public cocos2d::Sprite {
public:
	static Flag* create(cocos2d::Vec2 position);

	void start();
	void stop();
private:
	Flag(cocos2d::Vec2 position);
};

#endif