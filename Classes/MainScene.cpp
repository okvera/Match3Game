#include "MainScene.h"
#include "GameOverScene.h"

USING_NS_CC;
using namespace std;

int N;	// field size
int K;	// chip diversity

const int CHIP_SIZE = 32;

vector<vector<Chip*>> field;

Chip* selectedChip = nullptr;
int selectedChipI = -1;
int selectedChipJ = -1;

static bool needToCheckState = false;

Scene* MainScene::createScene(int fieldSize, int chipTypesNum)
{
	if (fieldSize > 0)
	{
		N = fieldSize;
	}
	if (chipTypesNum > 0)
	{
		K = chipTypesNum;
	}
	
	return MainScene::create();
}

bool MainScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	// generate field and ships data
	srand(time(NULL));

	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto background = DrawNode::create();
	background->drawSolidRect(Director::getInstance()->getVisibleOrigin(), visibleSize, Color4F(0.2, 0.2, 0.2, 1.0));
	this->addChild(background);

	// create background grid
	float baseWidth, baseHeight;
	baseWidth = baseHeight = N * CHIP_SIZE;
	fieldOrigin = Vec2((visibleSize.width - baseWidth) * 0.5, (visibleSize.height - baseHeight) * 0.5);
	for (size_t i = 0; i < N; i++)
	{
		for (size_t j = 0; j < N; j++)
		{
			auto backSprite = Sprite::create("back.png");
			backSprite->setAnchorPoint(Vec2(Vec2::ZERO));
			backSprite->setPosition(fieldOrigin.x + i * CHIP_SIZE, fieldOrigin.y + j * CHIP_SIZE);
			this->addChild(backSprite, 0);
		}
	}

	// create game field
	int counter = 0;
	if (!field.empty())
	{
		field.clear();
	}
	field.resize(N, vector<Chip*>(N));
	fillEmptyTiles();
	
	auto eventListener = EventListenerTouchOneByOne::create();
	eventListener->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);
	return true;
}

bool MainScene::onTouchBegan(Touch* touch, Event* event)
{
	Vec2 touchLocation = touch->getLocation();
	int i = (touchLocation.x - fieldOrigin.x) / CHIP_SIZE;
	int j = (touchLocation.y - fieldOrigin.y) / CHIP_SIZE;

	// check for limits
	if (i < 0 || i > N - 1 || j < 0 || j > N - 1)
		return true;
	if (field[i][j] == nullptr)
	{
		return true;
	}
	if (selectedChip == nullptr)
	{
		field[i][j]->select();
		selectedChip = field[i][j];
		selectedChipI = i;
		selectedChipJ = j;
		return true;
	}
	if (selectedChipI == i && selectedChipJ == j)
	{
		field[i][j]->unselect();
		selectedChip = nullptr;
		selectedChipI = -1;
		selectedChipJ = -1;
		return true;
	}
	int distX = abs(selectedChipI - i);
	int distY = abs(selectedChipJ - j);
	if (distX <= 1 && distY <= 1 && (distX != distY))
	{
		field[i][j]->select();
		// fake swap
		
		int x1 = field[i][j]->sprite->getPositionX();
		int y1 = field[i][j]->sprite->getPositionY();
		int x2 = selectedChip->sprite->getPositionX();
		int y2 = selectedChip->sprite->getPositionY();
		auto moveAction1 = MoveTo::create(0.3, Vec2(x1, y1));
		auto moveAction2 = MoveTo::create(0.3, Vec2(x2, y2));
		selectedChip->sprite->runAction(moveAction1);
		field[i][j]->sprite->runAction(moveAction2);
		

		if (*field[i][j] == *selectedChip || !explodeIfPossible(make_pair(i, j), make_pair(selectedChipI, selectedChipJ)))
		{
			// fake swap backwards
			cocos2d::Vector<cocos2d::FiniteTimeAction*> actions1;
			actions1.pushBack(DelayTime::create(0.3));
			actions1.pushBack(moveAction2);
			actions1.pushBack(ScaleTo::create(0.3, 1.0));

			cocos2d::Vector<cocos2d::FiniteTimeAction*> actions2; 
			actions2.pushBack(DelayTime::create(0.3));
			actions2.pushBack(moveAction1);
			actions2.pushBack(ScaleTo::create(0.3, 1.0));

			selectedChip->sprite->runAction(Sequence::create(actions1));
			field[i][j]->sprite->runAction(Sequence::create(actions2));
		}
		selectedChip = nullptr;
		selectedChipI = -1;
		selectedChipJ = -1;
		return true;
	}
	else
	{
		selectedChip->unselect();
		field[i][j]->select();
		selectedChip = field[i][j];
		selectedChipI = i;
		selectedChipJ = j;
	}
	return true;
}

void MainScene::dropTilesDown()
{
	bool needToDrop = false;
	for (size_t i = 0; i < N; i++)
	{
		int gapCounter = 0;
		for (size_t j = 0; j < N; j++)
			if (field[i][j] == nullptr)
			{
				gapCounter++;
			}
			else if (gapCounter > 0)
			{
				needToDrop = true;
				int x1 = field[i][j]->sprite->getPositionX();
				int y1 = field[i][j]->sprite->getPositionY();
				auto moveAction = MoveTo::create(0.3, Vec2(x1, y1 - CHIP_SIZE * gapCounter));
				field[i][j]->sprite->runAction(moveAction);

				field[i][j - gapCounter] = field[i][j];
				field[i][j] = nullptr;
			}
	}
	fillEmptyTiles();
}

void MainScene::fillEmptyTiles()
{
	for (size_t i = 0; i < N; i++)
	{
		for (size_t j = 0; j < N; j++)
			if (field[i][j] == nullptr)
			{
				int chipId = rand() % K + 1;

				string chipIconName = to_string(chipId) + ".png";
				auto chipSprite = Sprite::create(chipIconName);
				chipSprite->setAnchorPoint(Vec2(0, 0));
				chipSprite->setPosition(fieldOrigin.x + i * CHIP_SIZE, fieldOrigin.y + j * CHIP_SIZE);
				chipSprite->setOpacity(1);

				cocos2d::Vector<cocos2d::FiniteTimeAction*> actions;
				actions.pushBack(DelayTime::create(0.3));
				actions.pushBack(FadeTo::create(0.3, 255));
				chipSprite->runAction(Sequence::create(actions));

				this->addChild(chipSprite, 1);
				Chip* c = new Chip(chipId);
				c->sprite = chipSprite;
				field[i][j] = c;
			}
	}
	auto callback = CallFunc::create(CC_CALLBACK_0(MainScene::explodeAllIfPossible, this));
	auto pauseAction = DelayTime::create(0.3);
	this->runAction(Sequence::create(pauseAction, callback, nullptr));
}

bool MainScene::explodeAllIfPossible()	// use update input
{
	set<pair<int, int>> result;
	for (size_t i = 0; i < N; i++)
	{
		for (size_t j = 0; j < N; j++)
		{
			getLines(field, i, j, result);
		}
	}
	if (result.size() >= 3)
	{
		executeExplosion(result);
		return true;
	}
	if (!isMovePossible())
	{
		//field.clear();
		auto scene = GameOverScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));
	}
	return false;
}

bool MainScene::explodeIfPossible(pair<int, int> x, pair<int, int> y)
{
	int i1 = x.first;
	int j1 = x.second;
	int i2 = y.first;
	int j2 = y.second;
	swap(field[i1][j1], field[i2][j2]);

	// check both chips
	set<pair<int, int>> result;
	if (getLines(field, i1, j1, result) || getLines(field, i2, j2, result))
	{
		//explode
		field[i1][j1]->unselect();
		field[i2][j2]->unselect();
		executeExplosion(result);
		return true;
	}
	else
	{
		// swap backwards
		swap(field[i2][j2], field[i1][j1]);
		return false;
	}
}

void MainScene::executeExplosion(const set<pair<int, int>>& result)
{
	for (auto iter = result.begin(); iter != result.end(); iter++)
	{
		field[iter->first][iter->second]->explode();
		field[iter->first][iter->second] = nullptr;
	}
	// exploded
	auto callback = CallFunc::create(CC_CALLBACK_0(MainScene::dropTilesDown, this));
	auto pauseAction = DelayTime::create(0.3);
	this->runAction(Sequence::create(pauseAction, callback, nullptr));
}

bool MainScene::isMovePossible()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (i > 2)
			{
				if (*field[i - 3][j] == *field[i][j] && *field[i - 2][j] == *field[i][j])
				{
					return true;
				}
			}
			if (i < N - 3)
			{
				if (*field[i + 3][j] == *field[i][j] && *field[i + 2][j] == *field[i][j])
				{
					return true;
				}
			}
			if (j > 2)
			{
				if (*field[i][j - 3] == *field[i][j] && *field[i][j - 2] == *field[i][j])
				{
					return true;
				}
			}
			if (j < N - 3)
			{
				if (*field[i][j + 3] == *field[i][j] && *field[i][j + 2] == *field[i][j])
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool MainScene::getLines(const vector<vector<Chip*>>& vec, const int i, const int j, set<pair<int, int>>& result)
{
	set<pair<int, int>> local;
	int size = 0;
	bool found = false;

	// horizontal check
	for (int k = i - 1; k >= 0; k--)
	{
		if (*vec[k][j] == *vec[k + 1][j])
		{
			local.insert(make_pair(k, j));
		}
		else
			break;
	}
	for (int k = i + 1; k < N; k++)
	{
		if (*vec[k][j] == *vec[k - 1][j])
		{
			local.insert(make_pair(k, j));
		}
		else
			break;
	}
	if (local.size() >= 2)
	{
		found = true;
		result.insert(local.begin(), local.end());
	}
	local.clear();

	// vertical check
	for (int k = j - 1; k >= 0; k--)
	{
		if (*vec[i][k] == *vec[i][k + 1])
		{
			local.insert(make_pair(i, k));
		}
		else
			break;
	}
	for (int k = j + 1; k < N; k++)
	{
		if (*vec[i][k] == *vec[i][k - 1])
		{
			local.insert(make_pair(i, k));
		}
		else
			break;
	}
	if (local.size() >= 2)
	{
		found = true;
		result.insert(local.begin(), local.end());
	}

	// add initial chip
	if (found)
	{
		result.insert(make_pair(i, j));
	}
	return found;
}
