#include "GameDirector.h"
#include "BasicScene.h"

USING_NS_CC;

GameDirector* s_SharedGameDirector = nullptr;

void GameDirector::init(){
    Director* director = Director::getInstance();

    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    this->currentScene = BasicScene::createScene();

    // run
    director->runWithScene(this->currentScene);
}

GameDirector* GameDirector::getInstance(){
    if (!s_SharedGameDirector)
    {
        s_SharedGameDirector = new (std::nothrow) GameDirector();
        CCASSERT(s_SharedGameDirector, "FATAL: Not enough memory");
        s_SharedGameDirector->init();
    }

    return s_SharedGameDirector;
}

GameDirector::~GameDirector(){

}