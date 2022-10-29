#ifndef GAMEOVERSCENE_H
#define GAMEOVERSCENE_H 

#include "cocos2d.h"

USING_NS_CC;

class GameOverScene : public Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(GameOverScene);
};

#endif  // GAMEOVERSCENE_H
