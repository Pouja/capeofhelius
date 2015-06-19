#include "Chapter1.h"
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
    CC_SAFE_DELETE(chapter);
    return nullptr;
}

bool Chapter1::initVariables() {
    this->mapName = "chapter1.tmx";
    this->bgLocation = "backgrounds/chapter1-bg.png";

    return true;
}

bool Chapter1::initPlayers() {
    for (Value value : this->map->getObjectGroup("players")->getObjects()) {
        ValueMap object = value.asValueMap();

        std::string name = object.at("name").asString();
        float x = object.at("x").asFloat();
        float y = object.at("y").asFloat();

        Player* p = Player::create(Vec2(x, y), name);
        this->players.push_back(p);

        if (name.compare("zoe-nocape") == 0) {
            this->mainPlayer = p;
            this->respawnPoint = Vec2(x, y);
        }
    }
    std::function<void()> cb = [this] {
        this->activeDialog = nullptr;
    };

    Vec2 rPointd1 = map->mapToWorld(Vec2(5, 4));
    rPointd1.x -= map->getTileSize().width / 2;
    rPointd1.y -= map->getTileSize().height / 2;
    float width = map->getTileSize().width;
    float height = map->getTileSize().height;

    Player* mother = this->players[2];
    Player* father = this->players[1];
    Player* anouk = this->players[4];
    Player* martine = this->players[3];

    DialogAction* moveMom1 = new DialogAction(Rect(rPointd1.x, rPointd1.y, width, height), mother);
    DialogAction* speakMom1 = new DialogAction(std::queue<std::string>({
        "Mom: \"Are you really sure you want to go?\""
    }), hub);
    DialogAction* zoeFaceMom = new DialogAction(Rect(rPointd1.x + 4 * width, rPointd1.y, width, height), mainPlayer);
    DialogAction* speakZoeFather = new DialogAction(std::queue<std::string>({
        "Zoe: \"Yes, it is time to explore the world, I have to leave and try to live on my own.\"",
        "Father: \"Ah you will be allright.\""
    }), hub);;
    DialogAction* momFaceDad = new DialogAction(Rect(rPointd1.x - 10, rPointd1.y, width, height), mother);
    DialogAction* zoeMomTalk = new DialogAction(std::queue<std::string>({
        "Mom: \"How can you just say that so simple, she is our daughter!\"",
        "Zoe: \"Mom I will be fine, I'll write once in a while.\""
    }), hub);
    DialogAction* momFaceZoe = new DialogAction(Rect(rPointd1.x + 10,  rPointd1.y, width, height), mother);
    DialogAction* dadFaceZoe = new DialogAction(Rect(rPointd1.x - 10,  rPointd1.y, width, height), father);
    DialogAction* talkMomZoeDad = new DialogAction(std::queue<std::string>({
        "Mom: \"You better write everyday!\"",
        "Father: \"Don't be so strict, she will be fine.\"",
        "Mom: \"You are always so lose. I'll miss you Zoe.\"",
        "Zoe: \"I'm going, I will miss you both and ofcourse Skittle, take good care of him.\"",
        "Father: \"Oh do not worry about that!\"",
        "Zoe: \"Specially you father, do not be mean to him, like always. Good bye.\""
    }), hub);
    DialogAction* zoeWalksBorder = new DialogAction(Rect(rPointd1.x + 14 * width, rPointd1.y, width, height), mainPlayer);
    DialogAction* momDadTalk = new DialogAction(std::queue<std::string>({
        "Father and mom: \"We love you!\"",
        "Zoe: \"I love you too.\""
    }), hub);
    DialogAction* jumpOff = new DialogAction(Rect(rPointd1.x + 16 * width, rPointd1.y, width, height), mainPlayer);

    moveMom1->setNext(speakMom1)->setNext(zoeFaceMom)->setNext(speakZoeFather)->setNext(momFaceDad)
    ->setNext(zoeMomTalk)->setNext(momFaceZoe)->setNext(dadFaceZoe)->setNext(talkMomZoeDad)->setNext(zoeWalksBorder)
    ->setNext(momDadTalk)->setNext(jumpOff);
    dialogs["1-1"] = moveMom1;

    // DIALOG 2
    Vec2 rPointd2 = map->mapToWorld(Vec2(11, 33));
    rPointd2.x -= map->getTileSize().width / 2;
    rPointd2.y -= map->getTileSize().height / 2;

    DialogAction* moveZoeDown = new DialogAction(Rect(rPointd2.x, rPointd2.y, width, height), mainPlayer);
    DialogAction* zoeTalkMartine1 = new DialogAction(std::queue<std::string>({
        "Zoe: \"Heeey! Martine, what are you doing here?\"",
        "Martine: \"Zooooeeee, long time no see, doing good, you?\""
    }),hub);
    DialogAction* moveMartineToZoe = new DialogAction(Rect(rPointd2.x + width, rPointd2.y, width, height), martine);
    DialogAction* largeTalkZM =  new DialogAction(std::queue<std::string>({
        "Zoe: \"Fine. Today is the day!\"",
        "Martine: \"You are finally going on your trip? How exciting!\"",
        "Zoe: \"I know, my first stop is Helius.\"",
        "Martine: \"Helius!? You are going back there? After evertything that happened?\"",
        "Zoe: \"Huh, what are you talking about\"",
        "Martine: \"Oh you ....... ..... ..... .... ....\"",
        "Zoe: \"Sorry what did you say?\"",
        "Martine: \"Oh nothing, never mind. If you are going to Helius, you need to be able to defend yourself.\"",
        "Zoe: \"Defend myself, why? What is happening in Helius then?\"",
        "Martine: \"Just trust me, try to collect enough coin and meet Anouk\"",
        "Zoe: \"Huh.. oke\""
    }), hub);
    DialogAction* zoeMoveAwayM = new DialogAction(Rect(rPointd2.x + 3 * width, rPointd2.y, width, height), mainPlayer);
    DialogAction* mFaceZ = new DialogAction(Rect(rPointd2.x + 2 * width, rPointd2.y, width, height), martine);
    DialogAction* lastTalkMZ = new DialogAction(std::queue<std::string>({
        "Martine: \"Oh yeah, in case your forgot watch out for the insects and the water on the way.\"",
        "Zoe: \"Oh yeah, thanks!\""}), hub);
    moveZoeDown->setNext(zoeTalkMartine1)->setNext(moveMartineToZoe)->setNext(largeTalkZM)->setNext(zoeMoveAwayM)
    ->setNext(mFaceZ)->setNext(lastTalkMZ);
    this->dialogs["1-2"] = moveZoeDown;
    return true;
}

void Chapter1::onGameOver() {
    Director::getInstance()->replaceScene(Chapter1::createScene());
}
