#ifndef __ENEMY_GOBLIN_RANGED__
#define __ENEMY_GOBLIN_RANGED__

#include "src/Units/EnemyBasicUnitRanged.h"

class EnemyGoblinRanged : public EnemyBasicUnitRanged
{
public:
	EnemyGoblinRanged();
	~EnemyGoblinRanged();
	static EnemyGoblinRanged* create();
	static EnemyGoblinRanged* create(cocos2d::Point tmp);
	

	//void attack(BasicUnit *attacker, int damage, char attackType);
	//virtual void attack(BasicUnit *attacker, int damage, char attackType);

	bool tracked = false;

protected:
	//virtual bool enemyIsAttackable();
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
	int initAttackRange = 12;
	int initMovementRange = 16;
	//int initHealth = 120;
	int initAttackDamage = 100;
	float initWalkingSpeed = 0.35;
	int initLoot = 100;
	float initAttackSpeed = 1.0f;
	/********************************************/
};

#endif // __ENEMY_GOBLIN_RANGED__
