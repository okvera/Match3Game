#include "Chip.h"

void Chip::select()
{
	if (sprite != nullptr)
	{
		sprite->setScale(1.2, 1.2);
	}
}

void Chip::unselect()
{
	if (sprite != nullptr)
	{
		sprite->setScale(1.0, 1.0);
	}
}

void Chip::explode()
{
	if (sprite != nullptr)
	{
		cocos2d::Vector<cocos2d::FiniteTimeAction*> actions;
		actions.pushBack(FadeTo::create(0.3, 1.0));
		actions.pushBack(RemoveSelf::create());
		sprite->runAction(Sequence::create(actions));
	}
}
