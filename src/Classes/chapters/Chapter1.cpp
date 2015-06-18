#include "Chapter1.h"
#include "base/GameHub.h"

USING_NS_CC;

Scene* Chapter1::createScene() {
    Scene* scene = Scene::create();
    Layer* layer = Chapter1::create();
    scene->addChild(layer);
    return scene;
}

bool Chapter1::init() {
    return true;
}
