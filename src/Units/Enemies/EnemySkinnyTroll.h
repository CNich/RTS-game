#ifndef __ENEMY_SKINNY_TROLL__
#define __ENEMY_SKINNY_TROLL__

#include "src/Units/EnemyBasicUnit.h"

class EnemySkinnyTroll : public EnemyBasicUnit
{
public:
	EnemySkinnyTroll();
	~EnemySkinnyTroll();
	static EnemySkinnyTroll* create();
	static EnemySkinnyTroll* create(cocos2d::Point tmp);
	

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
	float initWalkingSpeed = 0.15;
	int initLoot = 300;
	/********************************************/
};

#endif // __ENEMY_SKINNY_TROLL__
