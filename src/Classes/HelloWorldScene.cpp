#include "HelloWorldScene.h"
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    std::string file = "tilemap.tmx";
    auto str = String::createWithContentsOfFile(FileUtils::getInstance()->fullPathForFilename(file.c_str()).c_str());
    this->tilemap = TMXTiledMap::createWithXML(str->getCString(),"");
    this->tilemap->getLayer("definitions")->setVisible(false);
    this->background = this->tilemap->getLayer("bg");
    this->tilemap->setScale(2.0);
    addChild(this->tilemap, -1);
    return true;
}
