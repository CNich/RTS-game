#ifndef __RANGED_BASIC_UNIT_H__
#define __RANGED_BASIC_UNIT_H__

#include "src/Units/BasicUnit.h"

class RangedBasicUnit : public BasicUnit
{
public:
	RangedBasicUnit();
	~RangedBasicUnit();
	static RangedBasicUnit* create();
	static RangedBasicUnit* create(cocos2d::Point tmp);
	
	//virtual int getAttackRange();
	//virtual int getMovementRange();

	//void attack(BasicUnit *attacker, int damage, char attackType);
	//virtual void attack(BasicUnit *attacker, int damage, char attackType);

protected:
	virtual bool enemyIsAttackable();
	//virtual void update(float dt);
	bool removeThisUnit = false;
	bool removeFromPf = true;

	/*Animation section*/
	/********************************************/
	virtual cocos2d::Animate* animationWalk();
	bool walkingAnimationFlag = false;

	virtual void animationDie();
	virtual void animationAttack();
	/********************************************/

	virtual void removeFromLevel();

	/*initialization variables*/
	/********************************************/
	int initAttackRange = 8;
	int initMovementRange = 13;
	//int initHealth = 120;
	int initAttackDamage = 40;
	float initWalkingSpeed = 0.15;
	int initPrice = 250;
	/********************************************/
};

#endif // __RANGED_BASIC_UNIT_H__
