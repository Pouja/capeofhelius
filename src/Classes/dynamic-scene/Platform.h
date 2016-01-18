#ifndef _GAME_PLATFORM_H_
#define _GAME_PLATFORM_H_

#include "cocos2d.h"

class Platform : public cocos2d::Sprite
{
public:
	static Platform* create(cocos2d::Vec2 startPosition, cocos2d::Vec2 endPosition, cocos2d::Vec2 velocity, const std::string& name, float scale, bool alternate);
    static Platform* parse(cocos2d::ValueMap dict, float scale, float tileWidth, float tileHeight);
	cocos2d::Vec2 getVelocity();
	void update(float delta);
private:
	void initPosition(bool alternate);
	Platform(cocos2d::Vec2 _start, cocos2d::Vec2 _end, cocos2d::Vec2 _velocity);
	cocos2d::Vec2 start;
	cocos2d::Vec2 end;
	cocos2d::Vec2 velocity;
};

#endif
