#ifndef _CHAPTER_0_H_
#define _CHAPTER_0_H_

#include "cocos2d.h"

class Chapter0 : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(Chapter0);
};

#endif
