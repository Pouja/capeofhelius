#ifndef _CHAPTER_1_H_
#define _CHAPTER_1_H_

#include "base/BasicScene.h"

class Chapter1 : public BasicScene {
public:
    static cocos2d::Scene* createScene();
    static Chapter1* create();
    virtual bool initVariables();
    void onGameOver();
};

#endif
