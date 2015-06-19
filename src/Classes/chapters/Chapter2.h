#ifndef _CHAPTER_2_H_
#define _CHAPTER_2_H_

#include "cocos2d.h"

class Chapter2 : public cocos2d::Layer {
private:
    int index;
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(Chapter2);
};

#endif
