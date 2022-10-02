#pragma once

#include "cocos2d.h"
USING_NS_CC;

class Chip
{
	int type = -1;
public:
	Chip(int type = -1)
	{
		this->type = type;
		sprite = nullptr;
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

