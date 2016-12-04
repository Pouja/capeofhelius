#ifndef _GAME_HUB_H_
#define _GAME_HUB_H_

#include "cocos2d.h"
#include "utils/CCLabelBMFontAnimated.h"

class GameHub : public cocos2d::Node {
private:
    cocos2d::Sprite* goldCoin;
    cocos2d::Sprite* xGoldCoin;
    cocos2d::Sprite* n1GoldCoin;
    cocos2d::Sprite* n2GoldCoin;
    cocos2d::Vec2 livesPosition;
    cocos2d::Sprite* textbox;
    cocos2d::Sprite* pulser;
    CCLabelBMFontAnimated* label;
    std::queue<std::string> textQueue;
    std::function<void()> callback;

    bool isAnimatingText;
    GameHub() : isAnimatingText(false){};

    void runPulse();
    void stopPulse();
public:
    /**
     * Initializes the game hub, sets all the labels and their positions and default values.
     * @return true (always)
     */
    virtual bool init();

    /**
     * Toggles between viewing the lives and the score of the player.
     */
    void toggle(bool visibility);

    /**
     * Clears the text in the textbox.
     */
    void clearText();

    /**
     * Sets the score
     * @param number The new score.
     */
    void setCoins(int number);

    /**
     * Sets the number of lives.
     * @param lives The new number of lives.
     */
    void setLives(int lives);

    /**
     * Sets the new queue of text to be shown and calls onFinish when all messages were view by the user.
     * @param text     The list of text to be shown
     * @param onFinish (Optional) Callback to be called after the queue is empty.
     */
    void setText(std::queue<std::string> text, std::function<void()> onFinish);
    void setText(std::queue<std::string> text);

    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    CREATE_FUNC(GameHub);
};

#endif
