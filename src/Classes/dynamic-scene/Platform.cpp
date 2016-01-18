#include "Platform.h"

USING_NS_CC;

Vec2 Platform::getVelocity() {
	return this->velocity;
}

Platform::Platform(cocos2d::Vec2 _start, cocos2d::Vec2 _end, cocos2d::Vec2 _velocity) :
	start(_start), end(_end), velocity(_velocity) {}


Platform* Platform::create(Vec2 start, Vec2 end, Vec2 velocity, const std::string& name, float scale, bool alternate) {
	Platform* platform = new Platform(start, end, velocity);
	if (platform && platform->initWithFile(name)) {
		platform->setScale(scale);
		platform->autorelease();
		platform->initPosition(alternate);
		return platform;
	}
	CC_SAFE_DELETE(platform);
	return nullptr;
}

Platform* Platform::parse(cocos2d::ValueMap dict, float scale, float tileWidth, float tileHeight) {
        std::string name = dict.at("sprite").asString();

        float x = dict.at("x").asFloat() * scale;
        float y = dict.at("y").asFloat() * scale;
        Vec2 start = Vec2(x, y);

        float xStep = dict.at("move_x").asFloat();
        float yStep = dict.at("move_y").asFloat();
        Vec2 end = Vec2(x + xStep * tileWidth, y + yStep * tileWidth);

        Vec2 velocity = Vec2(tileWidth * 2, tileHeight * 2);
        if (xStep == 0) velocity.x = 0;
        if (yStep == 0) velocity.y = 0;

        bool alternate = dict.find("alternate") != dict.end();
        return Platform::create(start, end, velocity, name, scale, alternate);
}

void Platform::initPosition(bool alternate) {
	Size size = this->getContentSize();
	Vec2 center(size.width * getScale() / 2, size.height * getScale() / 2);

	start.add(center);
	end.add(center);

	if (alternate) {
		this->setPosition(end);
	} else {
		this->setPosition(start);
	}
}

void Platform::update(float delta) {
	Vec2 desiredPosition = this->getPosition();
	Vec2 scaledVelocity = this->getVelocity();
	scaledVelocity.scale(delta);

	desiredPosition.add(scaledVelocity);

	desiredPosition.clamp(start, end);

	if (desiredPosition.equals(start) || desiredPosition.equals(end)) {
		this->velocity.scale(-1);
	}
	this->setPosition(desiredPosition);
}
