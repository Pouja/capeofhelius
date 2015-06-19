#ifndef _DIALOG_ACTION_H_
#define _DIALOG_ACTION_H_

#include "base/GameHub.h"
#include "player/Player.h"
#include "cocos2d.h"

class DialogAction : public cocos2d::Ref {
private:
    bool isText = false;
    bool isMove = false;
    bool isAction = false;
    cocos2d::FiniteTimeAction* action;
    std::queue<std::string> text;
    GameHub* hub;
    cocos2d::Rect targetRect;
    Player* player;
    DialogAction* next = nullptr;

public:
    void run(std::function<void()>);
    DialogAction* setNext(DialogAction* action);
    DialogAction(std::queue<std::string> _text, GameHub* _hub):
        isText(true), isMove(false), isAction(false), text(_text) {
            this->hub = _hub;
            this->hub->retain();
        };

    DialogAction(cocos2d::Rect _target, Player* _player):
        isText(false), isMove(true), isAction(false) , targetRect(_target) {
            this->player = _player;
            this->player->retain();
        };

    DialogAction(cocos2d::FiniteTimeAction* _action, Player* _target):
        isText(false), isMove(false), isAction(true) {
            this->action = _action;
            this->action->retain();
            this->player = _target;
            this->player->retain();
        };
};

#endif
