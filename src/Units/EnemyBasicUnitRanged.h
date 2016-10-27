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
	//virtual void attack(BasicUnit *attacker, int damage, char attackType);

	bool tracked = false;

protected:
	bool enemyIsAttackable();
	//virtual void update(float dt);
	bool removeThisUnit = false;
	bool removeFromPf = true;
	bool movingAlongBFSMap = false;
	virtual void animationAttack();
	virtual cocos2d::Animate* animationWalk();
	virtual void animationDie();
	virtual void removeFromLevel();

	/*initialization variables*/
	/********************************************/
	int initAttackRange = 13;
	int initMovementRange = 16;
	int initHealth = 120;
	int initAttackDamage = 40;
	float initWalkingSpeed = 0.15;
	int initLoot = 30;
	/********************************************/
};

#endif // __ENEMY_BASIC_UNIT_RANGED__
