#ifndef _TITLE_SCREEN_H_
#define _TITLE_SCREEN_H_

#include "cocos2d.h"

class TitleScreen : public cocos2d::Layer {
private:
	cocos2d::Label* title;
	cocos2d::Label* message;
	TitleScreen(){};

public:
	static TitleScreen* create(const std::string& title, const std::string& message, bool withDim);
    bool init(const std::string& title, const std::string& message, bool withDim);
	void setText(const std::string& title, const std::string& message);
};

#endif
