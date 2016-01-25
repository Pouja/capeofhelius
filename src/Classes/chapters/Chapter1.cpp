#include "Chapter1.h"
#include "../base/ChapterManager.h"
USING_NS_CC;

Scene* Chapter1::createScene()
{
    auto scene = Scene::create();
    auto layer = Chapter1::create();
    scene->addChild(layer);
    return scene;
}

Chapter1* Chapter1::create() {
    Chapter1* chapter = new Chapter1();
    if (chapter && chapter->initVariables() && chapter->init()) {
        chapter->parseDialogs(FileUtils::getInstance()->fullPathForFilename("dialogs/chapter1-dialog.xml"));
        chapter->autorelease();
        return chapter;
    }
    log("failed to create chapter1");
    CC_SAFE_DELETE(chapter);
    return nullptr;
}

bool Chapter1::initVariables() {
    this->mapName = "chapter1.tmx";
    this->bgLocation = "backgrounds/chapter1-bg.png";

    return true;
}

void Chapter1::parseDialogs(std::string filename) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError error = doc.LoadFile(filename.c_str());

    assert(error == tinyxml2::XMLError::XML_SUCCESS);

    tinyxml2::XMLElement* root = doc.RootElement();
    tinyxml2::XMLElement* xmlDialog = root->FirstChildElement();
    while (xmlDialog != NULL) {
        this->parseDialog(xmlDialog);
        xmlDialog = xmlDialog->NextSiblingElement();
    }
}

DialogAction* Chapter1::parseMove(tinyxml2::XMLElement* xmlMove) {
    std::string character = xmlMove->Attribute("character");
    assert(!character.empty());
    auto pair = this->players.find(character);
    assert(pair != this->players.end());

    Player* player = pair->second;

    float width = this->map->getTileSize().width;
    float height = this->map->getTileSize().height;

    float x, y = 0;
    xmlMove->QueryFloatAttribute("x", &(x));
    xmlMove->QueryFloatAttribute("y", &(y));

    return new DialogAction(Rect(x, y, width, height), player);
}

DialogAction* Chapter1::parseText(tinyxml2::XMLElement* xmlText) {
    std::queue<std::string> dialog;
    tinyxml2::XMLElement* xmlLine = xmlText->FirstChildElement();
    while (xmlLine != NULL) {
        dialog.push(xmlLine->GetText());
        xmlLine = xmlLine->NextSiblingElement();
    }
    return new DialogAction(dialog, this->hub);
}

DialogAction* Chapter1::parseDialogAction(tinyxml2::XMLElement* xmlDialog) {
    std::string type = xmlDialog->Name();

    if (type.compare("move") == 0) {
        return parseMove(xmlDialog);
    } else if (type.compare("text") == 0) {
        return parseText(xmlDialog);
    }

    log("Unsupported type '%s' encountered in a dialog file.", type.c_str());
    assert(false);
    return NULL;
}

DialogAction* Chapter1::parseDialog(tinyxml2::XMLElement* xmlDialog) {
    std::string id = xmlDialog->Attribute("id");
    tinyxml2::XMLElement* xmlDialogAction = xmlDialog->FirstChildElement();

    DialogAction* first = parseDialogAction(xmlDialogAction);
    xmlDialogAction = xmlDialogAction->NextSiblingElement();

    DialogAction* current = first;

    while (xmlDialogAction != NULL) {
        current = current->setNext(parseDialogAction(xmlDialogAction));
        xmlDialogAction = xmlDialogAction->NextSiblingElement();

    }
    this->dialogs[id] = first;
    return NULL;
}

bool Chapter1::dialogCondition(const std::string& id) {
    if (id.compare("1-5-c") == 0) {
        auto result = std::find_if(std::begin(this->dialogRects), std::end(this->dialogRects), [this](std::pair<std::string, Rect> dialogRect) {
            return std::get<0>(dialogRect).compare("1-4") == 0;
        });
        return result == std::end(this->dialogRects);
    }
    return false;
}

void Chapter1::onFinish() {
    ChapterManager::getInstance()->next();
}

void Chapter1::onGameOver() {
    Director::getInstance()->replaceScene(Chapter1::createScene());
}
