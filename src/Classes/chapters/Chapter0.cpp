#include "Chapter0.h"
#include "base/GameHub.h"
#include "ChapterPre1.h"

USING_NS_CC;

Scene* Chapter0::createScene() {
    Scene* scene = Scene::create();
    Layer* layer = Chapter0::create();
    scene->addChild(layer);
    return scene;
}

bool Chapter0::init() {
    GameHub* hub = GameHub::create();
    hub->toggleHud();
    addChild(hub);

    std::function<void()> onFinish = ([] {
        Director::getInstance()->replaceScene(TransitionFade::create(2.0f, ChapterPre1::createScene()));
    });

    hub->setText(std::queue<std::string>({
        "Unknown: \"zzzZZZzzZZZ zZZZZ\"",
        "Mom: \"Are you already up? Wake up!\"",
        "Unknown: \"Mh... just 5 more minutes...\" *Falls asleep*",
        "Mom: \"Zoe! It's gettting late! Wake up now!\"",
        "Zoe: \"Allright, allright, I'm comming\"",
    }), onFinish);

    return true;
}
