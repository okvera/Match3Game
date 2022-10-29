#ifndef CHIP_H
#define CHIP_H

#include "cocos2d.h"

USING_NS_CC;

class Chip
{
	int type{ -1 };
public:
	Chip(int type = -1)
	{
		this->type = type;

		std::string chipIconName = std::to_string(type) + ".png";
		sprite = Sprite::create(chipIconName);
	};

	Sprite* sprite;	

	bool Chip::operator==(const Chip& c)
	{
		return this->type == c.type;
	}

	void select();
	void unselect();
	void explode();
};

#endif	// CHIP_H
