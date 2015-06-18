#ifndef _CHAPTER_PRE_1_H_
#define _CHAPTER_PRE_1_H_

#include "cocos2d.h"

class ChapterPre1 : public cocos2d::Layer {
private:
    int index;
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(ChapterPre1);
};

#endif
