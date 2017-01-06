#ifndef __GOBLIN_MELEE__
#define __GOBLIN_MELEE__

#include "src/Units/EnemyBasicUnit.h"

class EnemyGoblinMelee : public EnemyBasicUnit
{
public:
	EnemyGoblinMelee();
	~EnemyGoblinMelee();
	static EnemyGoblinMelee* create();
	static EnemyGoblinMelee* create(cocos2d::Point tmp);


	//void attack(BasicUnit *attacker, int damage, char attackType);
	//virtual void attack(BasicUnit *attacker, int damage, char attackType);

	bool tracked = false;

protected:
	bool removeThisUnit = false;
	bool removeFromPf = true;
	bool movingAlongBFSMap = false;
	virtual void animationAttack();
	virtual cocos2d::Animate* animationWalk();
	virtual void animationDie();
	virtual void removeFromLevel();

    //virtual bool enemyIsAttackable();

	/*initialization variables*/
	/********************************************/
	int initAttackRange = 10;
	int initMovementRange = 10;
	//int initHealth = 120;
	int initMaxHealth = 500;
	int initAttackDamage = 200;
	float initWalkingSpeed = 0.25;
	float initAttackSpeed = 0.9;
	int initLoot = 300;
	/********************************************/
};

#endif // __GOBLIN_MELEE__
