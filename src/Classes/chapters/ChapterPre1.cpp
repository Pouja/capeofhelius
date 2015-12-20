#include "ChapterPre1.h"
#include "base/GameHub.h"
#include "utils/TitleScreen.h"
#include "../base/ChapterManager.h"

USING_NS_CC;

Scene* ChapterPre1::createScene() {
    Scene* scene = Scene::create();
    Layer* layer = ChapterPre1::create();
    scene->addChild(layer);
    return scene;
}

bool ChapterPre1::init() {
    TitleScreen* titleScreen = TitleScreen::create("By the makers of your origami:", "", false);
    addChild(titleScreen);

    std::vector<std::string> text = {
        "Your very own croissant:",
        "Pawya Inc. proudly presents:",
        "CAPE OF HELIUS",
        "Happy birthday!\nEnjoy :)",
        "So like every story, this one also has a main character: Zoë.",
        "A strong, beautifull, but yet a young woman set to be in an adventure she never expected.",
        "Her back story is like every other girl you meet.",
        "She cries, she learns, she gets angry, she smiles and foremost she always keeps her head up, an ability she will need dearly.",
        "Chapter 1: \"I'm off to go!\""
    };
    this->index = 0;

    CallFunc* onFinish = CallFunc::create([] {
        ChapterManager::getInstance()->next();
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

    return true;
}
