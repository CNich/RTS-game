#ifndef __ENEMY_BASIC_UNIT_RANGED__
#define __ENEMY_BASIC_UNIT_RANGED__

#include "src/Units/EnemyBasicUnit.h"

class EnemyBasicUnitRanged : public EnemyBasicUnit
{
public:
	EnemyBasicUnitRanged();
	~EnemyBasicUnitRanged();
	static EnemyBasicUnitRanged* create();
	static EnemyBasicUnitRanged* create(cocos2d::Point tmp);
	

	//void attack(BasicUnit *attacker, int damage, char attackType);
	int attackRange = 27;
	//virtual void attack(BasicUnit *attacker, int damage, char attackType);

	bool tracked = false;

protected:
	bool enemyIsAttackable();
	//virtual void update(float dt);
	bool removeThisUnit = false;
	bool removeFromPf = true;
	//int health = 40;
	bool movingAlongBFSMap = false;
	virtual void animationAttack();
	virtual cocos2d::Animate* animationWalk();
	virtual void animationDie();
	virtual void removeFromLevel();
};

#endif // __ENEMY_BASIC_UNIT_RANGED__
