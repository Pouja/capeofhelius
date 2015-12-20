#ifndef _H_Chapter_MANAGER_
#define _H_Chapter_MANAGER_


class ChapterManager
{
public:
    enum Chapter
    {
        CHAPTER_0,
        PRE_CHAPTER_1,
        CHAPTER_1,
        CHAPTER_2,
        END_CHAPTER
    };
private:
    Chapter current = CHAPTER_0;
    ChapterManager(){};
    ~ChapterManager(){};

    ChapterManager(ChapterManager const&) = delete;
    void operator=(ChapterManager const&) = delete;

public:
    static ChapterManager* getInstance();
    Chapter getCurrent();
    void next();
    void setChapter(ChapterManager::Chapter);
    void init();
};

#endif
