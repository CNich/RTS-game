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
	int initHealth = 40;

	/*Animation section*/
	/********************************************/
	virtual cocos2d::Animate* animationWalk();
	float initWalkingSpeed = 0.15;
	bool walkingAnimationFlag = false;

	virtual void animationDie();

	virtual void animationAttack();
	/********************************************/

	int initAttackRange = 5;
	int initMovementRange = 7;

};

#endif // __RANGED_BASIC_UNIT_H__
