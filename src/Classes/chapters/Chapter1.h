#ifndef _CHAPTER_1_H_
#define _CHAPTER_1_H_

#include "base/BasicScene.h"
#include "tinyxml2.h"

//TODO: rework this class completely.
class Chapter1 : public BasicScene {
public:
    static cocos2d::Scene* createScene();
    static Chapter1* create();
    virtual bool initVariables();
    void onGameOver();
    virtual void onFinish();
    virtual bool dialogCondition(const std::string& id);
    DialogAction* parseMove(tinyxml2::XMLElement* xmlMove);
    DialogAction* parseText(tinyxml2::XMLElement* xmlMove);
    DialogAction* parseDialogAction(tinyxml2::XMLElement* xmlDialog);
    void parseDialogs(std::string filename);
    DialogAction* parseDialog(tinyxml2::XMLElement* xmlDialog);
};

#endif
