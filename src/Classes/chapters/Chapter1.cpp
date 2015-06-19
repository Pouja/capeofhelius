#include "Chapter1.h"
#include "Chapter2.h"
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
        "Zoë: \"Yes, it is time to explore the world, I have to leave and try to live on my own.\"",
        "Father: \"Ah you will be allright.\""
    }), hub);;
    DialogAction* momFaceDad = new DialogAction(Rect(rPointd1.x - 10, rPointd1.y, width, height), mother);
    DialogAction* zoeMomTalk = new DialogAction(std::queue<std::string>({
        "Mom: \"How can you just say that so simple, she is our daughter!\"",
        "Zoë: \"Mom I will be fine, I'll write once in a while.\""
    }), hub);
    DialogAction* momFaceZoe = new DialogAction(Rect(rPointd1.x + 10,  rPointd1.y, width, height), mother);
    DialogAction* dadFaceZoe = new DialogAction(Rect(rPointd1.x - 10,  rPointd1.y, width, height), father);
    DialogAction* talkMomZoeDad = new DialogAction(std::queue<std::string>({
        "Mom: \"You better write everyday!\"",
        "Father: \"Don't be so strict, she will be fine.\"",
        "Mom: \"You are always so lose. I'll miss you Zoë.\"",
        "Zoë: \"I'm going, I will miss you both and ofcourse Skittle, take good care of him.\"",
        "Father: \"Oh do not worry about that!\"",
        "Zoë: \"Specially you father, do not be mean to him, like always. Good bye.\""
    }), hub);
    DialogAction* zoeWalksBorder = new DialogAction(Rect(rPointd1.x + 14 * width, rPointd1.y, width, height), mainPlayer);
    DialogAction* momDadTalk = new DialogAction(std::queue<std::string>({
        "Father and mom: \"We love you!\"",
        "Zoë: \"I love you too.\""
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
        "Zoë: \"Heeey! Martine, what are you doing here?\"",
        "Martine: \"Zooooeeee, long time no see, doing good, you?\""
    }), hub);
    DialogAction* moveMartineToZoe = new DialogAction(Rect(rPointd2.x + width, rPointd2.y, width, height), martine);
    DialogAction* largeTalkZM =  new DialogAction(std::queue<std::string>({
        "Zoë: \"Fine. Today is the day!\"",
        "Martine: \"You are finally going on your trip? How exciting!\"",
        "Zoë: \"I know, my first stop is Helius.\"",
        "Martine: \"Helius!? You are going back there? After evertything that happened?\"",
        "Zoë: \"Huh, what are you talking about\"",
        "Martine: \"Oh you ....... ..... ..... .... ....\"",
        "Zoë: \"Sorry what did you say?\"",
        "Martine: \"Oh nothing, never mind. If you are going to Helius, you need to be able to defend yourself.\"",
        "Zoë: \"Defend myself, why? What is happening in Helius then?\"",
        "Martine: \"Just trust me, try to collect enough coin and meet Anouk\"",
        "Zoë: \"Huh.. oke\""
    }), hub);
    DialogAction* zoeMoveAwayM = new DialogAction(Rect(rPointd2.x + 3 * width, rPointd2.y, width, height), mainPlayer);
    DialogAction* mFaceZ = new DialogAction(Rect(rPointd2.x + 2 * width, rPointd2.y, width, height), martine);
    DialogAction* lastTalkMZ = new DialogAction(std::queue<std::string>({
        "Martine: \"Oh yeah, in case your forgot watch out for the insects and the water on the way.\"",
        "Zoë: \"Oh yeah, thanks!\""
    }), hub);
    moveZoeDown->setNext(zoeTalkMartine1)->setNext(moveMartineToZoe)->setNext(largeTalkZM)->setNext(zoeMoveAwayM)
    ->setNext(mFaceZ)->setNext(lastTalkMZ);
    this->dialogs["1-2"] = moveZoeDown;

    //DIALOG 1-3
    this->dialogs["1-3"] = new DialogAction(std::queue<std::string>({"Anouk: \"Up here Zoë!\""}), hub);

    //DIALOG 1-4
    Vec2 rPointd4 = map->mapToWorld(Vec2(83, 33));
    rPointd4.x -= map->getTileSize().width / 2;
    rPointd4.y -= map->getTileSize().height / 2;

    DialogAction* moveZoeAnouk = new DialogAction(Rect(rPointd4.x + width, rPointd4.y, width, height), mainPlayer);
    DialogAction* zoeScreamAnouk = new DialogAction(std::queue<std::string>({"Zoë: \"Anooooouuuuuk\""}), hub);
    DialogAction* moveAnoukZoe = new DialogAction(Rect(rPointd4.x, rPointd4.y, width, height), anouk);
    DialogAction* talk = new DialogAction(std::queue<std::string>({
        "Anouk: \"Zooooeeeeee\"",
        "Zoë: \"I'm so happy to see you again!\"",
        "Anouk: \"Me too, what are you doing here so far away from SluisVoetHelle?\"",
        "Zoë: \"Well, I'm finally doing that trip!\"",
        "Anouk: \"So you are actually doing that, are you sure? Will you be ok?\"",
        "Zoë: \"Uhm.. yeah I guess, Martine also responded like that.\"",
        "Anouk: \"Yeah.. that.. so Martine said the same thing. Did she tell you anything by the way?\"",
        "Zoë: \"Yeah, she told me.\"",
        "Anouk: \"I'm so sorry, sorry that we could not help you. It must be tough missing him.\"",
        "Zoë: \"Uhm what are you talking about? Martine only told me about the water and the insects.\"",
        "Anouk: \"Oh, nevermind then.\"",
        "Zoë: \"Who should I be missing?\"",
        "Anouk: \"Nevermind Zoë, if you really want to head to Helius. You should buy one of our swords.\"",
        "Zoë: \"Yeah Martine, asked me to collect coins, I hope that I have enough. Here you!\""
    }), hub);
    DialogAction* clearCoins = new DialogAction("coins", mainPlayer, hub);
    DialogAction* lastTalkZA =  new DialogAction(std::queue<std::string>({
        "",
        "Anouk: \"Wonderfull, here is your sword, you know how to use it right?\"",
        "Zoë: \"I'm one strong woman, I will be fine.\"",
        "Anouk: \"Good luck\""
    }), hub);

    moveZoeAnouk->setNext(zoeScreamAnouk)->setNext(moveAnoukZoe)->setNext(talk)->setNext(clearCoins)->setNext(lastTalkZA);
    dialogs["1-4"] = moveZoeAnouk;

    //DIALOG 1-5-C
    Vec2 rPointd5 = map->mapToWorld(Vec2(97, 40));
    rPointd5.x -= map->getTileSize().width / 2;
    rPointd5.y -= map->getTileSize().height / 2;
    DialogAction* talkToAnouk = new DialogAction(std::queue<std::string>({
        "I better talk to Anouk first before I go."
    }), hub);
    DialogAction* moveBack = new DialogAction(Rect(rPointd5.x - 3 * width, rPointd5.y, width, height), mainPlayer);
    talkToAnouk->setNext(moveBack);
    dialogs["1-5-c"] = talkToAnouk;

    //Dialog 1-5-c-1
    DialogAction* zoeFinalWords = new DialogAction(std::queue<std::string>({
        "Zoë: \"That was one long day. I will rest here I guess and tomorrow I'll be in Helius.\"",
        "Zoë: \"But what were Anouk and Martine talking about, who should I be missing?\"",
        "Zoë: \"And what is so dangerous in Helius?\""
    }), hub);
    DialogAction* moveZoeDoor = new DialogAction(Rect(rPointd5.x - 0.5 * width,  rPointd5.y, width, height), mainPlayer);
    DialogAction* fadeZoe = new DialogAction(FadeOut::create(1.0f), mainPlayer);
    DialogAction* done = new DialogAction("finished", mainPlayer, hub);

    DialogAction* hideA = new DialogAction(FadeOut::create(0), anouk);
    DialogAction* hideM = new DialogAction(FadeOut::create(0), martine);
    DialogAction* moveA = new DialogAction(Place::create(Vec2(rPointd5.x - 0.5 * width,  rPointd5.y)), anouk);
    DialogAction* moveM = new DialogAction(Place::create(Vec2(rPointd5.x,  rPointd5.y)), martine);
    DialogAction* appearA = new DialogAction(FadeIn::create(0.5f), anouk);
    DialogAction* appearM = new DialogAction(FadeIn::create(0.5f), martine);
    DialogAction* moveA1 = new DialogAction(Rect(rPointd5.x + 1, rPointd5.y, width, height), martine);
    DialogAction* talkAM = new DialogAction(std::queue<std::string>({
        "Martine: \"It seems that deep in her hearth she can't let go of Helius.\"",
        "Anouk: \"No wonder, after everything that happened.\"",
        "Martine: \"I just hope that she will leave Helius and that she does not meet her.\"",
        "Anouk: \"Who? Oh her! That bitch.\""
    }), hub);
    moveZoeDoor->setNext(zoeFinalWords)->setNext(fadeZoe)->setNext(hideA)->setNext(hideM)->setNext(moveA)->setNext(moveM)
    ->setNext(appearA)->setNext(appearM)->setNext(moveA1)->setNext(talkAM)->setNext(done);
    dialogs["1-5-c-1"] = moveZoeDoor;
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
        Director::getInstance()->replaceScene(TransitionFade::create(2.0f, Chapter2::createScene()));
}

void Chapter1::onGameOver() {
    Director::getInstance()->replaceScene(Chapter1::createScene());
}
