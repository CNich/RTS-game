#ifndef __ENEMY_BASIC_UNIT__
#define __ENEMY_BASIC_UNIT__

#include "BasicUnit.h"

class EnemyBasicUnit : public BasicUnit
{
public:
	EnemyBasicUnit();
	~EnemyBasicUnit();
	static EnemyBasicUnit* create();
	static EnemyBasicUnit* create(cocos2d::Point tmp);
	

	//void attack(BasicUnit *attacker, int damage, char attackType);
	int attackRange = 7;
	virtual void attack(BasicUnit *attacker, int damage, char attackType);

protected:
	bool enemyIsAttackable();
	virtual void update(float dt);
	bool removeThisUnit = false;
	bool removeFromPf = true;
	int health = 40;
	bool movingAlongBFSMap = false;
};

#endif // __ENEMY_BASIC_UNIT__
