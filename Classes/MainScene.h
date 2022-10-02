#pragma once

#include "cocos2d.h"
#include "Chip.h"

USING_NS_CC;

using namespace std;

class MainScene : public Scene
{

public:
	Vec2 fieldOrigin = Vec2(Vec2::ZERO);

	static Scene* createScene(int fieldSize = 0, int chipTypesNum = 0);

	virtual bool init();

	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

	void swapChips(Chip* x, Chip* y, float delay = 0.0);
	bool explodeIfPossible(pair<int, int> x, pair<int, int> y);
	bool explodeAllIfPossible();
	bool getLines(const vector<vector<Chip*>>& vec, const int i, const int j, set<pair<int, int>>& result);
	void executeExplosion(const set<pair<int, int>>& result);
	void dropTilesDown();
	void fillEmptyTiles();
	bool isMovePossible();

	CREATE_FUNC(MainScene);
};

