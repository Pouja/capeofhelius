#include "Chapter2.h"
#include "base/GameHub.h"
#include "utils/TitleScreen.h"
#include "base/ChapterManager.h"

USING_NS_CC;

Scene* Chapter2::createScene() {
    Scene* scene = Scene::create();
    Layer* layer = Chapter2::create();
    scene->addChild(layer);
    return scene;
}

bool Chapter2::init() {
    TitleScreen* titleScreen = TitleScreen::create("End chapter 1:\"I'm off to go!\"", "", false);
    addChild(titleScreen);

    std::vector<std::string> text = {
        "To be continued..."
    };
    this->index = 0;

    CallFunc* onFinish = CallFunc::create([] {
        Director::getInstance()->end();
    });

    CallFunc* onTick = CallFunc::create([text, this, titleScreen] {
        titleScreen->setText(text[this->index], "");
        this->index++;
    });

    this->runAction(
        Sequence::create(
            Repeat::create(
                Sequence::create(DelayTime::create(7.0f), onTick, nullptr), text.size()
            ), DelayTime::create(5.0f), onFinish, nullptr));

    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = [](EventKeyboard::KeyCode keyCode, Event * event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
            ChapterManager::getInstance()->save();
            Director::getInstance()->end();
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}
