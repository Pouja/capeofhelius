#ifndef _CHAPTER_1_H_
#define _CHAPTER_1_H_

#include "base/BasicScene.h"

//TODO: rework this class completely.
class Chapter1 : public BasicScene {
public:
    static cocos2d::Scene* createScene();
    static Chapter1* create();
    virtual bool initVariables();
    virtual bool initPlayers();
    void onGameOver();
    virtual void onFinish();
    virtual bool dialogCondition(const std::string& id);
};

#endif
