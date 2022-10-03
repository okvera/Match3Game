#pragma once

#include "Chip.h"

using namespace std;

//Field limits
static const int MIN_FIELD_SIZE = 5;
static const int MAX_FIELD_SIZE = 24;
static const int START_FIELD_SIZE = 7;

static const int MIN_CHIP_AMOUNT = 3;
static const int MAX_CHIP_AMOUNT = 13;
static const int START_CHIP_AMOUNT = 5;

//Game logic
struct SelectedChip
{
	Chip* data = nullptr;
	int i = -1;
	int j = -1;
};

class Game
{
public:
	int fieldSizeInTiles = { START_FIELD_SIZE };
	int chipTypesAmount = { START_CHIP_AMOUNT };

	vector<vector<Chip*>> field;


	void create(const int fieldSize, const int chipAmount);
	Chip* createChip(const int i, const int j);
	
	void explodeChips(const set<pair<int, int>>& chips);
	bool isMovePossible();
	bool getLines(const int i, const int j, set<pair<int, int>>& result);
};