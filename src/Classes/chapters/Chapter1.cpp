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
    CCASSERT(false, "unsupported type encountered");
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

bool Chapter1::initPlayers() {
    ChapterManager* chapterManager = ChapterManager::getInstance();
    for (Value value : this->map->getObjectGroup("players")->getObjects()) {
        ValueMap object = value.asValueMap();

        std::string name = object.at("name").asString();
        float x = object.at("x").asFloat();
        float y = object.at("y").asFloat();

        Player* p;

        if (name.compare("zoe-nocape") == 0) {
            ChapterManager::Progress progress = chapterManager->getProgress();

            if (!progress.spawnPoint.equals(Vec2::ZERO)) {
                Vec2 worldCoord = map->mapToWorld(progress.spawnPoint);
                p = Player::create(worldCoord, name);
                this->respawnPoint = worldCoord;
            } else {
                p = Player::create(Vec2(x, y), name);
                this->respawnPoint = Vec2(x, y);
            }
            this->mainPlayer = p;
            name = "main";
        } else {
            p = Player::create(Vec2(x, y), name);
        }

        this->players[name] = p;
    }
    parseDialogs(FileUtils::getInstance()->fullPathForFilename("dialogs/chapter1-dialog.xml"));


    // // DIALOG 2
    // Vec2 rPointd2 = map->mapToWorld(Vec2(11, 33));
    // rPointd2.x -= map->getTileSize().width / 2;
    // rPointd2.y -= map->getTileSize().height / 2;

    // DialogAction* moveZoeDown = new DialogAction(Rect(rPointd2.x, rPointd2.y, width, height), mainPlayer);
    // DialogAction* zoeTalkMartine1 = new DialogAction(std::queue<std::string>({
    //     "Zoë: \"Heeey! Martine, what are you doing here?\"",
    //     "Martine: \"Zooooeeee, long time no see, doing good, you?\""
    // }), hub);
    // DialogAction* moveMartineToZoe = new DialogAction(Rect(rPointd2.x + width, rPointd2.y, width, height), martine);
    // DialogAction* largeTalkZM =  new DialogAction(std::queue<std::string>({
    //     "Zoë: \"Fine. Today is the day!\"",
    //     "Martine: \"You are finally going on your trip? How exciting!\"",
    //     "Zoë: \"I know, my first stop is Helius.\"",
    //     "Martine: \"Helius!? You are going back there? After evertything that happened?\"",
    //     "Zoë: \"Huh, what are you talking about\"",
    //     "Martine: \"Oh you ....... ..... ..... .... ....\"",
    //     "Zoë: \"Sorry what did you say?\"",
    //     "Martine: \"Oh nothing, never mind. If you are going to Helius, you need to be able to defend yourself.\"",
    //     "Zoë: \"Defend myself, why? What is happening in Helius then?\"",
    //     "Martine: \"Just trust me, try to collect enough coin and meet Anouk\"",
    //     "Zoë: \"Huh.. oke\""
    // }), hub);
    // DialogAction* zoeMoveAwayM = new DialogAction(Rect(rPointd2.x + 3 * width, rPointd2.y, width, height), mainPlayer);
    // DialogAction* mFaceZ = new DialogAction(Rect(rPointd2.x + 2 * width, rPointd2.y, width, height), martine);
    // DialogAction* lastTalkMZ = new DialogAction(std::queue<std::string>({
    //     "Martine: \"Oh yeah, in case your forgot watch out for the insects and the water on the way.\"",
    //     "Zoë: \"Oh yeah, thanks!\""
    // }), hub);
    // moveZoeDown->setNext(zoeTalkMartine1)->setNext(moveMartineToZoe)->setNext(largeTalkZM)->setNext(zoeMoveAwayM)
    // ->setNext(mFaceZ)->setNext(lastTalkMZ);
    // this->dialogs["1-2"] = moveZoeDown;

    // //DIALOG 1-3
    // this->dialogs["1-3"] = new DialogAction(std::queue<std::string>({"Anouk: \"Up here Zoë!\""}), hub);

    // //DIALOG 1-4
    // Vec2 rPointd4 = map->mapToWorld(Vec2(83, 33));
    // rPointd4.x -= map->getTileSize().width / 2;
    // rPointd4.y -= map->getTileSize().height / 2;

    // DialogAction* moveZoeAnouk = new DialogAction(Rect(rPointd4.x + width, rPointd4.y, width, height), mainPlayer);
    // DialogAction* zoeScreamAnouk = new DialogAction(std::queue<std::string>({"Zoë: \"Anooooouuuuuk\""}), hub);
    // DialogAction* moveAnoukZoe = new DialogAction(Rect(rPointd4.x, rPointd4.y, width, height), anouk);
    // DialogAction* talk = new DialogAction(std::queue<std::string>({
    //     "Anouk: \"Zooooeeeeee\"",
    //     "Zoë: \"I'm so happy to see you again!\"",
    //     "Anouk: \"Me too, what are you doing here so far away from SluisVoetHelle?\"",
    //     "Zoë: \"Well, I'm finally doing that trip!\"",
    //     "Anouk: \"So you are actually doing that, are you sure? Will you be ok?\"",
    //     "Zoë: \"Uhm.. yeah I guess, Martine also responded like that.\"",
    //     "Anouk: \"Yeah.. that.. so Martine said the same thing. Did she tell you anything by the way?\"",
    //     "Zoë: \"Yeah, she told me.\"",
    //     "Anouk: \"I'm so sorry, sorry that we could not help you. It must be tough missing him.\"",
    //     "Zoë: \"Uhm what are you talking about? Martine only told me about the water and the insects.\"",
    //     "Anouk: \"Oh, nevermind then.\"",
    //     "Zoë: \"Who should I be missing?\"",
    //     "Anouk: \"Nevermind Zoë, if you really want to head to Helius. You should buy one of our swords.\"",
    //     "Zoë: \"Yeah Martine, asked me to collect coins, I hope that I have enough. Here you!\""
    // }), hub);
    // DialogAction* clearCoins = new DialogAction("coins", mainPlayer, hub);
    // DialogAction* lastTalkZA =  new DialogAction(std::queue<std::string>({
    //     "",
    //     "Anouk: \"Wonderfull, here is your sword, you know how to use it right?\"",
    //     "Zoë: \"I'm one strong woman, I will be fine.\"",
    //     "Anouk: \"Good luck\""
    // }), hub);

    // moveZoeAnouk->setNext(zoeScreamAnouk)->setNext(moveAnoukZoe)->setNext(talk)->setNext(clearCoins)->setNext(lastTalkZA);
    // dialogs["1-4"] = moveZoeAnouk;

    // //DIALOG 1-5-C
    // Vec2 rPointd5 = map->mapToWorld(Vec2(97, 40));
    // rPointd5.x -= map->getTileSize().width / 2;
    // rPointd5.y -= map->getTileSize().height / 2;
    // DialogAction* talkToAnouk = new DialogAction(std::queue<std::string>({
    //     "I better talk to Anouk first before I go."
    // }), hub);
    // DialogAction* moveBack = new DialogAction(Rect(rPointd5.x - 3 * width, rPointd5.y, width, height), mainPlayer);
    // talkToAnouk->setNext(moveBack);
    // dialogs["1-5-c"] = talkToAnouk;

    // //Dialog 1-5-c-1
    // DialogAction* zoeFinalWords = new DialogAction(std::queue<std::string>({
    //     "Zoë: \"That was one long day. I will rest here I guess and tomorrow I'll be in Helius.\"",
    //     "Zoë: \"But what were Anouk and Martine talking about, who should I be missing?\"",
    //     "Zoë: \"And what is so dangerous in Helius?\""
    // }), hub);
    // DialogAction* moveZoeDoor = new DialogAction(Rect(rPointd5.x - 0.5 * width,  rPointd5.y, width, height), mainPlayer);
    // DialogAction* fadeZoe = new DialogAction(FadeOut::create(1.0f), mainPlayer);
    // DialogAction* done = new DialogAction("finished", mainPlayer, hub);

    // DialogAction* hideA = new DialogAction(FadeOut::create(0), anouk);
    // DialogAction* hideM = new DialogAction(FadeOut::create(0), martine);
    // DialogAction* moveA = new DialogAction(Place::create(Vec2(rPointd5.x - 0.5 * width,  rPointd5.y)), anouk);
    // DialogAction* moveM = new DialogAction(Place::create(Vec2(rPointd5.x,  rPointd5.y)), martine);
    // DialogAction* appearA = new DialogAction(FadeIn::create(0.5f), anouk);
    // DialogAction* appearM = new DialogAction(FadeIn::create(0.5f), martine);
    // DialogAction* moveA1 = new DialogAction(Rect(rPointd5.x + 1, rPointd5.y, width, height), martine);
    // DialogAction* talkAM = new DialogAction(std::queue<std::string>({
    //     "Martine: \"It seems that deep in her hearth she can't let go of Helius.\"",
    //     "Anouk: \"No wonder, after everything that happened.\"",
    //     "Martine: \"I just hope that she will leave Helius and that she does not meet her.\"",
    //     "Anouk: \"Who? Oh her! That bitch.\""
    // }), hub);
    // moveZoeDoor->setNext(zoeFinalWords)->setNext(fadeZoe)->setNext(hideA)->setNext(hideM)->setNext(moveA)->setNext(moveM)
    // ->setNext(appearA)->setNext(appearM)->setNext(moveA1)->setNext(talkAM)->setNext(done);
    // dialogs["1-5-c-1"] = moveZoeDoor;
    return true;
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
