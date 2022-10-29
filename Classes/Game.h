#ifndef GAME_H
#define GAME_H

#include "Chip.h"

//Field limits
constexpr static int MIN_FIELD_SIZE{ 5 };
constexpr static int MAX_FIELD_SIZE{ 24 };
constexpr static int START_FIELD_SIZE{ 7 };

constexpr static int MIN_CHIP_AMOUNT{ 3 };
constexpr static int MAX_CHIP_AMOUNT{ 13 };
constexpr static int START_CHIP_AMOUNT{ 5 };

using pairSet = std::set<std::pair<int, int>>;

//Game logic
struct SelectedChip
{
	Chip* data{ nullptr };
	int i{ -1 };
	int j{ -1 };
};

class Game
{
public:
	int fieldSizeInTiles{ START_FIELD_SIZE };
	int chipTypesAmount{ START_CHIP_AMOUNT };

	std::vector<std::vector<Chip*>> field;


	void create(const int fieldSize, const int chipAmount);
	Chip* createChip(const int i, const int j);
	
	void explodeChips(const pairSet& chips);
	bool isMovePossible();
	bool getLines(const int i, const int j, pairSet& result);
};

#endif	// GAME_H
