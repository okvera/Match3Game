#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "cocos2d.h"
#include "Chip.h"
#include "Game.h"

USING_NS_CC;

class MainScene : public Scene
{
	Vec2 fieldOrigin = Vec2::ZERO;

	SelectedChip* selectedChip = new SelectedChip();

public:

	static Scene* createScene(int fieldSize = 0, int chipTypesNum = 0);

	virtual bool init();

	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

	void swapChips(Chip* x, Chip* y, float delay = 0.0);
	bool explodeIfPossible(std::pair<int, int> x, std::pair<int, int> y);
	bool explodeAllIfPossible();
	void executeExplosion(const pairSet& chips);
	void dropTilesDown();
	void fillEmptyTiles();

	CREATE_FUNC(MainScene);
};

#endif	// MAINSCENE_H
