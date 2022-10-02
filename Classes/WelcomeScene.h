#pragma once

#include "cocos2d.h"
USING_NS_CC;

const int MIN_FIELD_SIZE = 3;
const int MAX_FIELD_SIZE = 24;
const int START_FIELD_SIZE = 7;

const int MIN_CHIP_AMOUNT = 2;
const int MAX_CHIP_AMOUNT = 13;
const int START_CHIP_AMOUNT = 5;

class WelcomeScene : public Scene
{
    
public:
    static Scene* createScene();

    virtual bool init();

    int fieldSize = START_FIELD_SIZE;
    int chipTypesNum = START_CHIP_AMOUNT;

    Label* fieldSizeLabel;
    Label* chipTypesNumLabel;

    CREATE_FUNC(WelcomeScene);
};

