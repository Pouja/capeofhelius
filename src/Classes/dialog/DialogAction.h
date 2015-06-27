#ifndef _DIALOG_ACTION_H_
#define _DIALOG_ACTION_H_

#include "base/GameHub.h"
#include "player/Player.h"
#include "cocos2d.h"

//TODO: Completely rewrite this class
class DialogAction : public cocos2d::Ref {
private:
    bool isText = false;
    bool isMove = false;
    bool isAction = false;
    bool isStatus = false;
    std::string status;
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
        isText(true), text(_text) {
            this->hub = _hub;
            this->hub->retain();
        };

    DialogAction(cocos2d::Rect _target, Player* _player):
        isMove(true) , targetRect(_target) {
            this->player = _player;
            this->player->retain();
        };

    DialogAction(cocos2d::FiniteTimeAction* _action, Player* _target):
        isAction(true) {
            this->action = _action;
            this->action->retain();
            this->player = _target;
            this->player->retain();
        };

    DialogAction(const std::string& _status, Player* _mainPlayer, GameHub* _hub):
        isStatus(true), status(_status) {
            this->hub = _hub;
            this->hub->retain();
            this->player = _mainPlayer;
            this->player->retain();
        }
};

#endif
