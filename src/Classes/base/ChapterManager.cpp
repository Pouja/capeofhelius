#include "ChapterManager.h"
#include "../chapters/Chapter0.h"
#include "../chapters/Chapter1.h"
#include "../chapters/Chapter2.h"
#include "../chapters/ChapterPre1.h"

#define CONFIG "config.xml"

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
    return this->progress.chapter;
}

ChapterManager::Progress ChapterManager::getProgress() {
    return this->progress;
}

void ChapterManager::save() {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* root = doc.NewElement("config");
    root->InsertEndChild(this->toXML(&doc, this->progress));
    doc.InsertFirstChild(root);
    FileUtils* fileUtil = FileUtils::getInstance();
    doc.SaveFile(fileUtil->fullPathForFilename(CONFIG).c_str());
}

tinyxml2::XMLElement* ChapterManager::toXML(tinyxml2::XMLDocument* doc,
        ChapterManager::Progress progress) {
    tinyxml2::XMLElement* xmlProgress = doc->NewElement("progress");
    xmlProgress->SetAttribute("chapter", progress.chapter);

    tinyxml2::XMLElement* xmlSpawn = doc->NewElement("spawnpoint");
    xmlSpawn->SetAttribute("x", progress.spawnPoint.x);
    xmlSpawn->SetAttribute("y", progress.spawnPoint.y);

    xmlProgress->InsertEndChild(xmlSpawn);

    return xmlProgress;
}

ChapterManager::Progress ChapterManager::parseXML(tinyxml2::XMLElement* element) {
    ChapterManager::Progress progress;
    int x, y, chapter = 0;
    element->QueryIntAttribute("chapter", &(chapter));
    tinyxml2::XMLElement* spawnpoint = element->FirstChildElement("spawnpoint");
    progress.chapter = (ChapterManager::Chapter) chapter;
    if (spawnpoint != NULL) {
        spawnpoint->QueryIntAttribute("x", &(x));
        spawnpoint->QueryIntAttribute("y", &(y));
        progress.spawnPoint = Vec2(x, y);
    }
    return progress;
}

void ChapterManager::init() {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(FileUtils::getInstance()->fullPathForFilename(CONFIG).c_str());

    tinyxml2::XMLElement* config = doc.FirstChildElement("config");
    assert(config != NULL);

    this->progress = this->parseXML(config->FirstChildElement("progress"));
    this->startChapter(this->progress.chapter);
}

void ChapterManager::next() {
    CCAssert(this->progress.chapter != END_CHAPTER, "Can not call ChapterManager::next when current is END_CHAPTER");
    switch (this->progress.chapter) {
    case CHAPTER_0:
        Director::getInstance()->replaceScene(TransitionFade::create(2.0f, ChapterPre1::createScene()));
        this->progress.chapter = PRE_CHAPTER_1;
        break;
    case PRE_CHAPTER_1:
        Director::getInstance()->replaceScene(Chapter1::createScene());
        this->progress.chapter = CHAPTER_1;
        break;
    case CHAPTER_1:
        Director::getInstance()->replaceScene(TransitionFade::create(2.0f, Chapter2::createScene()));
        this->progress.chapter = CHAPTER_2;
        break;
    case CHAPTER_2:
        Director::getInstance()->end();
        this->progress.chapter = END_CHAPTER;
        break;
    }
}

void ChapterManager::startChapter(ChapterManager::Chapter chapter) {
    switch (chapter) {
    case CHAPTER_0:
        Director::getInstance()->runWithScene(Chapter0::createScene());
        break;
    case PRE_CHAPTER_1:
        Director::getInstance()->runWithScene(ChapterPre1::createScene());
        break;
    case CHAPTER_1:
        Director::getInstance()->runWithScene(Chapter1::createScene());
        break;
    case CHAPTER_2:
        Director::getInstance()->runWithScene(Chapter2::createScene());
        break;
    case END_CHAPTER:
        Director::getInstance()->end();
        break;
    }
}
