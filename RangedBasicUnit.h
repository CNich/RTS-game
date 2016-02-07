#ifndef __RANGED_BASIC_UNIT_H__
#define __RANGED_BASIC_UNIT_H__

#include "BasicUnit.h"

class RangedBasicUnit : public BasicUnit
{
public:
	RangedBasicUnit();
	~RangedBasicUnit();
	static RangedBasicUnit* create();
	static RangedBasicUnit* create(cocos2d::Point tmp);
	

	//void attack(BasicUnit *attacker, int damage, char attackType);
	int attackRange = 6;

protected:
	bool enemyIsAttackable();
	virtual void update(float dt);
	bool removeThisUnit = false;
	bool removeFromPf = true;
	int health = 40;
};

#endif // __RANGED_BASIC_UNIT_H__
