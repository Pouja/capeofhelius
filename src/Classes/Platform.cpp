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