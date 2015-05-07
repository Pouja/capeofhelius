#ifndef _I_KEY_LISTENER_H_
#define _I_KEY_LISTENER_H_

#include "cocos2d.h"

class IKeyListener {
public:
    virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) = 0;
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {};
    virtual bool isKeyTransparent() = 0;
};

#endif