#include "Game.h"

void Game::create(int fieldSize, int chipAmount)
{
	this->fieldSizeInTiles = fieldSize;
	this->chipTypesAmount = chipAmount;

	if (!field.empty())
	{
		field.clear();
	}
	field.resize(fieldSizeInTiles, vector<Chip*>(fieldSizeInTiles));

	srand(time(NULL));
}

Chip* Game::createChip(const int i, const int j)
{
	int chipType = rand() % chipTypesAmount + 1;

	field[i][j] = new Chip(chipType);
	return field[i][j];
}

void Game::explodeChips(const set<pair<int, int>>& chips)
{
	for (auto iter = chips.begin(); iter != chips.end(); iter++)
	{
		//Chip* chip = field[iter->first][iter->second];
		field[iter->first][iter->second]->explode();
		field[iter->first][iter->second] = nullptr;
	}
}

bool Game::isMovePossible()
{
	Chip* throughTwo = nullptr;
	Chip* throughOne = nullptr;
	Chip* current = nullptr;

	for (int i = 0; i < fieldSizeInTiles; i++)
	{
		for (int j = 0; j < fieldSizeInTiles; j++)
		{
			current = field[i][j];
			if (i > 2)
			{
				throughTwo = field[i - 3][j];
				throughOne = field[i - 2][j];
				if (*throughTwo == *current && *throughOne == *current)
				{
					return true;
				}
			}
			if (i < fieldSizeInTiles - 3)
			{
				throughTwo = field[i + 3][j];
				throughOne = field[i + 2][j];
				if (*throughTwo == *current && *throughOne == *current)
				{
					return true;
				}
			}
			if (j > 2)
			{
				throughTwo = field[i][j - 3];
				throughOne = field[i][j - 2];
				if (*throughTwo == *current && *throughOne == *current)
				{
					return true;
				}
			}
			if (j < fieldSizeInTiles - 3)
			{
				throughTwo = field[i][j + 3];
				throughOne = field[i][j + 2];
				if (*throughTwo == *current && *throughOne == *current)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool Game::getLines(const int i, const int j, set<pair<int, int>>& result)
{
	set<pair<int, int>> local;
	int size = 0;
	bool found = false;

	// horizontal check
	for (int k = i - 1; k >= 0; k--)
	{
		if (*field[k][j] == *field[k + 1][j])
		{
			local.insert(make_pair(k, j));
		}
		else
			break;
	}
	for (int k = i + 1; k < fieldSizeInTiles; k++)
	{
		if (*field[k][j] == *field[k - 1][j])
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
		if (*field[i][k] == *field[i][k + 1])
		{
			local.insert(make_pair(i, k));
		}
		else
			break;
	}
	for (int k = j + 1; k < fieldSizeInTiles; k++)
	{
		if (*field[i][k] == *field[i][k - 1])
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