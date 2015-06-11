#ifndef _FLAG_H_
#define _FLAG_H_

#include "cocos2d.h"

class Torch : public cocos2d::Sprite {
public:
	static Torch* create(cocos2d::Vec2 position);

	void start();
	void stop();
private:
	Torch(cocos2d::Vec2 position);
};

#endif