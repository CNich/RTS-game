#ifndef __ENEMY_BASIC_UNIT_RANGED__
#define __ENEMY_BASIC_UNIT_RANGED__

#include "src/Units/BasicUnit.h"

class EnemyBasicUnitRanged : public BasicUnit
{
public:
	EnemyBasicUnitRanged();
	~EnemyBasicUnitRanged();
	static EnemyBasicUnitRanged* create();
	static EnemyBasicUnitRanged* create(cocos2d::Point tmp);
	

	//void attack(BasicUnit *attacker, int damage, char attackType);
	int attackRange = 6;
	virtual void attack(BasicUnit *attacker, int damage, char attackType);

protected:
	bool enemyIsAttackable();
	virtual void update(float dt);
	bool removeThisUnit = false;
	bool removeFromPf = true;
	int health = 40;
	bool movingAlongBFSMap = false;
};

#endif // __ENEMY_BASIC_UNIT_RANGED__
