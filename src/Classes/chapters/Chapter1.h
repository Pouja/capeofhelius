#ifndef _CHAPTER_1_H_
#define _CHAPTER_1_H_

#include "cocos2d.h"

class Chapter1 : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(Chapter1);
};

#endif
