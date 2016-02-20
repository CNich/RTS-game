#ifndef __NINJA__
#define __NINJA__

#include "BasicUnit.h"

class Ninja : public BasicUnit
{
public:
	Ninja();
	~Ninja();
	static Ninja* create();
	static Ninja* create(cocos2d::Point tmp);
	

	//void attack(BasicUnit *attacker, int damage, char attackType);
	int attackRange = 6;
	virtual void attack(BasicUnit *attacker, int damage, char attackType);

protected:
	bool enemyIsAttackable();
	virtual void update(float dt);
	bool removeThisUnit = false;
	bool removeFromPf = true;
	int health = 200;
};

#endif // __NINJA__
