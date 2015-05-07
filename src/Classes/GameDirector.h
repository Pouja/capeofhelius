#ifndef _GAME_DIRECTOR_H_
#define _GAME_DIRECTOR_H_

#include "cocos2d.h"

class GameDirector {
public:
    static GameDirector* getInstance();
    cocos2d::Director* getCocosDirector();
    void init();
private:
    GameDirector* instance;
    GameDirector() {};
    ~GameDirector();

    cocos2d::Scene* currentScene;
};

#endif
