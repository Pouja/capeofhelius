#ifndef _H_Chapter_MANAGER_
#define _H_Chapter_MANAGER_

#include "cocos2d.h"
#include "tinyxml2/tinyxml2.h"

class ChapterManager
{
public:
    enum Chapter
    {
        CHAPTER_0 = 0,
        PRE_CHAPTER_1,
        CHAPTER_1,
        CHAPTER_2,
        END_CHAPTER
    };
    typedef struct Progress {
        Chapter chapter = CHAPTER_0;
        cocos2d::Vec2 spawnPoint = cocos2d::Vec2::ZERO;
    } Progress;
private:
    // The progress of the main player
    Progress progress;

    ChapterManager() {};
    ~ChapterManager() {};

    ChapterManager(ChapterManager const&) = delete;
    void operator=(ChapterManager const&) = delete;

    /**
     * Creates an XMLElement of the progress struct
     * @param  doc      The xml document
     * @param  progress The progress struct
     * @return          An XMLElement with all the information of progress
     */
    tinyxml2::XMLElement* toXML(tinyxml2::XMLDocument* doc, Progress progress);

    /**
     * Parses an xml element to extract the progress information
     * @param  element The xml element to be parsed
     * @return         A progress struct
     */
    Progress parseXML(tinyxml2::XMLElement* element);

    /**
    * Starts the scene with the given chapter
    * @param ChapterManager::Chapter The chapter to start the game with.
    */
    void startChapter(ChapterManager::Chapter);
public:
    static ChapterManager* getInstance();
    Chapter getCurrent();

    /**
     * Replaces the current scene with the next one in line
     */
    void next();

    /**
     * Loads the configuration file and starts the scene
     */
    void init();

    /**
     * Saves the current progress
     */
    void save();

    /**
     * @return Gets the current progress
     */
    Progress getProgress();
};

#endif
