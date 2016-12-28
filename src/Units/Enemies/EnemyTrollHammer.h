#ifndef __ENEMY_TROLL_HAMMER__
#define __ENEMY_TROLL_HAMMER__

#include "src/Units/EnemyBasicUnit.h"

class EnemyTrollHammer : public EnemyBasicUnit
{
public:
	EnemyTrollHammer();
	~EnemyTrollHammer();
	static EnemyTrollHammer* create();
	static EnemyTrollHammer* create(cocos2d::Point tmp);


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

    virtual bool enemyIsAttackable();

	/*initialization variables*/
	/********************************************/
	int initAttackRange = 10;
	int initMovementRange = 10;
	//int initHealth = 120;
	int initAttackDamage = 200;
	float initWalkingSpeed = 0.35;
	float initAttackSpeed = 1.5;
	int initLoot = 300;
	/********************************************/
};

#endif // __ENEMY_TROLL_HAMMER__
