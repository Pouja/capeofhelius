#ifndef _TITLE_SCREEN_H_
#define _TITLE_SCREEN_H_

#include "cocos2d.h"

class TitleScreen : public cocos2d::Layer {
private:
	cocos2d::Label* title;
	cocos2d::Label* message;
	TitleScreen(){};
    /**
     * Initializes all the labels and sets their positions.
     * @param  title   The text to be shown in the title lable.
     * @param  message The text to be shown in the message lable.
     * @param  withDim Indicate to reduce the opacity from 1 when it is true.
     * @return         true (always)
     */
    bool init(const std::string& title, const std::string& message, bool withDim);

public:
    /**
     * Renders a new title screen with the given title and message.
     * @param  title
     * @param  message
     * @param  withDim If set to true it will set the opacity to 0.7 otherwise to 1.
     * @return
     */
	static TitleScreen* create(const std::string& title, const std::string& message, bool withDim);

    /**
     * Sets a new title and message
     * @param title
     * @param message
     */
	void setText(const std::string& title, const std::string& message);
};

#endif
