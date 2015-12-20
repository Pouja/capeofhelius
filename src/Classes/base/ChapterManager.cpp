#include "ChapterManager.h"
#include "cocos2d.h"
#include "../chapters/Chapter0.h"
#include "../chapters/Chapter1.h"
#include "../chapters/Chapter2.h"
#include "../chapters/ChapterPre1.h"
// #include "tinyxml2.h"

static ChapterManager* s_ChapterManager = nullptr;

USING_NS_CC;

ChapterManager* ChapterManager::getInstance() {
    if (!s_ChapterManager) {
        s_ChapterManager = new (std::nothrow) ChapterManager();
        CCASSERT(s_ChapterManager, "FATAL: Not enough memory");
    }
    return s_ChapterManager;
}

ChapterManager::Chapter ChapterManager::getCurrent() {
    return this->current;
}

void ChapterManager::init() {
    // TODO: read/write save file
    // tinyxml2::XMLDocument doc;
    // tinyxml2::XMLNode* root = doc.NewElement("root");
    // doc.InsertFirstChild(root);
    // doc.SaveFile( "madeByHand.xml" );

    Director::getInstance()->runWithScene(Chapter0::createScene());
    this->current = CHAPTER_0;
}

void ChapterManager::next() {
    CCAssert(this->current != END_CHAPTER, "Can not call ChapterManager::next when current is END_CHAPTER");
    switch (this->current) {
    case CHAPTER_0:
        Director::getInstance()->replaceScene(TransitionFade::create(2.0f, ChapterPre1::createScene()));
        this->current = PRE_CHAPTER_1;
        break;
    case PRE_CHAPTER_1:
        Director::getInstance()->replaceScene(Chapter1::createScene());
        this->current = CHAPTER_1;
        break;
    case CHAPTER_1:
        Director::getInstance()->replaceScene(TransitionFade::create(2.0f, Chapter2::createScene()));
        this->current = CHAPTER_2;
        break;
    case CHAPTER_2:
        Director::getInstance()->end();
        this->current = END_CHAPTER;
        break;
    }
}

void ChapterManager::setChapter(ChapterManager::Chapter next) {
    this->current = next;
    switch (this->current) {
    case CHAPTER_0:
        Director::getInstance()->replaceScene(TransitionFade::create(2.0f, Chapter0::createScene()));
        break;
    case PRE_CHAPTER_1:
        Director::getInstance()->replaceScene(TransitionFade::create(2.0f, ChapterPre1::createScene()));
        break;
    case CHAPTER_1:
        Director::getInstance()->replaceScene(Chapter1::createScene());
        break;
    case CHAPTER_2:
        Director::getInstance()->replaceScene(TransitionFade::create(2.0f, Chapter2::createScene()));
        break;
    case END_CHAPTER:
        Director::getInstance()->end();
        break;
    }
}
