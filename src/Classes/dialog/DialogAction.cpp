#include "DialogAction.h"

USING_NS_CC;

void DialogAction::run(std::function<void()> cb) {
    std::function<void()> onFinish = [this,cb]{
        if(this->next == nullptr){
            cb();
        } else {
            this->next->run(cb);
        }
    };

    if (isText) {
        this->hub->setText(this->text, onFinish);
    } else if (isMove) {
        this->player->moveTo(this->targetRect, onFinish);
    } else if (isAction) {
        this->player->runAction(Sequence::create(this->action, CallFunc::create([onFinish] {
            onFinish();
        }), nullptr));
    }
}

DialogAction* DialogAction::setNext(DialogAction* _next){
    this->next = _next;
    return _next;
}
