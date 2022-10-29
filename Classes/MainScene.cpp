#include "MainScene.h"
#include "GameOverScene.h"

USING_NS_CC;

constexpr int CHIP_SIZE{ 32 };

int fieldSizeInTiles{0};	// TODO static member
int chipTypesAmount{0};	 // TODO static member

Game* game = new Game();

Scene* MainScene::createScene(int fieldSize, int chipTypesNum)
{
	if (fieldSize > 0)
	{
		fieldSizeInTiles = fieldSize;
	}
	if (chipTypesNum > 0)
	{
		chipTypesAmount = chipTypesNum;
	}
	game->create(fieldSizeInTiles, chipTypesAmount);	// TODO ?
	return MainScene::create();
}

bool MainScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	// generate field and ships data

	auto visibleSize = Director::getInstance()->getVisibleSize();

	// create background
	auto background = DrawNode::create();
	background->drawSolidRect(Director::getInstance()->getVisibleOrigin(), visibleSize, Color4F(0.2, 0.2, 0.2, 1.0));
	this->addChild(background);

	// create grid
	float baseWidth = fieldSizeInTiles * CHIP_SIZE;
	float baseHeight = fieldSizeInTiles * CHIP_SIZE;
	fieldOrigin = Vec2((visibleSize.width - baseWidth) * 0.5, (visibleSize.height - baseHeight) * 0.5);
	for (auto i{ 0 }; i < fieldSizeInTiles; ++i)
	{
		for (auto j{ 0 }; j < fieldSizeInTiles; ++j)
		{
			auto backSprite = Sprite::create("back.png");
			backSprite->setAnchorPoint(Vec2::ZERO);
			backSprite->setPosition(fieldOrigin.x + i * CHIP_SIZE, fieldOrigin.y + j * CHIP_SIZE);
			this->addChild(backSprite, 0);
		}
	}

	// create game field
	fillEmptyTiles();
	
	// add touch listener
	auto eventListener = EventListenerTouchOneByOne::create();
	eventListener->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);
	return true;
}

bool MainScene::onTouchBegan(Touch* touch, Event* event)
{
	Vec2 touchLocation = touch->getLocation();
	const int i{ static_cast<int>(touchLocation.x - fieldOrigin.x) / CHIP_SIZE };
	const int j{ static_cast<int>(touchLocation.y - fieldOrigin.y) / CHIP_SIZE };

	// check for limits
	if (i < 0 || i > fieldSizeInTiles - 1 || j < 0 || j > fieldSizeInTiles - 1)
		return true;
	if (game->field[i][j] == nullptr)
	{
		return true;
	}
	if (selectedChip->data == nullptr)
	{
		game->field[i][j]->select();
		selectedChip->data = game->field[i][j];
		selectedChip->i = i;
		selectedChip->j = j;
		return true;
	}
	if (selectedChip->i == i && selectedChip->j == j)
	{
		game->field[i][j]->unselect();
		selectedChip->data = nullptr;
		selectedChip->i = -1;
		selectedChip->j = -1;
		return true;
	}
	int distX{ abs(selectedChip->i - i) };
	int distY{ abs(selectedChip->j - j) };
	if (distX <= 1 && distY <= 1 && (distX != distY))
	{
		game->field[i][j]->select();

		// fake swap
		Vec2 firstChipPosition{ game->field[i][j]->sprite->getPositionX(), game->field[i][j]->sprite->getPositionY()};
		Vec2 secondChipPosition{selectedChip->data->sprite->getPositionX(), selectedChip->data->sprite->getPositionY()};
		auto moveAction1 = MoveTo::create(0.3, firstChipPosition);
		auto moveAction2 = MoveTo::create(0.3, secondChipPosition);
		selectedChip->data->sprite->runAction(moveAction1);
		game->field[i][j]->sprite->runAction(moveAction2);
		

		if (*game->field[i][j] == *selectedChip->data || !explodeIfPossible(std::make_pair(i, j), std::make_pair(selectedChip->i, selectedChip->j)))
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

			selectedChip->data->sprite->runAction(Sequence::create(actions1));
			game->field[i][j]->sprite->runAction(Sequence::create(actions2));
		}
		selectedChip->data = nullptr;
		selectedChip->i = -1;
		selectedChip->j = -1;
		return true;
	}
	else
	{
		selectedChip->data->unselect();
		game->field[i][j]->select();
		selectedChip->data = game->field[i][j];
		selectedChip->i = i;
		selectedChip->j = j;
	}
	return true;
}

void MainScene::fillEmptyTiles()
{
	for (int i{ 0 }; i < fieldSizeInTiles; ++i)
	{
		for (int j{ 0 }; j < fieldSizeInTiles; ++j)
			if (game->field[i][j] == nullptr)
			{
				Sprite* sprite = game->createChip(i, j)->sprite;

				sprite->setAnchorPoint(Vec2::ZERO);
				sprite->setPosition(fieldOrigin.x + i * CHIP_SIZE, fieldOrigin.y + j * CHIP_SIZE);
				sprite->setOpacity(1);
				this->addChild(sprite, 1);

				cocos2d::Vector<cocos2d::FiniteTimeAction*> actions;
				actions.pushBack(DelayTime::create(0.3));
				actions.pushBack(FadeTo::create(0.3, 255));
				sprite->runAction(Sequence::create(actions));
			}
	}
	auto callback = CallFunc::create(CC_CALLBACK_0(MainScene::explodeAllIfPossible, this));
	auto pauseAction = DelayTime::create(0.3);
	this->runAction(Sequence::create(pauseAction, callback, nullptr));
}

bool MainScene::explodeAllIfPossible()	// use update input
{
	pairSet result{};
	for (int i{ 0 }; i < fieldSizeInTiles; ++i)
	{
		for (int j{ 0 }; j < fieldSizeInTiles; ++j)
		{
			game->getLines(i, j, result);
		}
	}
	if (result.size() >= 3)
	{
		executeExplosion(result);
		return true;
	}
	if (!game->isMovePossible())
	{
		auto scene = GameOverScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));
	}
	return false;
}

bool MainScene::explodeIfPossible(std::pair<int, int> x, std::pair<int, int> y)
{
	int i1{ x.first };
	int j1{ x.second };
	int i2{ y.first };
	int j2{ y.second };
	std::swap(game->field[i1][j1], game->field[i2][j2]);

	// check both chips
	pairSet result;
	if (game->getLines(i1, j1, result) || game->getLines(i2, j2, result))
	{
		//explode
		game->field[i1][j1]->unselect();
		game->field[i2][j2]->unselect();
		executeExplosion(result);
		return true;
	}
	else
	{
		// swap backwards
		std::swap(game->field[i2][j2], game->field[i1][j1]);
		return false;
	}
}

void MainScene::executeExplosion(const pairSet& chips)
{
	game->explodeChips(chips);

	auto callback = CallFunc::create(CC_CALLBACK_0(MainScene::dropTilesDown, this));
	auto pauseAction = DelayTime::create(0.3);
	this->runAction(Sequence::create(pauseAction, callback, nullptr));
}

void MainScene::dropTilesDown()
{
	for (int i{ 0 }; i < fieldSizeInTiles; ++i)
	{
		int gapCounter{ 0 };
		for (int j{ 0 }; j < fieldSizeInTiles; ++j)
			if (game->field[i][j] == nullptr)
			{
				++gapCounter;
			}
			else if (gapCounter > 0)
			{
				int x = game->field[i][j]->sprite->getPositionX();
				int y = game->field[i][j]->sprite->getPositionY();
				auto moveAction = MoveTo::create(0.3, Vec2(x, y - CHIP_SIZE * gapCounter));
				game->field[i][j]->sprite->runAction(moveAction);

				game->field[i][j - gapCounter] = game->field[i][j];
				game->field[i][j] = nullptr;
			}
	}
	fillEmptyTiles();
}
