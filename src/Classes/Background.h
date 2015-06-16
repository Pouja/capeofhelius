#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include "cocos2d.h"

class Background : public cocos2d::Sprite
{
private:
	cocos2d::Size mapSize;
	Background(cocos2d::Size _mapSize): mapSize(_mapSize) {};
public:
	static Background* create(const std::string& filename, cocos2d::Size mapSize);
	void move(cocos2d::Vec2 newPosition);
};

#endif