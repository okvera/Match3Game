#ifndef WELCOMESCENE_H
#define WELCOMESCENE_H

#include "Game.h"
#include "cocos2d.h"

USING_NS_CC;

class WelcomeScene : public Scene
{
	int fieldSize{ START_FIELD_SIZE };
	int chipTypesNum{ START_CHIP_AMOUNT };

	Label* fieldSizeLabel{};
	Label* chipTypesNumLabel{};

	const std::string fieldSizeText{ "Field Size: " };
	const std::string chipTypesText{ "Chip types: " };

public:
	static Scene* createScene();

	virtual bool init();

	CREATE_FUNC(WelcomeScene);
};

#endif  // WELCOMESCENE_H
