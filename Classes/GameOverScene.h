#pragma once

#include "cocos2d.h"
USING_NS_CC;

class GameOverScene : public Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(GameOverScene);
};

